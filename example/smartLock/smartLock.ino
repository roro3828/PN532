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
}
void loop()
{
    uint8_t buf[1024];
    uint16_t size;
    PN532::PICC::TypeA typea;
    int t=nfc.PollingTypeA(1,NULL,0,&typea);
    if(t==0){
        return;
    }
    Serial.print("UID:");
    for(int i=0;i<typea.uidlen;i++){
        Serial.printf("%02X ",typea.uid[i]);
    }
    Serial.print("\n");
    uint8_t type=0x00;

    if(nfc.selectDF(typea.tg,(const uint8_t[]){0xF0,0x49,0x30,0x47,0x69,0x50,0x29,0x47},8)==APDU_STATUS_SUCCESS){
        Serial.printf("Success\n");
    }

    buf[0]=0x23;
    buf[1]=0x6f;
    buf[2]=0xb9;
    buf[3]=0x1c;

    Serial.printf("%02X\n",nfc.SendAPDU(typea.tg,0x66,0x01,0x12,0x34,buf,4,buf,&size));

    for(int i=0;i<size;i++){
        Serial.printf("%02X ",buf[i]);
    }

    Serial.printf("\n");
    delay(1000);
}