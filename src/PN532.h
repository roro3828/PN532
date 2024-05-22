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

// REG CIU
#define REG_CIU_Mode                        (0x6301) // Defines general modes for transmitting and receiving.
#define REG_CIU_TxMode                      (0x6302) // Defines the transmission data rate and framing during transmission.
#define REG_CIU_RxMode                      (0x6303) // Defines the reception data rate and framing during receiving
#define REG_CIU_TxControl                   (0x6304) // Controls the logical behavior of the antenna driver pins TX1 and TX2.
#define REG_CIU_TxAuto                      (0x6305) // Controls the setting of the antenna driver
#define REG_CIU_Command                     (0x6331) // Starts and stops the command execution
#define REG_CIU_Status2                     (0x6338) // Contain status flags of the receiver, transmitter and Data Mode Detector
#define REG_CIU_Control                     (0x633C) // Contains miscellaneous control bits
#define REG_CIU_ManualRCV                   (0x630D) // Allows manual fine tuning of the internal receiver

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

#define PN532_PACKET_BUF_LEN                512
#define PN532_APDU_BUF_LEN                  512
#define IN_LIST_PASSIVE_TARGET_USE_SIZE     3
#define IN_DATA_EXCHANGE_USE_SIZE           2

// APDU Commands
#define APDU_CMD_READ_BINARY                0xB0
#define APDU_CMD_WRITE_BINARY               0xD0
#define APDU_CMD_SELECT_FILE                0xA4
#define APDU_CMD_VERIFY                     0x20
#define APDU_CMD_VERIFY_SPECIFIC_REFERENCE  0x80
#define APDU_CMD_READ_RECORD                0xB2

#define APDU_STATUS_SUCCESS                 0x9000

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
    class PICC{
        private:
        struct base{
            uint8_t tg;
            PN532 *pcd;
        };
        public:
        struct Test:base{
            uint8_t uid[8];
        };
        struct TypeA{
            uint8_t tg;
            uint8_t uid[10];
            uint8_t uidlen;
            PN532 *pcd;
        };
        struct TypeB{
            uint8_t tg;
            uint8_t pupi[4];
            uint8_t ap[4];
            uint8_t prot[3];
            PN532 *pcd;
        };
        struct Felica{
            uint8_t tg;
            uint8_t idm[8];
            uint8_t pmm[8];
            uint16_t systemcode;
            PN532 *pcd;
            // commands
            /**
             * @brief Read block data from service whitch dosen't require encryption.see https://www.sony.co.jp/Products/felica/business/tech-support/data/card_usersmanual_2.2j.pdf
             * @param[in] service_count Service count. (minimum 0, maximum 16)
             * @param[in] servicecode_list List of service codes. Size must be service_count
             * @param[in] block_count Block count.
             * @param[in] block_list List of block. Minimum size is 2*block_count, maximum is 3*block_count
             * @param[out] response Response blocks.
             * @param[out] response_count Response block count.
             * @return StatusFlag See 4.5 https://www.sony.co.jp/Products/felica/business/tech-support/data/card_usersmanual_2.2j.pdf
            */
            uint16_t readWithoutEncryption(const uint8_t service_count,const uint16_t *servicecode_list,const uint8_t block_count,const uint8_t *block_list,uint8_t response[][16],uint8_t *response_count);
            /**
             * @brief Write block data to service whitch dosen't require encryption.see https://www.sony.co.jp/Products/felica/business/tech-support/data/card_usersmanual_2.2j.pdf
             * @param[in] felica Felica
             * @param[in] service_count Service count. (minimum 0, maximum 16)
             * @param[in] servicecode_list List of service codes. Size must be service_count
             * @param[in] block_count Block count.
             * @param[in] block_list List of block. Minimum size is 2*block_count, maximum is 3*block_count
             * @param[in] blockData Data to write.
             * @return StatusFlag See 4.5 https://www.sony.co.jp/Products/felica/business/tech-support/data/card_usersmanual_2.2j.pdf
            */
            uint16_t writeWithoutEncryption(const uint8_t service_count,const uint16_t *servicecode_list,const uint8_t block_count,const uint8_t *block_list,const uint8_t blockData[][16]);
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
    bool getGeneralStatus(uint8_t *err,uint8_t *field,uint8_t *nbTg,uint8_t targets[][4],uint8_t *SAMstatus);
    bool readRegister(const uint16_t *addrlist,const uint8_t addrcount,uint8_t *vallist);
    bool readRegister(const uint16_t addr,uint8_t *val);
    bool writeRegister(const uint16_t *addrlist,const uint8_t *vallist,const uint8_t count);
    bool writeRegister(const uint16_t addr,const uint8_t val);
    bool setParameters(const uint8_t flags);
    enum RFConfigItem:uint8_t{
        RFfield                 =0x01,
        VariousTimings          =0x02,
        MaxRtyCOM               =0x04,
        MaxRetries              =0x05,
        AnalogSettingTypeA      =0x0A,
        AnalogSetting212_424    =0x0B,
        AnalogSettingTypeB      =0x0C,
        AnalogSetting848        =0x0D
    };
    bool RFConfiguration(const RFConfigItem configitem,const uint8_t *configdata);
    bool setRFfield(const bool autoRFCA,const bool RF);


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

    uint8_t PollingTypeA(const uint8_t maxtg,const uint8_t *selectuid,const uint8_t uidlen,PN532::PICC::TypeA *picc);
    uint8_t PollingTypeB(const uint8_t maxtg,const uint8_t afi,PN532::PICC::TypeB *picc);
    uint8_t PollingFelica(const uint8_t maxtg,const uint16_t systemcode,const uint8_t req,const uint8_t timeslot,PN532::PICC::Felica *picc);


    /**
     * @brief This command is used to support protocol data exchanges between the PN532 as initiator and a target. 
     * @param[in] tg A byte containing the logical number of the relevant target. see 7.4.5 https://www.nxp.com/docs/en/user-guide/141520.pdf
     * @param[in] send Data
     * @param[in] sendlen
     * @param[out] response
     * @param[out] responselen
     * @param[in] timeout ms
     * @return Status code
    */
    uint8_t inDataExchange(const uint8_t tg,const uint8_t *send,const uint16_t sendlen,uint8_t *response,uint16_t *responselen,uint16_t timeout=1000U);
    /**
     * @brief This command is used to support protocol data exchanges between the PN532 as initiator and a target. 
     * @param[in] tg A byte containing the logical number of the relevant target. see 7.4.5 https://www.nxp.com/docs/en/user-guide/141520.pdf
     * @param[in] sendlist Pointer list of send data
     * @param[in] sendlenlist List of send data length
     * @param[in] sendcount Number of data list
     * @param[out] response
     * @param[out] responselen
     * @param[in] timeout ms
     * @return Status code
    */
    uint8_t inDataExchange(const uint8_t tg,const uint8_t **sendlist,const uint16_t *sendlenlist,const uint8_t sendcount,uint8_t *response,uint16_t *responselen,uint16_t timeout=1000U);

    
    uint8_t tgInitAsTarget(const uint8_t mode,const uint8_t *mifareParams,const uint8_t *felicaParams,const uint8_t *nfcid,const uint8_t *gt,const uint8_t gtlen,const uint8_t *tk,const uint8_t tklen);
    uint8_t tgInitAsTarget(const uint8_t mode,const uint8_t *mifareParams,const uint8_t *felicaParams,const uint8_t *nfcid,const uint8_t *gt,const uint8_t gtlen,const uint8_t *tk,const uint8_t tklen,const uint16_t timeout);
    //tgInitAsTarget(const uint8_t mode,const uint16_t sensres,const uint8_t *nfcid1,const uint8_t selres,const uint8_t *nfcid2,const uint8_t *pad,const uint16_t *systemcode,const uint8_t *nfcid3,const uint8_t *gt,const uint8_t gtlen,const uint8_t *tk,const uint8_t tklen)
    uint8_t tgGetData(uint8_t *response,uint16_t *responselen);
    uint8_t tgSetData(const uint8_t *data,const uint16_t datalen);
    // APDU Commands
    /**
     * @brief Send APDU Command to target
     * @param[in] tg A byte containing the logical number of the relevant target. see 7.4.5 https://www.nxp.com/docs/en/user-guide/141520.pdf
     * @param[in] CLA Command Class
     * @param[in] INS Command code
     * @param[in] P1 Arg1
     * @param[in] P2 Arg2
     * @param[in] data
     * @param[in] datalen
     * @param[out] response
     * @param[out] responseLength
     * @param[in] le expected response length.
     * @param[in] timeout timeout ms
     * @return Status code
    */
    uint16_t SendAPDU(const uint8_t tg,const uint8_t CLA,const uint8_t INS,const uint8_t P1,const uint8_t P2,const uint8_t *data,const uint16_t dataLength,uint8_t *response,uint16_t *responseLength,uint16_t le=0,uint16_t timeout=1000U);


    uint16_t readBinary(const uint8_t tg,const uint8_t p1,const uint8_t p2,const uint16_t le,uint8_t *response,uint16_t *responseLength);
    uint16_t selectMF(const uint8_t tg,const uint16_t selectionControl,uint8_t *response=NULL,uint16_t *responseLength=NULL);
    uint16_t selectFile(const uint8_t tg,const uint16_t selectionControl,const uint8_t *id,const uint8_t idlen,uint8_t *response=NULL,uint16_t *responseLength=NULL);
    uint16_t selectDF(const uint8_t tg,const uint8_t *id,const uint8_t idlen);
    uint16_t selectEF(const uint8_t tg,const uint8_t *id,const uint8_t idlen);
    uint16_t verify(const uint8_t tg,const uint8_t qualifier,const uint8_t *verificationdata,const uint8_t datalen);
    uint16_t readRecord(const uint8_t tg,const uint8_t recordID,const uint8_t control,const uint8_t readLength,uint8_t *response,uint16_t *responseLength);


    // felica commands
    bool felica_requestService(const PICC::Felica *felica,const uint8_t node_count,const uint16_t *nodecode_list,uint16_t *response,uint8_t *responseLength);
    uint8_t felica_requestResponse(const PICC::Felica *felica);

    /**
     * @brief Read block data from service whitch dosen't require encryption.see https://www.sony.co.jp/Products/felica/business/tech-support/data/card_usersmanual_2.2j.pdf
     * @param[in] felica Felica
     * @param[in] service_count Service count. (minimum 0, maximum 16)
     * @param[in] servicecode_list List of service codes. Size must be service_count
     * @param[in] block_count Block count.
     * @param[in] block_list List of block. Minimum size is 2*block_count, maximum is 3*block_count
     * @param[out] response Response blocks.
     * @param[out] response_count Response block count.
     * @return StatusFlag See 4.5 https://www.sony.co.jp/Products/felica/business/tech-support/data/card_usersmanual_2.2j.pdf
    */
    uint16_t felica_readWithoutEncryption(const PICC::Felica *felica,const uint8_t service_count,const uint16_t *servicecode_list,const uint8_t block_count,const uint8_t *block_list,uint8_t response[][16],uint8_t *response_count);
    /**
     * @brief Write block data to service whitch dosen't require encryption.see https://www.sony.co.jp/Products/felica/business/tech-support/data/card_usersmanual_2.2j.pdf
     * @param[in] felica Felica
     * @param[in] service_count Service count. (minimum 0, maximum 16)
     * @param[in] servicecode_list List of service codes. Size must be service_count
     * @param[in] block_count Block count.
     * @param[in] block_list List of block. Minimum size is 2*block_count, maximum is 3*block_count
     * @param[in] blockData Data to write.
     * @return StatusFlag See 4.5 https://www.sony.co.jp/Products/felica/business/tech-support/data/card_usersmanual_2.2j.pdf
    */
    uint16_t felica_writeWithoutEncryption(const PICC::Felica *felica,const uint8_t service_count,const uint16_t *servicecode_list,const uint8_t block_count,const uint8_t *block_list,const uint8_t blockData[][16]);

    

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

    uint8_t pn532_zero=0;

    uint8_t pn532_packetbuffer[PN532_PACKET_BUF_LEN];

    PN532Interface *_interface;
};
#endif
