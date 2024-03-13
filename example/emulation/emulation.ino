#include <PN532_debug.h>
#include <Arduino.h>
#include <Wire.h>
#include<PN532.h>
#include <PN532_I2C.h>
PN532_I2C pn532i2c(Wire);
PN532 nfc(pn532i2c);


void setup()
{    
    Serial.begin(115200);

    pn532i2c.begin(2,1);
    delay(100);
    uint32_t versiondata=nfc.getFirmwareVersion();
    if(!versiondata){
        Serial.print("Didn't find PN53x board");
        while (1); // halt
    }
    Serial.print("Found chip PN5");
    Serial.println((versiondata>>24) & 0xFF, HEX); 
    Serial.print("Firmware ver. ");
    Serial.print((versiondata>>16) & 0xFF, DEC); 
    Serial.print('.'); Serial.println((versiondata>>8) & 0xFF, DEC);

    nfc.SAMConfig();
    //nfc.setParameters(0x4);

}
void loop()
{
    Serial.println("tgInit");
    Serial.println(nfc.tgInitAsTarget(0,
    (const uint8_t[]){0x00,0x08,0x12,0x34,0x56,0x60},
    (const uint8_t[]){0x01,0xFE,0xA2,0xA3,0xA4,0xA5,0xA6,0xA7,0xC0,0xC1,0xC2,0xC3,0xC4,0xC5,0xC6,0xC7,0xFF,0xFF},
    (const uint8_t[]){0xAA,0x99,0x88,0x77,0x66,0x55,0x44,0x33,0x22,0x11},
    NULL,0,NULL,0
    ));
    uint8_t buf[128];
    uint16_t size;
    nfc.tgGetData(buf,&size);
    nfc.tgSetData((const uint8_t[]){0x90,0x00},2U);
    nfc.tgGetData(buf,&size);
    nfc.tgSetData((const uint8_t[]){0x90,0x00},2U);
    delay(2000);
}