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
    int t=nfc.PollingTypeA(2,NULL,0,&typea);
    if(t==0){
        return;
    }
    Serial.print("UID:");
    for(int i=0;i<typea.uidlen;i++){
        Serial.printf("%02X ",typea.uid[i]);
    }
    Serial.print("\n");
    uint8_t type=0x00;
    if(nfc.selectFile(0x01,0x0400,(uint8_t*)"1PAY.SYS.DDF01",14,buf,&size)!=APDU_STATUS_SUCCESS){
        if(nfc.selectFile(0x01,0x0400,(uint8_t*)"2PAY.SYS.DDF01",14,buf,&size)!=APDU_STATUS_SUCCESS){
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

    if(type==2){
        uint8_t *aid;
        uint8_t aidlen;
        for(int i=0;i<size;i++){
            if(buf[i]==0x4f){
                aid=&buf[i+2];
                aidlen=buf[i+1];
                break;
            }
        }
        if(nfc.selectFile(typea.tg,0x0400,aid,aidlen,buf,&size)!=0x9000){
            return;
        }

        uint8_t gpo[]={
            0x80,0xA8,
            0x00,0x00,
            0x23,
            0x83,
            0x21,
            0x28,0x00,0x00,0x00,
            0x00,0x00,0x00,0x00,0x10,0x00,
            0x00,0x00,0x00,0x00,0x00,0x00,
            0x03,0x92,
            0x00,0x00,0x00,0x00,0x00,
            0x03,0x92,
            0x24,0x03,0x20,
            0x00,
            0x38,0x39,0x30,0x31,
            0x00};
        if(nfc.inDataExchange(typea.tg,gpo,41,buf,&size)!=0x0000){
            return;
        }
        uint8_t cardnumber[16];
        uint8_t yy;
        uint8_t mm;
        for(int i=0;i<size;i++){
            if(buf[i]==0x57){
                for(int j=0;j<8;j++){
                    cardnumber[j*2]=(buf[i+2+j]>>4)&0x0F;
                    cardnumber[j*2+1]=(buf[i+2+j]&0x0F);
                }

                yy=((buf[i+10]&0x0F)*10)+((buf[i+11]>>4)&0x0F);
                mm=((buf[i+11]&0x0F)*10)+((buf[i+12]>>4)&0x0F);

                break;
            }
        }

        for(int i=0;i<16;i++){
            Serial.printf("%hhu",cardnumber[i]);
        }
        Serial.printf("\nYY:%02hhu\nMM:%02hhu\n",yy,mm);
    }

    Serial.printf("\n");
    delay(1000);
}