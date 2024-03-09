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

uint8_t PN532::InListPassiveTarget(const uint8_t maxtg,const PN532::Type type,const uint8_t *data,const uint8_t datalen,uint8_t *targetdata,uint8_t *targetdatalen){
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
        memcpy(targetdata,&pn532_packetbuffer[2],*targetdatalen);
    }
    return count;
}

uint8_t PN532::InAutoPoll(const uint8_t pollNr,const uint8_t period,const PN532::Type *types,const uint8_t typeslen,uint8_t *foundtypes,uint8_t *foundlen){
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
        memcpy(foundtypes,&pn532_packetbuffer[2],*foundlen);
    }

    return count;
}