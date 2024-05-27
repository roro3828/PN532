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
    uint8_t size;
    PN532::PICC::TypeB typeb;
    int t=nfc.PollingTypeB(1,0x00,&typeb);
    if(t==0){
        return;
    }
    Serial.print("AP:");
    for(int i=0;i<4;i++){
        Serial.print(typeb.ap[i],HEX);
        Serial.print(" ");
    }
    char pin[]="1234";

    Serial.print("\nType B found\n");
    //if(!readMynumber((uint8_t*)pin)){
    if(!readDriver(&typeb)){
        Serial.print("\nReadFaild\n");
        delay(100);
    }
    else{
        delay(1000);
    }
}
bool readDriver(PN532::PICC::TypeB *picc){
    uint8_t buf[128];
    uint16_t len;
    uint16_t status;


    status=picc->selectMF(0,buf,&len);
    Serial.printf("SelectMF %04X\n",status);
    if(status!=0x9000){
        return false;
    }

    status=picc->selectEF((const uint8_t[]){0x2F,0x01},2);
    Serial.printf("SelectEF %04X\n",status);
    if(status!=0x9000){
        return false;
    }

    status=picc->readBinary(0x00,0x05,0x08,buf,&len);
    Serial.printf("ReadBin %04X\n",status);
    if(status!=0x9000){
        return false;
    }
    for(int i=0;i<len;i++){
        Serial.printf("%02X ",buf[i]);
    }
    Serial.printf("\n");

    uint16_t syear=(buf[0]>>4)*1000+(buf[0]&0x0F)*100+(buf[1]>>4)*10+(buf[1]&0x0F);
    uint8_t smonth=(buf[2]>>4)*10+(buf[2]&0x0F);
    uint8_t sday=(buf[3]>>4)*10+(buf[3]&0x0F);
    uint16_t eyear=(buf[4]>>4)*1000+(buf[4]&0x0F)*100+(buf[5]>>4)*10+(buf[5]&0x0F);
    uint8_t emonth=(buf[6]>>4)*10+(buf[6]&0x0F);
    uint8_t eday=(buf[7]>>4)*10+(buf[7]&0x0F);
    Serial.printf("交付日\t:%u %u %u\n",syear,smonth,sday);
    Serial.printf("有効期限\t:%u %u %u\n",eyear,emonth,eday);

    return true;
}
