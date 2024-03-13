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
    PN532::PICC::TypeB typeb;
    PN532::PICC::TypeA typea;
    PN532::PICC::Felica felica[2];
    //int t=nfc.PollingTypeB(1,0x00,&typeb);
    int t=nfc.PollingTypeA(2,NULL,0,&typea);
    //int t=nfc.PollingFelica(2,0x03,0x01,0,felica);
    if(t==0){
        return;
    }
    Serial.print("UID:");
    for(int i=0;i<typea.uidlen;i++){
        Serial.print(typea.uid[i],HEX);
        Serial.print(" ");
    }
    Serial.print("\n");
    uint8_t type=0x00;
    if(nfc.selectFile(0x01,0x0400,(uint8_t*)"1PAY.SYS.DDF01",14,buf,&size)!=APDU_STATUS_SUCCESS){
        if(nfc.selectFile(0x01,0x0400,(uint8_t*)"2PAY.SYS.DDF01",14,buf,&size)!=APDU_STATUS_SUCCESS){
            Serial.println("This card is not supported.");
        }
        else{
            type=2;
        }
    }
    else{
        type=1;
    }
    uint8_t aid[10];
    uint8_t aidlen;
    for(int i=0;i<size;i++){
        if(buf[i]==0x4F){
            aidlen=buf[i+1];
            for(int j=0;j<aidlen;j++){
                aid[j]=buf[j+i+2];
            }
            break;
        }
    }
    for(int i=0;i<aidlen;i++){
        Serial.print(aid[i],HEX);
        Serial.print(" ");
    }
    Serial.print("\n");
    Serial.print("Status:");
    Serial.println(nfc.selectFile(0x01,0x0400,aid,aidlen,buf,&size),HEX);
    for(int i=0;i<size;i++){
        Serial.print(buf[i],HEX);
        Serial.print(" ");
    }
    Serial.print("\n");

    uint8_t gpo[128]={0x83,0x21,0x28,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,
    0x03,0x29,
    0x00,0x00,0x00,0x00,0x00,
    0x03,0x29,
    0x24,0x03,0x11,
    0x00,
    0x00,0x00,0x00,0x01};
    uint8_t head[5]={
        0x80,0xA8,0x00,0x00
    };
    head[4]=23;

    const uint8_t *sendlist[]={
        head,
        gpo
    };
    const uint16_t sendlistlen[]={
        5,
        10
    };
    Serial.print("Status:");
    Serial.println(nfc.inDataExchange(0x01,sendlist,sendlistlen,2,buf,&size),HEX);
    for(int i=0;i<size;i++){
        Serial.print(buf[i],HEX);
        Serial.print(" ");
    }
    Serial.print("\n");


    Serial.print("\n");
        delay(1000);
}