#include "PN532_I2C.h"
#include "PN532_debug.h"
#include "Arduino.h"

PN532_I2C::PN532_I2C(TwoWire &wire)
{
    _wire = &wire;
    command = 0;
}

void PN532_I2C::begin()
{
    _wire->setBufferSize(PN532_I2C_BUF_SIZE);
    _wire->begin();
}
void PN532_I2C::begin(int sda,int scl)
{
    _wire->setBufferSize(PN532_I2C_BUF_SIZE);
    _wire->setPins(sda,scl);
    _wire->begin();
}

void PN532_I2C::wakeup()
{
    delay(500); // wait for all ready to manipulate pn532
}

int8_t PN532_I2C::writeCommand(const uint8_t *header, uint8_t hlen, const uint8_t *body, uint16_t blen)
{
    command = header[0];
    _wire->beginTransmission(PN532_I2C_ADDRESS);

    write(PN532_PREAMBLE);
    write(PN532_STARTCODE1);
    write(PN532_STARTCODE2);

    uint16_t length = hlen + blen + 1; // length of data field: TFI + DATA
    if(0xFF<=length){
        if(265<=length){ // In the firmware implementation of the PN532, the maximum length of the packet data is limited to 264 bytes (265 bytes with TFI included).
            return PN532_EXCESS_DATA_MAX_LEN;
        }
        // The normal LEN and LCS fields are fixed to the 0xFF value, which is normally considered as erroneous frame, due to the fact that the checksum does not fit. 
        write(0xFF);
        write(0xFF);
    }
    write(length);
    write(~length + 1); // checksum of length

    write(PN532_HOSTTOPN532); // TFI
    uint8_t sum = PN532_HOSTTOPN532; // sum of TFI + DATA

    DMSG("write: ");

    for (uint8_t i = 0; i < hlen; i++)
    {
        if (write(header[i]))
        {
            sum += header[i];

            DMSG_HEX(header[i]);
        }
        else
        {
            DMSG("\nToo many data to send, I2C doesn't support such a big packet\n"); // I2C max packet: 32 bytes
            return PN532_INVALID_FRAME;
        }
    }

    for (uint16_t i = 0; i < blen; i++)
    {
        if (write(body[i]))
        {
            sum += body[i];

            DMSG_HEX(body[i]);
        }
        else
        {
            DMSG("\nToo many data to send, I2C doesn't support such a big packet\n"); // I2C max packet: 32 bytes
            return PN532_INVALID_FRAME;
        }
    }

    uint8_t checksum = ~sum + 1; // checksum of TFI + DATA
    write(checksum);
    write(PN532_POSTAMBLE);

    _wire->endTransmission();

    DMSG('\n');

    return readAckFrame();
}
int8_t PN532_I2C::writeCommand(const uint8_t *header, uint8_t hlen, const uint8_t *body, uint8_t blen){
    return writeCommand(header,hlen,body,(uint16_t)blen);
}


int32_t PN532_I2C::getResponseLength(uint16_t len, uint16_t timeout){
    uint16_t time = 0;
    uint16_t length;

    // [RDY] 00 00 FF LEN LCS (TFI PD0 ... PDn) DCS 00
    do{
        if (_wire->requestFrom(PN532_I2C_ADDRESS, len)){
            if (read() & 1){ // check first byte --- status
                break; // PN532 is ready
            }
        }

        delay(1);
        time++;
        if ((0 != timeout) && (time > timeout)){
            return PN532_TIMEOUT;
        }
    } while (1);

    if (PN532_PREAMBLE != read() || // PREAMBLE
        PN532_STARTCODE1 != read() || // STARTCODE1
        PN532_STARTCODE2 != read()    // STARTCODE2
    )
    {
        return PN532_INVALID_FRAME;
    }

    length = read();
    if((length&0xFFU)==0xFFU){
        if(0xFFU!=read()){
            return PN532_INVALID_FRAME;
        }
        length=read();  // LENM
        length=read()+(length<<8);// LENL
    }

    // Checksum of length
    if(0xFF<=length){
        if((length>>8+length+read())&0xFF!=0){
            DMSG("LCS is not ok\n");
            return PN532_INVALID_FRAME;
        }
    }
    else{
        if((length+read())&0xFF!=0){
            DMSG("LCS is not ok\n");
            return PN532_INVALID_FRAME;
        }
    }

    length--;

    return (int32_t)length;
}
int8_t PN532_I2C::readResponse(uint8_t buf[], uint16_t *len, uint16_t timeout){
    int32_t res=getResponseLength(8,timeout);
    if(res<0){
        return res;
    }
    uint16_t length=res&(0xFFFF);
    DMSG("receve len: ");
    DMSG(length);
    DMSG("\n");
    if (*len<length){
        DMSG("Response is too big\n");
        return PN532_NO_SPACE; // not enough space
    }

    sendNack();

    if(0xFF<=length){
        res=getResponseLength(11+length,timeout);
    }
    else{
        res=getResponseLength(8+length,timeout);
    }
    if(res<0){
        return res;
    }

    if(read()!=PN532_PN532TOHOST){
        return PN532_INVALID_FRAME;
    }

    *len=length;

    DMSG("read:  ");

    uint8_t sum = PN532_PN532TOHOST;
    for (uint8_t i = 0; i < length; i++){
        buf[i] = read();
        sum += buf[i];

        DMSG_HEX(buf[i]);
    }
    DMSG('\n');

    uint8_t checksum = read();
    if (0 != (uint8_t)(sum + checksum)){
        DMSG("checksum is not ok\n");
        return PN532_INVALID_FRAME;
    }
    read(); // POSTAMBLE

    return 0;
}
int8_t PN532_I2C::readResponse(uint8_t buf[], uint8_t *len, uint16_t timeout){
    uint16_t tmp=*len;
    int8_t r=readResponse(buf,&tmp,timeout);
    *len=tmp;
    return r;
}

int8_t PN532_I2C::readAckFrame(){
    const uint8_t PN532_ACK[] = {0, 0, 0xFF, 0, 0xFF, 0};
    uint8_t ackBuf[sizeof(PN532_ACK)];

    DMSG("wait for ack at : ");
    DMSG(millis());
    DMSG('\n');

    uint16_t time = 0;
    do{
        if (_wire->requestFrom(PN532_I2C_ADDRESS, sizeof(PN532_ACK) + 1))
        {
            if (read() & 1)
            {          // check first byte --- status
                break; // PN532 is ready
            }
        }

        delay(1);
        time++;
        if (time > PN532_ACK_WAIT_TIME)
        {
            DMSG("Time out when waiting for ACK\n");
            return PN532_TIMEOUT;
        }
    } while (1);

    DMSG("ready at : ");
    DMSG(millis());
    DMSG('\n');

    for (uint8_t i = 0; i < sizeof(PN532_ACK); i++)
    {
        ackBuf[i] = read();
    }

    if (memcmp(ackBuf, PN532_ACK, sizeof(PN532_ACK)))
    {
        DMSG("Invalid ACK\n");
        return PN532_INVALID_ACK;
    }

    return 0;
}

void PN532_I2C::sendNack(){
    const uint8_t PN532_NACK[] = {0, 0, 0xFF, 0xFF, 0, 0};
    _wire->beginTransmission(PN532_I2C_ADDRESS);
    for (uint16_t i = 0; i < sizeof(PN532_NACK); ++i)
    {
        write(PN532_NACK[i]);
    }
    _wire->endTransmission();
}
