#ifndef __PN532_I2C_H__
#define __PN532_I2C_H__

#include <Wire.h>
#include "PN532Interface.h"

#define PN532_I2C_BUF_SIZE 2048

class PN532_I2C : public PN532Interface
{
public:
    PN532_I2C(TwoWire &wire);

    void begin();
    void begin(int sda,int scl);
    void wakeup();
    int8_t writeCommand(const uint8_t *header, uint8_t hlen,const uint8_t *body,const uint16_t blen);
    int8_t writeCommand(const uint8_t *header, uint8_t hlen,const uint8_t *body,const uint16_t blen,const uint8_t *body2,const uint16_t b2len);
    int8_t writeCommand(const uint8_t *header, uint8_t hlen,const uint8_t *body,const uint16_t blen,const uint8_t *body2,const uint16_t b2len,const uint8_t *body3,const uint16_t b3len);
    int8_t writeCommand(const uint8_t *header, uint8_t hlen,const uint8_t *body,const uint16_t blen,const uint8_t *body2,const uint16_t b2len,const uint8_t *body3,const uint16_t b3len,const uint8_t *body4,const uint16_t b4len);
    int8_t readResponse(uint8_t buf[], uint8_t *len, uint16_t timeout);
    int8_t readResponse(uint8_t buf[], uint16_t *len, uint16_t timeout);

private:
    TwoWire *_wire;
    uint8_t command;

    int32_t getResponseLength(uint16_t len, uint16_t timeout);
    int8_t readAckFrame();
    void sendNack();

    inline uint8_t write(uint8_t data)
    {
        return _wire->write(data);
    }

    inline uint8_t read()
    {
        return _wire->read();
    }
};

#endif
