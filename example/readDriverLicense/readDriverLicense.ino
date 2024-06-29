#include <PN532_debug.h>
#include <Arduino.h>
#include <Wire.h>
#include<PN532.h>
#include <PN532_I2C.h>
PN532_I2C pn532i2c(Wire);
PN532 nfc(pn532i2c);


char pinA[]="1234";
char pinB[]="1234";

// 運転できる車の種類
char cartype[18][25]={
    "二輪,小型,原付",
    "その他",
    "第二種",
    "大型",
    "普通",
    "大型特殊",
    "大型自動二輪",
    "普通自動二輪",
    "小型特殊",
    "原付",
    "けん引",
    "大型第二種",
    "普通第二種",
    "大型特殊第二種",
    "けん引第二種",
    "中型",
    "中型第二種",
    "準中型"
};
// 元号
char gengo[5][10]={
    "明治","大正","昭和","平成","令和"
};
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
    Serial.print("PUPI:");
    for(int i=0;i<4;i++){
        Serial.print(typeb.pupi[i],HEX);
        Serial.print(" ");
    }

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
    uint8_t buf[1024];
    uint16_t len;
    uint16_t status;


    status=picc->selectMF(0);
    if(status!=0x9000){
        Serial.printf("SelectMF Failed %04X\n",status);
        return false;
    }
    status=picc->selectEF((const uint8_t[]){0x2F,0x01},2);
    if(status!=0x9000){
        Serial.printf("SelectEF Failed %04X\n",status);
        return false;
    }

    status=picc->readBinary(0x00,0x05,0x08,buf,&len);
    if(status!=0x9000){
        Serial.printf("ReadBin Failed %04X\n",status);
        return false;
    }

    uint16_t syear=(buf[0]>>4)*1000+(buf[0]&0x0F)*100+(buf[1]>>4)*10+(buf[1]&0x0F);
    uint8_t smonth=(buf[2]>>4)*10+(buf[2]&0x0F);
    uint8_t sday=(buf[3]>>4)*10+(buf[3]&0x0F);
    uint16_t eyear=(buf[4]>>4)*1000+(buf[4]&0x0F)*100+(buf[5]>>4)*10+(buf[5]&0x0F);
    uint8_t emonth=(buf[6]>>4)*10+(buf[6]&0x0F);
    uint8_t eday=(buf[7]>>4)*10+(buf[7]&0x0F);
    Serial.printf("交付日  :%u年%u月%u日\n",syear,smonth,sday);
    Serial.printf("有効期限:%u年%u月%u日\n",eyear,emonth,eday);

    
    status=picc->selectEF((const uint8_t[]){0x00,0x01},2);
    if(status!=0x9000){
        Serial.printf("SelectEF Failed %04X\n",status);
        return false;
    }
    status=picc->verify(0x80,(uint8_t*)pinA,4);
    if(status!=0x9000){
        Serial.printf("Verify Failed %04X\n",status);
        return false;
    }


    status=picc->selectDF((const uint8_t[]){0xA0,0x00,0x00,0x02,0x31,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},16);
    if(status!=0x9000){
        Serial.printf("SelectDF Failed %04X\n",status);
        return false;
    }
    status=picc->selectEF((const uint8_t[]){0x00,0x01},2);
    if(status!=0x9000){
        Serial.printf("SelectEF Failed %04X\n",status);
        return false;
    }
    status=picc->readBinary(0,0,0,buf,&len);
    if(status!=0x9000){
        Serial.printf("Read Failed %04X\n",status);
        return false;
    }
    uint16_t i=0;
    while(i<len){
        uint8_t tag=buf[i];
        uint8_t size=buf[i+1];
        i+=2;
        if(tag==0x21){
            Serial.printf("運転免許証番号:");
            for(uint16_t j=0;j<size;j++){
                Serial.printf("%c",buf[i+j]);
            }
            Serial.printf("\n");
        }
        else if(0x22<=tag&&tag<=0x33){
            uint8_t cartag=tag-0x22;
            uint8_t g=buf[i]-0x31;
            uint8_t y=(buf[i+1]-0x30)*10+(buf[i+2]-0x30);
            uint8_t m=(buf[i+3]-0x30)*10+(buf[i+4]-0x30);
            uint8_t d=(buf[i+5]-0x30)*10+(buf[i+6]-0x30);
            if(!(y==m&&m==d&&d==00)){
                Serial.printf("%s :%s%02d年%02d月%02d日\n",cartype[cartag],gengo[g],y,m,d);
            }
        }
        i+=size;
    }


    return true;
}
