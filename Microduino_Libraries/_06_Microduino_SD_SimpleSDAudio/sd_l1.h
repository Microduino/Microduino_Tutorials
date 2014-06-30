#ifndef SD_L1_H
#define SD_L1_H

#define SD_ENABLE_WRITE_ACCESS          0
#define SD_ENABLE_MULTIBLOCK_ACCESS     1
#define SD_ENABLE_L1_INFORMATIVE        0

// init/deinit functions
// ***************************************
uint8_t     SD_L1_Init();
void        SD_L1_DeInit();

// read access to 512 byte blocks
// ***************************************
uint8_t     SD_L1_ReadBlock(uint32_t blockNumber, uint8_t *dst);
#if SD_ENABLE_MULTIBLOCK_ACCESS
  uint8_t   SD_L1_ReadMBStart(uint32_t blockNumber); 
  uint8_t   SD_L1_ReadMB(uint8_t *dst); 
  uint8_t   SD_L1_ReadMBStop();
#endif /* SD_ENABLE_MULTIBLOCK_ACCESS */

// write access to 512 byte blocks
// ***************************************
#if SD_ENABLE_WRITE_ACCESS
  uint8_t   SD_L1_WriteBlock(uint32_t blockNumber, const uint8_t *src);
  #if SD_ENABLE_MULTIBLOCK_ACCESS
    uint8_t SD_L1_WriteMBStart(uint32_t blockNumber, uint32_t eraseCount);
    uint8_t SD_L1_WriteMB(const uint8_t* src);
    uint8_t SD_L1_WriteMBStop();
  #endif /* SD_ENABLE_MULTIBLOCK_ACCESS */
#endif /* SD_ENABLE_WRITE_ACCESS */


// other functions, just informative
// ***************************************
uint8_t     SD_L1_GetCardType();
#if SD_ENABLE_L1_INFORMATIVE
    uint32_t    SD_L1_GetCardSize();
    uint8_t     SD_L1_ReadCSD(uint8_t *dst);
    uint8_t     SD_L1_ReadCID(uint8_t *dst);
#endif


// error codes
/** timeout error for command CMD0 (initialize card in SPI mode), signal problem */
#define SD_CARD_ERROR_CMD0          0x01
/** CMD8 was not accepted - not a valid SD card */
#define SD_CARD_ERROR_CMD8          0x02
/** ACMD41 initialization process timeout */
#define SD_CARD_ERROR_ACMD41        0x03
/** card returned an error response for CMD58 (read OCR) */
#define SD_CARD_ERROR_CMD58         0x04
/** card returned an error response for CMD16 (set block len) */
#define SD_CARD_ERROR_CMD16         0x05
/** card operation voltage range doesn't match (2.7V - 3.6V) */
#define SD_CARD_ERROR_VOLTMATCH     0x06
/** timeout while waiting for start of read data */
#define SD_CARD_ERROR_READ_TIMEOUT  0x07
/** card returned error token when tried to read data */
#define SD_CARD_ERROR_READ          0x08
/** card returned an error response for CMD17 (read single block) */
#define SD_CARD_ERROR_CMD17         0x09
/** card returned an error token when tried to write data */
#define SD_CARD_ERROR_WRITE         0x0a
/** card returned an error response for CMD24 (write single block) */
#define SD_CARD_ERROR_CMD24         0x0b
/** timeout while waiting for writing data */
#define SD_CARD_ERROR_WRITE_TIMEOUT 0x0c
/** card returned an error in status after a data writing command */
#define SD_CARD_ERROR_WRITE_PROGR   0x0d
/** card returned an error response for CMD9  (read CSD) */
#define SD_CARD_ERROR_CMD9          0x0e
/** card returned an error response for CMD10 (read CID) */
#define SD_CARD_ERROR_CMD10         0x0f
/** card returned an error response for CMD18 (read multi block) */
#define SD_CARD_ERROR_CMD18         0x10
/** card returned an error response for CMD12 (stop block operation) */
#define SD_CARD_ERROR_CMD12         0x11
/** card returned an error response for ACMD23 (set pre-erase count) */
#define SD_CARD_ERROR_ACMD23        0x12
/** card returned an error response for CMD25 (write multi block) */
#define SD_CARD_ERROR_CMD25         0x13
/** card did not become ready for a multiple block write */
#define SD_CARD_ERROR_WRITE_MB      0x14 
/** timeout after sending STOP_TRAN_TOKEN after write multi block data */
#define SD_CARD_ERROR_STOP_TRAN     0x15

// card types
/** Standard capacity V1 SD card */
#define SD_CARD_TYPE_SD1  1
/** Standard capacity V2 SD card */
#define SD_CARD_TYPE_SD2  2
/** High Capacity SD card */
#define SD_CARD_TYPE_SDHC 3

#endif