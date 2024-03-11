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
    PN532::PICC::TypeA typea[2];
    PN532::PICC::Felica felica[2];
    int t=nfc.PollingTypeB(1,0x00,&typeb);
    //int t=nfc.PollingTypeA(2,NULL,0,typea);
    //int t=nfc.PollingFelica(2,0x03,0x01,0,felica);
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
    if(!pinStatus()){
        Serial.print("\nReadFaild\n");
        delay(100);
    }
    else{
        delay(1000);
    }
}
int8_t lookupPIN(){
    uint16_t status=nfc.verify(0x01,0x80,NULL,0);
    if(((status>>8)&0xFF)==0x63U){
        return (int8_t)(status&0x0F);
    }
    else{
        return -1;
    }
}
bool pinStatus(){
    Serial.print("Reading...\n");
    // SELECT 入力補助AP
    if(nfc.selectDF(0x01,(const uint8_t[]){0xD3,0x92,0x10,0x00,0x31,0x00,0x01,0x01,0x04,0x08},10)!=APDU_STATUS_SUCCESS){
        return false;
    }
    if(nfc.selectEF(0x01,(const uint8_t[]){0x00,0x11},2)!=APDU_STATUS_SUCCESS){
        return false;
    }
    int8_t N=lookupPIN();
    if(nfc.selectEF(0x01,(const uint8_t[]){0x00,0x14},2)!=APDU_STATUS_SUCCESS){
        return false;
    }
    int8_t NA=lookupPIN();
    if(nfc.selectEF(0x01,(const uint8_t[]){0x00,0x15},2)!=APDU_STATUS_SUCCESS){
        return false;
    }
    int8_t NB=lookupPIN();

    // SELECT 券面事項AP
    if(nfc.selectDF(0x01,(const uint8_t[]){0xD3,0x92,0x10,0x00,0x31,0x00,0x01,0x01,0x04,0x02},10)!=APDU_STATUS_SUCCESS){
        return false;
    }
    if(nfc.selectEF(0x01,(const uint8_t[]){0x00,0x13},2)!=APDU_STATUS_SUCCESS){
        return false;
    }
    int8_t KA=lookupPIN();
    if(nfc.selectEF(0x01,(const uint8_t[]){0x00,0x12},2)!=APDU_STATUS_SUCCESS){
        return false;
    }
    int8_t KB=lookupPIN();

    // SELECT JPKIAP
    if(nfc.selectDF(0x01,(const uint8_t[]){0xD3,0x92,0xF0,0x00,0x26,0x01,0x00,0x00,0x00,0x01},10)!=APDU_STATUS_SUCCESS){
        return false;
    }
    if(nfc.selectEF(0x01,(const uint8_t[]){0x00,0x18},2)!=APDU_STATUS_SUCCESS){
        return false;
    }
    int8_t JPKIA=lookupPIN();
    if(nfc.selectEF(0x01,(const uint8_t[]){0x00,0x1B},2)!=APDU_STATUS_SUCCESS){
        return false;
    }
    int8_t JPKIS=lookupPIN();

    //*/
    Serial.printf("N    :%2d\nNA   :%2d\nNB   :%2d\nKA   :%2d\nKB   :%2d\nJPKIA:%2d\nJPKIS:%2d\n",
        N,
        NA,
        NB,
        KA,
        KB,
        JPKIA,
        JPKIS
    );

    return true;
}
bool readMynumber(uint8_t *pin){
    uint8_t buf[512];
    uint16_t size;

    // SELECT 補助AP
    if(nfc.selectDF(0x01,(const uint8_t[]){0xD3,0x92,0x10,0x00,0x31,0x00,0x01,0x01,0x04,0x08},10)!=APDU_STATUS_SUCCESS){
        return false;
    }
    // SELECT 暗証番号
    if(nfc.selectEF(0x01,(const uint8_t[]){0x00,0x11},2)!=APDU_STATUS_SUCCESS){
        return false;
    }
    // VERIFY
    if(nfc.verify(0x01,APDU_CMD_VERIFY_SPECIFIC_REFERENCE,pin,4)!=APDU_STATUS_SUCCESS){
        Serial.println("Verify failed");
        Serial.print("Left count:");
        Serial.println(lookupPIN());
        return false;
    }
    // SELECT マイナンバー
    if(nfc.selectEF(0x01,(const uint8_t[]){0x00,0x01},2)!=APDU_STATUS_SUCCESS){
        return false;
    }
    // READ
    if(nfc.readBinary(0x01,0,0,0,buf,&size)!=APDU_STATUS_SUCCESS){
        return false;
    }
    buf[15]='\0';
    Serial.printf("%s\n",&buf[3]);
    return true;
}