#ifndef __PN532_INTERFACE_H__
#define __PN532_INTERFACE_H__
#define PN532_I2C_ADDRESS (0x48 >> 1)

#include <stdint.h>

#define PN532_PREAMBLE              (0x00)
#define PN532_STARTCODE1            (0x00)
#define PN532_STARTCODE2            (0xFF)
#define PN532_POSTAMBLE             (0x00)

#define PN532_HOSTTOPN532           (0xD4)
#define PN532_PN532TOHOST           (0xD5)

#define PN532_ACK_WAIT_TIME         (10)  // ms, timeout of waiting for ACK

#define PN532_INVALID_ACK           (-1)
#define PN532_TIMEOUT               (-2)
#define PN532_INVALID_FRAME         (-3)
#define PN532_NO_SPACE              (-4)
#define PN532_EXCESS_DATA_MAX_LEN   (-5)

#define REVERSE_BITS_ORDER(b)       b = (b & 0xF0) >> 4 | (b & 0x0F) << 4; \
                                    b = (b & 0xCC) >> 2 | (b & 0x33) << 2; \
                                    b = (b & 0xAA) >> 1 | (b & 0x55) << 1

class PN532Interface
{
public:
    virtual void begin() = 0;
    virtual void wakeup() = 0;
    /**
    * @brief    write a command and check ack
    * @param    header  packet header
    * @param    hlen    length of header
    * @param    body    packet body
    * @param    blen    length of body
    * @return   0       success
    *           not 0   failed
    */
    virtual int8_t writeCommand(const uint8_t *header, uint8_t hlen,const uint8_t *body,const uint16_t blen) = 0;
    /**
    * @brief    write a command and check ack
    * @param    header  packet header
    * @param    hlen    length of header
    * @param    body    packet body
    * @param    blen    length of body
    * @param    body2    packet body2
    * @param    b2len    length of body2
    * @return   0       success
    *           not 0   failed
    */
    virtual int8_t writeCommand(const uint8_t *header, uint8_t hlen,const uint8_t *body,const uint16_t blen,const uint8_t *body2,const uint16_t b2len) = 0;
    /**
    * @brief    write a command and check ack
    * @param    header  packet header
    * @param    hlen    length of header
    * @param    body    packet body
    * @param    blen    length of body
    * @param    body2    packet body2
    * @param    b2len    length of body2
    * @param    body3    packet body3
    * @param    b3len    length of body3
    * @return   0       success
    *           not 0   failed
    */
    virtual int8_t writeCommand(const uint8_t *header, uint8_t hlen,const uint8_t *body,const uint16_t blen,const uint8_t *body2,const uint16_t b2len,const uint8_t *body3,const uint16_t b3len) = 0;
    /**
    * @brief    write a command and check ack
    * @param    header  packet header
    * @param    hlen    length of header
    * @param    body    packet body
    * @param    blen    length of body
    * @param    body2    packet body2
    * @param    b2len    length of body2
    * @param    body3    packet body3
    * @param    b3len    length of body3
    * @param    body4    packet body4
    * @param    b4len    length of body4
    * @return   0       success
    *           not 0   failed
    */
    virtual int8_t writeCommand(const uint8_t *header, uint8_t hlen,const uint8_t *body,const uint16_t blen,const uint8_t *body2,const uint16_t b2len,const uint8_t *body3,const uint16_t b3len,const uint8_t *body4,const uint16_t b4len) = 0;

    /**
    * @brief    read the response of a command, strip prefix and suffix
    * @param    buf     to contain the response data
    * @param    len     lenght to read
    * @param    timeout max time to wait, 0 means no timeout
    * @return   0       success
    *           not 0   failed
    */
    virtual int8_t readResponse(uint8_t buf[], uint8_t *len, uint16_t timeout = 1000) = 0;
    /**
    * @brief    read the response of a command, strip prefix and suffix
    * @param    buf     to contain the response data
    * @param    len     lenght to read
    * @param    timeout max time to wait, 0 means no timeout
    * @return   0       success
    *           not 0   failed
    */
    virtual int8_t readResponse(uint8_t buf[], uint16_t *len, uint16_t timeout = 1000) = 0;
};

#endif

