#include "Arduino.h"
#include "PN532.h"
#include "PN532_debug.h"
#include <string.h>

#define HAL(func)   (_interface->func)

PN532::PN532(PN532Interface &interface)
{
    _interface = &interface;
}

bool PN532::SAMConfig(){
    pn532_packetbuffer[0] = PN532_COMMAND_SAMCONFIGURATION;
    pn532_packetbuffer[1] = 0x01; // normal mode;
    pn532_packetbuffer[2] = 0x14; // timeout 50ms * 20 = 1 second
    pn532_packetbuffer[3] = 0x01; // use IRQ pin!

    DMSG("SAMConfig\n");

    if(_interface->writeCommand(pn532_packetbuffer,4,NULL,(uint8_t)0)){
        return false;
    }
    uint8_t s=PN532_PACKET_BUF_LEN;
    _interface->readResponse(pn532_packetbuffer,&s);
    return 0<s;
}

uint32_t PN532::getFirmwareVersion(void){
    uint32_t response;

    pn532_packetbuffer[0] = PN532_COMMAND_GETFIRMWAREVERSION;

    if (_interface->writeCommand(pn532_packetbuffer,(uint8_t)1,NULL,(uint16_t)0)) {
        return 0;
    }

    uint8_t length=5;
    // read data packet
    int16_t status = _interface->readResponse(pn532_packetbuffer,&length,1000);
    if (0 > status) {
        return 0;
    }

    response = pn532_packetbuffer[1];
    response <<= 8;
    response |= pn532_packetbuffer[2];
    response <<= 8;
    response |= pn532_packetbuffer[3];
    response <<= 8;
    response |= pn532_packetbuffer[4];

    return response;
}

uint8_t PN532::inListPassiveTarget(const uint8_t maxtg,const PN532::Type type,const uint8_t *data,const uint8_t datalen,uint8_t *targetdata,uint8_t *targetdatalen){
    uint8_t fixedtype=type&0x0F;
    pn532_packetbuffer[0] = PN532_COMMAND_INLISTPASSIVETARGET;
    pn532_packetbuffer[1] = maxtg;
    pn532_packetbuffer[2] = fixedtype;

    *targetdatalen=0;

    if(_interface->writeCommand(pn532_packetbuffer,3,data,datalen)){
        return 0;
    }
    uint16_t length=PN532_PACKET_BUF_LEN;
    // read
    int8_t status=_interface->readResponse(pn532_packetbuffer,&length,1000);
    if(status<0){
        return 0;
    }
    uint8_t count=pn532_packetbuffer[1];
    if(count!=0){
        *targetdatalen=length-2;
        memmove(targetdata,&pn532_packetbuffer[2],*targetdatalen);
    }
    return count;
}

uint8_t PN532::inAutoPoll(const uint8_t pollNr,const uint8_t period,const PN532::Type *types,const uint8_t typeslen,uint8_t *foundtypes,uint8_t *foundlen){
    pn532_packetbuffer[0] = PN532_COMMAND_INAUTOPOLL;
    pn532_packetbuffer[1] = pollNr;
    pn532_packetbuffer[2] = period;
    *foundlen=0;

    if (_interface->writeCommand(pn532_packetbuffer,(uint8_t)3,(const uint8_t*)types,typeslen)) {
        return 0;
    }

    uint16_t length=PN532_PACKET_BUF_LEN;
    // read data packet
    int8_t status = _interface->readResponse(pn532_packetbuffer,&length,1000);
    if (0 > status) {
        return 0;
    }

    uint8_t count=pn532_packetbuffer[1];
    if(count!=0){
        *foundlen=length-2;
        memmove(foundtypes,&pn532_packetbuffer[2],*foundlen);
    }

    return count;
}
uint8_t PN532::PollingTypeA(const uint8_t maxtg,const uint8_t *selectuid,const uint8_t uidlen,PN532::PICC::TypeA *picc){
    uint8_t length=PN532_PACKET_BUF_LEN;
    uint8_t count=inListPassiveTarget(maxtg,Type::PASSIVE_TYPE_A,selectuid,uidlen,pn532_packetbuffer,&length);
    uint8_t index=0;
    for(uint8_t i=0;i<count;i++){
        picc[i].tg=pn532_packetbuffer[index];
        picc[i].uidlen=pn532_packetbuffer[4];
        for(uint8_t j=0;j<picc[i].uidlen;j++){
            picc[i].uid[j]=pn532_packetbuffer[index+5+j];
        }
        index=pn532_packetbuffer[index+4]+5+index;
    }
    return count;
}
uint8_t PN532::PollingTypeB(const uint8_t maxtg,const uint8_t afi,PN532::PICC::TypeB *picc){
    uint8_t length=PN532_PACKET_BUF_LEN;
    uint8_t count=inListPassiveTarget(maxtg,Type::PASSIVE_TYPE_B,&afi,(uint8_t)1,pn532_packetbuffer,&length);
    uint8_t index=0;
    for(uint8_t i=0;i<count;i++){
        if(pn532_packetbuffer[index+1]!=0x50){
            return 0;
        }
        picc[i].tg=pn532_packetbuffer[index];
        for(uint8_t j=0;j<4;j++){
            picc[i].pupi[j]=pn532_packetbuffer[index+2+j];
        }
        for(uint8_t j=0;j<4;j++){
            picc[i].ap[j]=pn532_packetbuffer[index+6+j];
        }
        for(uint8_t j=0;j<3;j++){
            picc[i].prot[j]=pn532_packetbuffer[index+10+j];
        }
        index=pn532_packetbuffer[index+13]+14+index;
    }
    return count;
}
uint8_t PN532::PollingFelica(const uint8_t maxtg,const uint16_t systemcode,const uint8_t req,const uint8_t timeslot,PN532::PICC::Felica *picc){
    pn532_packetbuffer[0+IN_LIST_PASSIVE_TARGET_USE_SIZE]=FELICA_CMD_POLLING;
    pn532_packetbuffer[1+IN_LIST_PASSIVE_TARGET_USE_SIZE]=(systemcode>>8)&0xFF;
    pn532_packetbuffer[2+IN_LIST_PASSIVE_TARGET_USE_SIZE]=systemcode&0xFF;
    pn532_packetbuffer[3+IN_LIST_PASSIVE_TARGET_USE_SIZE]=req;
    pn532_packetbuffer[4+IN_LIST_PASSIVE_TARGET_USE_SIZE]=timeslot;

    uint8_t length=PN532_PACKET_BUF_LEN;
    uint8_t count=inListPassiveTarget(maxtg,Type::PASSIVE_212,&pn532_packetbuffer[IN_LIST_PASSIVE_TARGET_USE_SIZE],(uint8_t)5,pn532_packetbuffer,&length);
    uint8_t index=0;
    for(uint8_t i=0;i<count;i++){
        picc[i].pcd=this;
        if(pn532_packetbuffer[index+2]!=0x01){
            return 0;
        }
        uint8_t len=pn532_packetbuffer[index+1];
        picc[i].tg=pn532_packetbuffer[index];
        for(uint8_t j=0;j<8;j++){
            picc[i].idm[j]=pn532_packetbuffer[index+3+j];
        }
        for(uint8_t j=0;j<8;j++){
            picc[i].pmm[j]=pn532_packetbuffer[index+11+j];
        }
        if(req==0x01){
            picc[i].systemcode=(((uint16_t)pn532_packetbuffer[index+19])<<8)+pn532_packetbuffer[index+20];
        }

        index+=len+1;
    }
    return count;
}

uint8_t PN532::inDataExchange(const uint8_t tg,const uint8_t **sendlist,const uint16_t *sendlenlist,const uint8_t sendcount,uint8_t *response,uint16_t *responselen){
    pn532_packetbuffer[0]=PN532_COMMAND_INDATAEXCHANGE;
    pn532_packetbuffer[1]=tg;
    *responselen=0;

    if(_interface->writeCommand(pn532_packetbuffer,2,sendlist,sendlenlist,sendcount)){
        return 0;
    }
    uint16_t length=PN532_PACKET_BUF_LEN;
    if(_interface->readResponse(pn532_packetbuffer,&length,1000)<0){
        return 0;
    }

    uint8_t status=pn532_packetbuffer[1];
    if(status&0b111111!=0){
        DMSG("Error");
        return status;
    }

    *responselen=length-2;
    memmove(response,&pn532_packetbuffer[2],*responselen);
    return status;
}
uint8_t PN532::inDataExchange(const uint8_t tg,const uint8_t *send,const uint16_t sendlen,uint8_t *response,uint16_t *responselen){
    const uint8_t *sendlist[]={send};
    return inDataExchange(tg,sendlist,&sendlen,(const uint8_t)1,response,responselen);
}

bool PN532::APDU::select(const uint8_t *id,const uint8_t idlen,const uint8_t p1,const uint8_t p2,uint8_t *status){
    
}

bool PN532::felica_requestService(const PICC::Felica *felica,const uint8_t node_count,const uint16_t *nodecode_list,uint16_t *response,uint8_t *responseLength){
    pn532_packetbuffer[IN_DATA_EXCHANGE_USE_SIZE]=11+2*node_count;
    pn532_packetbuffer[1+IN_DATA_EXCHANGE_USE_SIZE]=FELICA_CMD_REQUEST_SERVICE;
    pn532_packetbuffer[2+IN_DATA_EXCHANGE_USE_SIZE]=node_count;
    uint16_t length=PN532_PACKET_BUF_LEN;
    const uint8_t *sendlist[]={
        &pn532_packetbuffer[IN_DATA_EXCHANGE_USE_SIZE],
        felica->idm,
        &node_count,
        (uint8_t*)nodecode_list
    };
    const uint16_t sendlenlist[]={
        2,
        8,
        1,
        2*node_count
    };
    uint8_t status=inDataExchange(felica->tg,sendlist,sendlenlist,4,pn532_packetbuffer,&length);
    if(status&0b111111!=0){
        DMSG("Error");
        return false;
    }
    *responseLength=pn532_packetbuffer[10];
    memmove((uint8_t*)response,&pn532_packetbuffer[11],2*pn532_packetbuffer[10]);
    return true;
}

uint8_t PN532::felica_requestResponse(const PICC::Felica *felica){
    pn532_packetbuffer[IN_DATA_EXCHANGE_USE_SIZE]=10;
    pn532_packetbuffer[1+IN_DATA_EXCHANGE_USE_SIZE]=FELICA_CMD_REQUEST_RESPONSE;
    uint16_t length=PN532_PACKET_BUF_LEN;
    const uint8_t *sendlist[]={
        &pn532_packetbuffer[IN_DATA_EXCHANGE_USE_SIZE],
        felica->idm
    };
    const uint16_t sendlenlist[]={
        2,
        8
    };
    uint8_t status=inDataExchange(felica->tg,sendlist,sendlenlist,2,pn532_packetbuffer,&length);
    if(status&0b111111!=0){
        DMSG("Error");
        return 0xFF;
    }
    return pn532_packetbuffer[10];
}

uint16_t PN532::felica_readWithoutEncryption(const PICC::Felica *felica,const uint8_t service_count,const uint16_t *servicecode_list,const uint8_t block_count,const uint8_t *block_list,uint8_t response[][16],uint8_t *response_count){
    uint8_t blocklen=0;
    for(uint8_t i=0;i<block_count;i++){
        if((block_list[blocklen]>>7)&0x01==1){
            blocklen+=2;
        }
        else{
            blocklen+=3;
        }
    }
    pn532_packetbuffer[IN_DATA_EXCHANGE_USE_SIZE]=12+2*service_count+blocklen;
    pn532_packetbuffer[1+IN_DATA_EXCHANGE_USE_SIZE]=FELICA_CMD_READ_WITHOUT_ENCRYPTION;
    pn532_packetbuffer[2+IN_DATA_EXCHANGE_USE_SIZE]=service_count;
    uint16_t length=PN532_PACKET_BUF_LEN;
    const uint8_t *sendlist[]={
        &pn532_packetbuffer[IN_DATA_EXCHANGE_USE_SIZE],
        felica->idm,
        &pn532_packetbuffer[IN_DATA_EXCHANGE_USE_SIZE+2],
        (uint8_t*)servicecode_list,
        &block_count,
        block_list
    };
    const uint16_t sendlenlist[]={
        2,
        8,
        1,
        2*service_count,
        1,
        blocklen
    };


    uint8_t status=inDataExchange(felica->tg,sendlist,sendlenlist,6,pn532_packetbuffer,&length);
    if(status&0b111111!=0){
        DMSG("Error");
        return status;
    }

    uint16_t statusflag=pn532_packetbuffer[10];
    statusflag=(statusflag<<8)+pn532_packetbuffer[11];
    if(statusflag!=0){
        *response_count=0;
        return statusflag;
    }
    *response_count=pn532_packetbuffer[12];
    memmove((uint8_t*)response,&pn532_packetbuffer[13],16*pn532_packetbuffer[12]);
    return statusflag;
}
uint16_t PN532::felica_writeWithoutEncryption(const PICC::Felica *felica,const uint8_t service_count,const uint16_t *servicecode_list,const uint8_t block_count,const uint8_t *block_list,const uint8_t blockData[][16]){
    uint8_t blocklen=0;
    for(uint8_t i=0;i<block_count;i++){
        if((block_list[blocklen]>>7)&0x01==1){
            blocklen+=2;
        }
        else{
            blocklen+=3;
        }
    }
    pn532_packetbuffer[IN_DATA_EXCHANGE_USE_SIZE]=12+2*service_count+blocklen+block_count*16;
    pn532_packetbuffer[1+IN_DATA_EXCHANGE_USE_SIZE]=FELICA_CMD_WRITE_WITHOUT_ENCRYPTION;
    pn532_packetbuffer[2+IN_DATA_EXCHANGE_USE_SIZE]=service_count;
    uint16_t length=PN532_PACKET_BUF_LEN;
    const uint8_t *sendlist[]={
        &pn532_packetbuffer[IN_DATA_EXCHANGE_USE_SIZE],
        felica->idm,
        &pn532_packetbuffer[IN_DATA_EXCHANGE_USE_SIZE+2],
        (uint8_t*)servicecode_list,
        &block_count,
        block_list,
        (uint8_t*)blockData
    };
    const uint16_t sendlenlist[]={
        2,
        8,
        1,
        2*service_count,
        1,
        blocklen,
        block_count*16
    };


    uint8_t status=inDataExchange(felica->tg,sendlist,sendlenlist,7,pn532_packetbuffer,&length);
    if(status&0b111111!=0){
        DMSG("Error");
        return status;
    }

    uint16_t statusflag=pn532_packetbuffer[10];
    statusflag=(statusflag<<8)+pn532_packetbuffer[11];
    return statusflag;
}

uint16_t PN532::PICC::Felica::readWithoutEncryption(const uint8_t service_count,const uint16_t *servicecode_list,const uint8_t block_count,const uint8_t *block_list,uint8_t response[][16],uint8_t *response_count){
    return this->pcd->felica_readWithoutEncryption(this,service_count,servicecode_list,block_count,block_list,response,response_count);
}