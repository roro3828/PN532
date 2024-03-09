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
        index=pn532_packetbuffer[index+13]+13+index;
    }
    return count;
}



uint8_t PN532::inDataExchange(const uint8_t tg,const uint8_t *send,const uint8_t sendlen,uint8_t *response,uint8_t *responselen){
    pn532_packetbuffer[0]=PN532_COMMAND_INDATAEXCHANGE;
    pn532_packetbuffer[1]=tg;
    *responselen=0;

    if(_interface->writeCommand(pn532_packetbuffer,2,send,sendlen)){
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


bool PN532::APDU::select(const uint8_t *id,const uint8_t idlen,const uint8_t p1,const uint8_t p2,uint8_t *status){
    
}