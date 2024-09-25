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
    PN532::PICC::TypeA typea;
    int t=nfc.PollingTypeA(2,NULL,0,&typea);
    if(t==0){
        return;
    }
    Serial.print("UID:");
    for(int i=0;i<typea.uidlen;i++){
        Serial.printf("%02X ",typea.uid[i]);
    }
    Serial.print("\n");

    uint8_t fci[256];
    uint16_t fcilen;
    uint8_t type=0x00;
    if(typea.selectFile(0x0400,(uint8_t*)"1PAY.SYS.DDF01",14,fci,&fcilen)!=APDU_STATUS_SUCCESS){
        if(typea.selectFile(0x0400,(uint8_t*)"2PAY.SYS.DDF01",14,fci,&fcilen)!=APDU_STATUS_SUCCESS){
            Serial.println("This card is not supported.");
            return;
        }
        else{
            type=2;
        }
    }
    else{
        type=1;
    }

    Serial.printf("Type:%d\n",type);
    for(int i=0;i<fcilen;i++){
        Serial.printf("%02X ",fci[i]);
    }
    Serial.print("\n");

    if(type==2){
        uint8_t *dfname;
        uint8_t dflen;
        for(int i=0;i<fcilen;i++){
            if(fci[i]==0x84){
                dfname=&fci[i+2];
                dflen=fci[i+1];
                break;
            }
        }


        uint8_t *aid;
        uint8_t aidlen;
        for(int i=0;i<fcilen;i++){
            if(fci[i]==0x4f){
                aid=&fci[i+2];
                aidlen=fci[i+1];
                break;
            }
        }
        if(typea.selectFile(0x0400,aid,aidlen,buf,&size)!=APDU_STATUS_SUCCESS){
            return;
        }
for(int i=0;i<size;i++){
        Serial.printf("%02X ",buf[i]);
    }
    Serial.print("\n");
    }

    Serial.printf("\n");

    delay(1000);
}