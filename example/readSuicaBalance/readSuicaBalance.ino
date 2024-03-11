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
    nfc.SAMConfig();
}
void loop()
{
    uint8_t buf[1024];
    uint8_t size;
    PN532::PICC::Felica felica;
    int t=nfc.PollingFelica(1,0x03,0x01,0,&felica);
    if(t==0){
        return;
    }
    Serial.print("IDm:");
    for(int i=0;i<8;i++){
        Serial.print(felica.idm[i],HEX);
        Serial.print(" ");
    }
    Serial.print("\nPMm:");
    for(int i=0;i<8;i++){
        Serial.print(felica.pmm[i],HEX);
        Serial.print(" ");
    }
    Serial.print("\ncode:");
    Serial.println(felica.systemcode,HEX);
    Serial.print("Mode:");
    Serial.println(nfc.felica_requestResponse(&felica),HEX);

    uint16_t scode[]={0x090f};
    uint8_t blockData[3][16];
    uint8_t blockList[4]={0x80,0x00,0x80,0x02};
    uint8_t r;
    memset(blockData, 0, 16*3);
    uint16_t status=felica.readWithoutEncryption(1,scode,2,blockList,blockData,&r);
    Serial.print("Status :");
    Serial.println(status,HEX);
    if(status!=0x00){
        return;
    }
    uint16_t balance=blockData[0][11];
    balance=(balance<<8)+blockData[0][10];
    for(int i=0;i<r;i++){
        Serial.print("Block ");
        Serial.println(i);
        for(int j=0;j<16;j++){
            Serial.print(blockData[i][j],HEX);
            Serial.print(" ");
        }
        Serial.println("");
    }
    Serial.print("Balance:");
    Serial.println(balance);
    delay(1000);
}