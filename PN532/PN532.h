#ifndef __PN532_H__
#define __PN532_H__

#include <stdint.h>
#include "PN532Interface.h"

// PN532 Commands
#define PN532_COMMAND_DIAGNOSE              (0x00)
#define PN532_COMMAND_GETFIRMWAREVERSION    (0x02)
#define PN532_COMMAND_GETGENERALSTATUS      (0x04)
#define PN532_COMMAND_READREGISTER          (0x06)
#define PN532_COMMAND_WRITEREGISTER         (0x08)
#define PN532_COMMAND_READGPIO              (0x0C)
#define PN532_COMMAND_WRITEGPIO             (0x0E)
#define PN532_COMMAND_SETSERIALBAUDRATE     (0x10)
#define PN532_COMMAND_SETPARAMETERS         (0x12)
#define PN532_COMMAND_SAMCONFIGURATION      (0x14)
#define PN532_COMMAND_POWERDOWN             (0x16)
#define PN532_COMMAND_RFCONFIGURATION       (0x32)
#define PN532_COMMAND_RFREGULATIONTEST      (0x58)
#define PN532_COMMAND_INJUMPFORDEP          (0x56)
#define PN532_COMMAND_INJUMPFORPSL          (0x46)
#define PN532_COMMAND_INLISTPASSIVETARGET   (0x4A)
#define PN532_COMMAND_INATR                 (0x50)
#define PN532_COMMAND_INPSL                 (0x4E)
#define PN532_COMMAND_INDATAEXCHANGE        (0x40)
#define PN532_COMMAND_INCOMMUNICATETHRU     (0x42)
#define PN532_COMMAND_INDESELECT            (0x44)
#define PN532_COMMAND_INRELEASE             (0x52)
#define PN532_COMMAND_INSELECT              (0x54)
#define PN532_COMMAND_INAUTOPOLL            (0x60)
#define PN532_COMMAND_TGINITASTARGET        (0x8C)
#define PN532_COMMAND_TGSETGENERALBYTES     (0x92)
#define PN532_COMMAND_TGGETDATA             (0x86)
#define PN532_COMMAND_TGSETDATA             (0x8E)
#define PN532_COMMAND_TGSETMETADATA         (0x94)
#define PN532_COMMAND_TGGETINITIATORCOMMAND (0x88)
#define PN532_COMMAND_TGRESPONSETOINITIATOR (0x90)
#define PN532_COMMAND_TGGETTARGETSTATUS     (0x8A)

#define PN532_RESPONSE_INDATAEXCHANGE       (0x41)
#define PN532_RESPONSE_INLISTPASSIVETARGET  (0x4B)


#define PN532_MIFARE_ISO14443A              (0x00)

// Mifare Commands
#define MIFARE_CMD_AUTH_A                   (0x60)
#define MIFARE_CMD_AUTH_B                   (0x61)
#define MIFARE_CMD_READ                     (0x30)
#define MIFARE_CMD_WRITE                    (0xA0)
#define MIFARE_CMD_WRITE_ULTRALIGHT         (0xA2)
#define MIFARE_CMD_TRANSFER                 (0xB0)
#define MIFARE_CMD_DECREMENT                (0xC0)
#define MIFARE_CMD_INCREMENT                (0xC1)
#define MIFARE_CMD_STORE                    (0xC2)

// FeliCa Commands
#define FELICA_CMD_POLLING                  (0x00)
#define FELICA_CMD_REQUEST_SERVICE          (0x02)
#define FELICA_CMD_REQUEST_RESPONSE         (0x04)
#define FELICA_CMD_READ_WITHOUT_ENCRYPTION  (0x06)
#define FELICA_CMD_WRITE_WITHOUT_ENCRYPTION (0x08)
#define FELICA_CMD_REQUEST_SYSTEM_CODE      (0x0C)

// Prefixes for NDEF Records (to identify record type)
#define NDEF_URIPREFIX_NONE                 (0x00)
#define NDEF_URIPREFIX_HTTP_WWWDOT          (0x01)
#define NDEF_URIPREFIX_HTTPS_WWWDOT         (0x02)
#define NDEF_URIPREFIX_HTTP                 (0x03)
#define NDEF_URIPREFIX_HTTPS                (0x04)
#define NDEF_URIPREFIX_TEL                  (0x05)
#define NDEF_URIPREFIX_MAILTO               (0x06)
#define NDEF_URIPREFIX_FTP_ANONAT           (0x07)
#define NDEF_URIPREFIX_FTP_FTPDOT           (0x08)
#define NDEF_URIPREFIX_FTPS                 (0x09)
#define NDEF_URIPREFIX_SFTP                 (0x0A)
#define NDEF_URIPREFIX_SMB                  (0x0B)
#define NDEF_URIPREFIX_NFS                  (0x0C)
#define NDEF_URIPREFIX_FTP                  (0x0D)
#define NDEF_URIPREFIX_DAV                  (0x0E)
#define NDEF_URIPREFIX_NEWS                 (0x0F)
#define NDEF_URIPREFIX_TELNET               (0x10)
#define NDEF_URIPREFIX_IMAP                 (0x11)
#define NDEF_URIPREFIX_RTSP                 (0x12)
#define NDEF_URIPREFIX_URN                  (0x13)
#define NDEF_URIPREFIX_POP                  (0x14)
#define NDEF_URIPREFIX_SIP                  (0x15)
#define NDEF_URIPREFIX_SIPS                 (0x16)
#define NDEF_URIPREFIX_TFTP                 (0x17)
#define NDEF_URIPREFIX_BTSPP                (0x18)
#define NDEF_URIPREFIX_BTL2CAP              (0x19)
#define NDEF_URIPREFIX_BTGOEP               (0x1A)
#define NDEF_URIPREFIX_TCPOBEX              (0x1B)
#define NDEF_URIPREFIX_IRDAOBEX             (0x1C)
#define NDEF_URIPREFIX_FILE                 (0x1D)
#define NDEF_URIPREFIX_URN_EPC_ID           (0x1E)
#define NDEF_URIPREFIX_URN_EPC_TAG          (0x1F)
#define NDEF_URIPREFIX_URN_EPC_PAT          (0x20)
#define NDEF_URIPREFIX_URN_EPC_RAW          (0x21)
#define NDEF_URIPREFIX_URN_EPC              (0x22)
#define NDEF_URIPREFIX_URN_NFC              (0x23)

#define PN532_GPIO_VALIDATIONBIT            (0x80)
#define PN532_GPIO_P30                      (0)
#define PN532_GPIO_P31                      (1)
#define PN532_GPIO_P32                      (2)
#define PN532_GPIO_P33                      (3)
#define PN532_GPIO_P34                      (4)
#define PN532_GPIO_P35                      (5)

// FeliCa consts
#define FELICA_READ_MAX_SERVICE_NUM         16
#define FELICA_READ_MAX_BLOCK_NUM           12 // for typical FeliCa card
#define FELICA_WRITE_MAX_SERVICE_NUM        16
#define FELICA_WRITE_MAX_BLOCK_NUM          10 // for typical FeliCa card
#define FELICA_REQ_SERVICE_MAX_NODE_NUM     32

#define PN532_PACKET_BUF_LEN                2048

class PN532
{
public:
    // see 7.3.13 https://www.nxp.com/docs/en/user-guide/141520.pdf
    enum Type:uint8_t{
        PASSIVE_TYPE_A          =0x00,
        PASSIVE_212             =0x01,
        PASSIVE_424             =0x02,
        PASSIVE_TYPE_B          =0x03,
        JEWEL                   =0x04,
        MIFARE                  =0x10,
        FELICA_212              =0x11,
        FELICA_424              =0x12,
        COMPLIANT_TYPE_A        =0x20,
        COMPLIANT_TYPE_B        =0x23,
        DEP_PASSIVE_106         =0x40,
        DEP_PASSIVE_212         =0x41,
        DEP_PASSIVE_424         =0x42,
        DEP_ACTIVE_106          =0x80,
        DEP_ACTIVE_212          =0x81,
        DEP_ACTIVE_424          =0x82
    };
    // card data
    struct PICC{
        struct TypeB{
            uint8_t tg;
            uint8_t pupi[4];
            uint8_t ap[4];
            uint8_t prot[3];
        };
    };


    PN532(PN532Interface &interface);

    // Generic PN532 functions
    /**
     * @brief  Configures the SAM (Secure Access Module)
    */
    bool SAMConfig();
    /**
     * @brief  Checks the firmware version of the PN5xx chip, see 7.2.2 https://www.nxp.com/docs/en/user-guide/141520.pdf
     * @returns  The chip's firmware version and ID
    */
    uint32_t getFirmwareVersion();
    uint32_t readRegister(uint16_t reg);
    uint32_t writeRegister(uint16_t reg, uint8_t val);


    // polling
    /**
     * @brief The goal of this command is to detect as many targets (maximum MaxTg) as possible in passive mode.
     * @param[in] maxtg The maximum number of targets to be initialized by the PN532.
     * The PN532 is capable of handling 2 targets maximum at once, so this field should not exceed 0x02. For Jewel card, only one target can be initialized.
     * @param[in] type The baud rate and the modulation type to be used during the initialization.
     * @param[in] data An array of data to be used during the initialization of the target(s). Depending on the Baud Rate specified, the content of this field is different.
     * see 7.3.5 https://www.nxp.com/docs/en/user-guide/141520.pdf
     * @param[in] datalen Length of data.
     * @param[out] targetdata Contains the information about the detected targets and depends on the baud rate selected.
     * @param[out] targetdatalen Length of targetdata.
     * @return The Number of initialized Targets (minimum 0, maximum 2 targets)
    */
    uint8_t inListPassiveTarget(const uint8_t maxtg,const PN532::Type type,const uint8_t *data,const uint8_t datalen,uint8_t *targetdata,uint8_t *targetdatalen);
    /**
     * @brief This command is used to poll card(s) / target(s) of specified Type present in the RF field.
     * @param[in] pollNr specifies the number of polling (one polling is a polling for each Type j).
     * 0x01 – 0xFE : 1 up to 254 polling.
     * 0xFF : Endless polling.
     * @param[in] period (0x01 – 0x0F) indicates the polling period in units of 150 ms.
     * @param[in] types indicates the mandatory target type to be polled.
     * 
     * @param[out] foundtypes
     * @param[out] foundlen The number of target(s) found (maximum is two targets, only one of them can be DEP compliant)
     * @return found count
    */
    uint8_t inAutoPoll(const uint8_t pollNr,const uint8_t period,const PN532::Type *types,const uint8_t typeslen,uint8_t *foundtypes,uint8_t *foundlen);

    uint8_t PollingTypeB(const uint8_t maxtg,const uint8_t afi,PN532::PICC::TypeB *picc);


    /**
     * @brief This command is used to support protocol data exchanges between the PN532 as initiator and a target. 
     * @param[in] tg A byte containing the logical number of the relevant target. see 7.4.5 https://www.nxp.com/docs/en/user-guide/141520.pdf
     * @param[in] send Data
     * @param[in] sendlen
     * @param[out] response
     * @param[out] responselen
     * @return Status code
    */
    uint8_t inDataExchange(const uint8_t tg,const uint8_t *send,const uint8_t sendlen,uint8_t *response,uint8_t *responselen);

    // APDU Commands

    // APDU Commands
    struct APDU{
        bool select(const uint8_t *id,const uint8_t idlen,const uint8_t p1,const uint8_t p2,uint8_t *status);
        bool select(const uint8_t *id,const uint8_t idlen,const uint8_t p1,const uint8_t p2);
        bool selectDF(const uint8_t *id,const uint8_t idlen);
        bool selectEF(const uint8_t *id,const uint8_t idlen);
        bool readBinary(const uint8_t p1,const uint8_t p2,const uint16_t Lelen,uint8_t *response,uint16_t *responseLength,uint8_t *status);
        bool readBinary(const uint8_t p1,const uint8_t p2,const uint16_t Lelen,uint8_t *response,uint16_t *responseLength);
        bool verify(const uint8_t *verificationdata,const uint8_t datalen,uint8_t *status);
        bool verify(const uint8_t *verificationdata,const uint8_t datalen);
    };

    

    // Help functions to display formatted text
    static void PrintHex(const uint8_t *data, const uint32_t numBytes);
    static void PrintHexChar(const uint8_t *pbtData, const uint32_t numBytes);

    uint8_t *getBuffer(uint8_t *len) {
        *len = sizeof(pn532_packetbuffer) - 4;
        return pn532_packetbuffer;
    };


private:
    uint8_t _uid[7];  // ISO14443A uid
    uint8_t _uidLen;  // uid len
    uint8_t _pupi[4]; // ISO14443B pupi
    uint8_t _apdata[4]; // ISO14443B apdata
    uint8_t _protocol[3]; // ISO14443B protocolinfo
    uint8_t _key[6];  // Mifare Classic key
    uint8_t inListedTag; // Tg number of inlisted tag.
    uint8_t _felicaIDm[8]; // FeliCa IDm (NFCID2)
    uint8_t _felicaPMm[8]; // FeliCa PMm (PAD)

    uint8_t pn532_packetbuffer[PN532_PACKET_BUF_LEN];

    PN532Interface *_interface;
};

#endif
