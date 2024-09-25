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

    delay(1000);
    
    nfc.SAMConfig();
}
void loop()
{
    nfc.writeRegister(REG_CIU_RxMode,0);
    nfc.writeRegister(REG_CIU_TxMode,0);
    nfc.setRFfield(false,false);

    uint8_t prev;
    nfc.readRegister(REG_CIU_Status2,&prev);
    prev = bitClear(prev, 3); // disable CRYPTO1
    nfc.writeRegister(REG_CIU_Status2, prev);

    nfc.readRegister(REG_CIU_ManualRCV,&prev);
    prev = bitClear(prev, 4); // disable ParityDisable
    nfc.writeRegister(REG_CIU_ManualRCV, prev);

    nfc.readRegister(REG_CIU_TxAuto,&prev);
    prev = bitSet(prev, 2); // enable InitialRFOn
    nfc.writeRegister(REG_CIU_TxAuto, prev);

    nfc.setParameters(0b00100100); // enable fAutomaticRATS
    //*/


    uint8_t buf[128];
    uint16_t size;
    Serial.println("tgInit");
    uint8_t mode=nfc.tgInitAsTarget(0b101,
    (const uint8_t[]){0x04,0x00,0x12,0x34,0x56,0x40},
    (const uint8_t[]){0x01,0x12,0x03,0x12,0xEC,0x1F,0x88,0x00,0x05,0x31,0x43,0x45,0x46,0x82,0xB7,0xFF,0x00,0x03},
    (const uint8_t[]){0x01,0x12,0x03,0x12,0xEC,0x1F,0x88,0x00,0x00,0x00},
    NULL,0,
    NULL,0,
    buf,&size
    );
    Serial.printf("Mode%02X\n",mode);
    Serial.printf("Initiator:");
    for(int i=0;i<size;i++){
        Serial.printf("%1X ",buf[i]);
    }
    Serial.printf("\n");
    uint16_t status=nfc.tgGetData(buf,&size);
    Serial.printf("Status:%04X\n",status);
    Serial.print("-->");
    for(int i=0;i<size;i++){
        Serial.print(buf[i],HEX);
        Serial.print(" ");
    }
    Serial.println("");

    nfc.tgSetData((const uint8_t[]){0x90,0x00},2U);
    nfc.tgGetData(buf,&size);
    nfc.tgSetData((const uint8_t[]){0x90,0x00},2U);
    //*/
    delay(2000);
}