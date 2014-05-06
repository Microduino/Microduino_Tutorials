
#include "sd_l0.h"
#include "sd_l1.h"

// SD card commands
/** GO_IDLE_STATE - init card in spi mode if CS low */
#define SD_CMD0 0x00
/** SEND_IF_COND - verify SD Memory Card interface operating condition.*/
#define SD_CMD8 0x08
/** SEND_CSD - read the Card Specific Data (CSD register), response R1 */
#define SD_CMD9 0x09
/** SEND_CID - read the card identification information (CID register), response R1 */
#define SD_CMD10 0x0A
/** STOP_TRANSMISSION - end multiple block read sequence, response R1b */
#define SD_CMD12 0x0C
/** SEND_STATUS - read the card status register, response R2 */
#define SD_CMD13 0x0D
/** SET_BLOCKLEN arg0[31:0]: block length, response R1 */
#define SD_CMD16 0x10
/** READ_SINGLE_BLOCK - read a single data block from the card, response R1 */
#define SD_CMD17 0x11
/** READ_MULTIPLE_BLOCK - read a multiple data blocks from the card, response R1 */
#define SD_CMD18 0x12
/** WRITE_BLOCK - write a single data block to the card, response R1 */
#define SD_CMD24 0x18
/** WRITE_MULTIPLE_BLOCK - write blocks of data until a STOP_TRANSMISSION, response R1 */
#define SD_CMD25 0x19
/** ERASE_WR_BLK_START - sets the address of the first block to be erased */
//#define SD_CMD32 0x20
/** ERASE_WR_BLK_END - sets the address of the last block of the continuous
    range to be erased*/
//#define SD_CMD33 0x21
/** ERASE - erase all previously selected blocks */
//#define SD_CMD38 0x26
/** APP_CMD - escape for application specific command */
#define SD_CMD55 0x37
/** READ_OCR - read the OCR register of a card */
#define SD_CMD58 0x3A
/** CRC_ON_OFF - Turns CRC option on or off, response R1 */
#define SD_CMD59 0x3B

/** SET_WR_BLK_ERASE_COUNT - Set the number of write blocks to be
     pre-erased before writing, response R1 */
#define SD_ACMD23 0x17
/** SD_SEND_OP_COMD - Sends host capacity support information and
    activates the card's initialization process */
#define SD_ACMD41 0x29

//------------------------------------------------------------------------------
/** status for card in the ready state */
#define SD_R1_READY_STATE 0x00
/** status for card in the idle state */
#define SD_R1_IDLE_STATE 0x01
/** status bit for illegal command */
#define SD_R1_ILLEGAL_COMMAND 0x04
/** start data token for read or write single block*/
#define SD_DATA_START_BLOCK 0xFE
/** stop token for write multiple blocks*/
//#define SD_STOP_TRAN_TOKEN 0xFD
/** start data token for write multiple blocks*/
//#define SD_WRITE_MULTIPLE_TOKEN 0xFC
/** mask for data response tokens after a write block operation */
//#define SD_DATA_RES_MASK 0x1F
/** write data accepted token */
//#define SD_DATA_RES_ACCEPTED 0x05

// prototypes for internal usage
uint8_t SD_L1_WaitNotBusy(uint16_t timeout);
uint8_t SD_L1_CardCommand(uint8_t cmd, uint32_t arg);
uint8_t SD_L1_CardACommand(uint8_t cmd, uint32_t arg);
uint8_t SD_L1_ReadData(uint8_t *dst, uint16_t count);
uint8_t SD_L1_WriteData(uint8_t token, const uint8_t *src);

uint8_t SD_L1_type;

/** Sends also one dummy byte to ensure MISO goes high impedance */
void SD_L1_SetCSHigh() {
  SD_L0_SetCSHigh();
  SD_L0_SpiSendByte(0xff);
}

#define SD_L1_SetCSLow() SD_L0_SetCSLow()

/** wait for card to go not busy */
uint8_t SD_L1_WaitNotBusy(uint16_t timeout) 
{
  uint16_t t0 = SD_L0_GetTimestamp();
  while (SD_L0_SpiRecvByte() != 0xFF) {
    if ((SD_L0_GetTimestamp() - t0) >= timeout) return 0;
  }
  return 1;
}

/**
 * Send a command to the memory card which responses with a R1 response (and possibly others).
 *
 * \param[in] command The command to send.
 * \param[in] arg The argument for command.
 * \returns The command answer.
 */
uint8_t SD_L1_CardCommand(uint8_t cmd, uint32_t arg) 
{
  uint8_t response;
  uint8_t crc = 0xFF;
      
  // select card
  SD_L1_SetCSLow();

  // wait up to timeout if busy
  SD_L1_WaitNotBusy(SD_COMMAND_TIMEOUT);

  // send command
  SD_L0_SpiSendByte(cmd | 0x40);

  // send argument
  SD_L0_SpiSendByte((arg >> 24) & 0xff);
  SD_L0_SpiSendByte((arg >> 16) & 0xff);
  SD_L0_SpiSendByte((arg >> 8) & 0xff);
  SD_L0_SpiSendByte((arg >> 0) & 0xff);
  
  // send CRC, only required for commands 0 and 8
  if (cmd == SD_CMD0) crc = 0x95;  // correct crc for CMD0 with arg 0
  if (cmd == SD_CMD8) crc = 0x87;  // correct crc for CMD8 with arg 0X1AA
  SD_L0_SpiSendByte(crc);

  // skip stuff byte for stop read
  if (cmd == SD_CMD12) SD_L0_SpiRecvByte();

  // wait for response
  for(uint8_t i = 0; i < 100; ++i)
  {
      response = SD_L0_SpiRecvByte();
      if(response != 0xff)
          break;
  }
  return response;
}

/**
 * Send an application specific command which responses with a R1 response (and possibly others).
 *
 * \param[in] command The command to send.
 * \param[in] arg The argument for command.
 * \returns The command answer.
 */
uint8_t SD_L1_CardACommand(uint8_t cmd, uint32_t arg)
{
    SD_L1_CardCommand(SD_CMD55, 0);
    return SD_L1_CardCommand(cmd, arg);
}

/**
 * Read one block of data from card.
 *
 * \return Error code, 0 if ok.
 */
uint8_t SD_L1_ReadData(uint8_t *dst, uint16_t count) 
{
  // wait for start block token
  uint8_t status;
  uint16_t t0 = SD_L0_GetTimestamp();
  
  while ((status = SD_L0_SpiRecvByte()) == 0xFF) {
    if ((SD_L0_GetTimestamp() - t0) > SD_READ_TIMEOUT) {
      SD_L1_SetCSHigh();
      return(SD_CARD_ERROR_READ_TIMEOUT);
    }
  }
  
  if (status != SD_DATA_START_BLOCK) {
    SD_L1_SetCSHigh();
    return(SD_CARD_ERROR_READ);
  }
  
  // transfer data
  SD_L0_SpiRecvBlock(dst, count);

  // discard CRC
  SD_L0_SpiRecvByte();
  SD_L0_SpiRecvByte();
  SD_L1_SetCSHigh();

  return 0;
}

/**
 * Write one block of data to card.
 *
 * \return Error code, 0 if ok.
 */
uint8_t SD_L1_WriteData(uint8_t token, const uint8_t *src) 
{
  uint8_t status;
  
  SD_L0_SpiSendBlock(token, src);

  SD_L0_SpiSendByte(0xff);  // dummy crc
  SD_L0_SpiSendByte(0xff);  // dummy crc

  status = SD_L0_SpiRecvByte();
  if ((status & 0x1f) != 0x05) {
    SD_L1_SetCSHigh();
    return(SD_CARD_ERROR_WRITE);
  }
  return 0;
}

// --------------------------------------------------------------------------------------
// ------------------------------------- Public functions -------------------------------
// --------------------------------------------------------------------------------------

/**
 * Initialize the SD memory card.
 *
 * Power up the card, set SPI mode.
 * Detects the card version (V1, V2, SDHC), sets
 * sector length to 512.
 *
 * \return Zero if successfull, errorcode otherwise
 */
uint8_t SD_L1_Init()
{
  uint8_t status;
  // 16-bit init start time allows over a minute
  uint16_t t0 = SD_L0_GetTimestamp();
  uint32_t arg;
  
  SD_L1_type = 0;

  // set pin modes
  SD_L0_Init();
  
  // must supply min of 74 clock cycles with CS high.
  SD_L1_SetCSHigh();
  for (uint8_t i = 0; i < 10; i++) SD_L0_SpiSendByte(0xFF);

  // command to go idle in SPI mode
  while ((SD_L1_CardCommand(SD_CMD0, 0)) != SD_R1_IDLE_STATE) {
    if ((SD_L0_GetTimestamp() - t0) > SD_INIT_TIMEOUT) {
      SD_L1_SetCSHigh();
      return(SD_CARD_ERROR_CMD0);
    }
  }
  
  // check SD version ( 2.7V - 3.6V + test pattern )
  if ((SD_L1_CardCommand(SD_CMD8, 0x1AA) & SD_R1_ILLEGAL_COMMAND)) {
    SD_L1_type = SD_CARD_TYPE_SD1;
    // Not done here: Test if SD or MMC card here using CMD55 + CMD1
  } else {
    // only need last byte of r7 response
    SD_L0_SpiRecvByte();
    SD_L0_SpiRecvByte();
    status = SD_L0_SpiRecvByte();
    if ((status & 0x01) == 0) {
      // card operation voltage range doesn't match
      SD_L1_SetCSHigh();
      return(SD_CARD_ERROR_VOLTMATCH);       
    }
    if (SD_L0_SpiRecvByte() != 0xAA) {
      SD_L1_SetCSHigh();
      return(SD_CARD_ERROR_CMD8);
    }
    SD_L1_type = SD_CARD_TYPE_SD2;
  }
  
  // Turn CRC option off
  SD_L1_CardCommand(SD_CMD59, 0);
  
  // initialize card and send host supports SDHC if SD2
  arg = (SD_L1_type == SD_CARD_TYPE_SD2) ? 0X40000000 : 0;
  while ((SD_L1_CardACommand(SD_ACMD41, arg)) != SD_R1_READY_STATE) {
    // check for timeout
    if ((SD_L0_GetTimestamp() - t0) > SD_INIT_TIMEOUT) {
      SD_L1_SetCSHigh();
      return(SD_CARD_ERROR_ACMD41);
    }
  }
  
  // if SD2 read OCR register to check for SDHC card
  if (SD_L1_type == SD_CARD_TYPE_SD2) {
    if (SD_L1_CardCommand(SD_CMD58, 0)) {
      SD_L1_SetCSHigh();
      return(SD_CARD_ERROR_CMD58);
    }
    // other implementation test only against 0x40 for SDHC detection...
    if ((SD_L0_SpiRecvByte() & 0xC0) == 0xC0) SD_L1_type = SD_CARD_TYPE_SDHC;
    // discard rest of ocr - contains allowed voltage range
    SD_L0_SpiRecvByte();
    SD_L0_SpiRecvByte();
    SD_L0_SpiRecvByte();
  }

  // set block size to 512 bytes
  if(SD_L1_CardCommand(SD_CMD16, 512))
  {
      SD_L1_SetCSHigh();
      return(SD_CARD_ERROR_CMD16);
  }
  
  SD_L1_SetCSHigh();
  SD_L0_SpiSetHighSpeed();
  return 0;
}

/**
 * Shutdown SD card safely 
 */
void SD_L1_DeInit()
{
  SD_L1_type = 0;
  
  // must supply min of 74 clock cycles with CS high.
  SD_L1_SetCSHigh();
  for (uint8_t i = 0; i < 10; i++) SD_L0_SpiSendByte(0xFF);
  
  SD_L0_DeInit();
}

/** Return the card type: SD V1, SD V2 or SDHC
 * \return 0 - no card, 1 - SD V1, 2 - SD V2, or 3 - SDHC.
 */
uint8_t SD_L1_GetCardType() 
{
    return(SD_L1_type);
}

/**
 * Read a 512 byte block from an SD card.
 *
 * \param[in] blockNumber Logical block to be read.
 * \param[out] dst Pointer to the location that will receive the data.
 *
 * \return 0 is returned for success, error code otherwise
 */
uint8_t SD_L1_ReadBlock(uint32_t blockNumber, uint8_t *dst) 
{
  // use address if not SDHC card
  if (SD_L1_type != SD_CARD_TYPE_SDHC) blockNumber <<= 9;
  if (SD_L1_CardCommand(SD_CMD17, blockNumber)) {
    SD_L1_SetCSHigh();
    return(SD_CARD_ERROR_CMD17);
  }
  return SD_L1_ReadData(dst, 512);
}

#if SD_ENABLE_MULTIBLOCK_ACCESS
/** 
 * Start a read multiple blocks sequence.
 *
 * \param[in] blockNumber Address of first block in sequence.
 *
 * \note This function is used with ReadMB() and StopReadMB() for optimized
 * multiple block reads.  SPI chipSelect must be low for the entire sequence.
 *
 * \return 0 is returned for success, error code otherwise
 */
uint8_t SD_L1_ReadMBStart(uint32_t blockNumber) 
{
  if (SD_L1_type != SD_CARD_TYPE_SDHC) blockNumber <<= 9;
  if (SD_L1_CardCommand(SD_CMD18, blockNumber)) {
    SD_L1_SetCSHigh();
    return(SD_CARD_ERROR_CMD18);
  }
  SD_L1_SetCSHigh();
  return 0;
}

/** 
 * Read one data block in a multiple block read sequence
 *
 * \param[in] dst Pointer to the location that will receive the data.
 *
 * \return 0 is returned for success, error code otherwise
 */
uint8_t SD_L1_ReadMB(uint8_t *dst) 
{
  SD_L1_SetCSLow();
  return SD_L1_ReadData(dst, 512);
}

/** 
 * End a read multiple blocks sequence.
 *
 * \return 0 is returned for success, error code otherwise
 */
uint8_t SD_L1_ReadMBStop() 
{
  SD_L1_SetCSLow();
  if (SD_L1_CardCommand(SD_CMD12, 0)) {
    SD_L1_SetCSHigh();
    return(SD_CARD_ERROR_CMD12);
  }
  SD_L1_SetCSHigh();
  return 0;  
}
#endif /* SD_ENABLE_MULTIBLOCK_ACCESS */

#if SD_ENABLE_L1_INFORMATIVE
/**
 * Read the 16 byte CSD register of SD card.
 *
 * Remark: It seems that the only usefull information stored in CSD
 *         is the actual card size only as other fields are 
 *         typically filled with too general values.
 *
 * \param[out] dst Pointer to the location that will receive the data.
 * \return 0 is returned for success, error code otherwise
 */
uint8_t SD_L1_ReadCSD(uint8_t *dst)
{
  if (SD_L1_CardCommand(SD_CMD9, 0)) {
    SD_L1_SetCSHigh();
    return(SD_CARD_ERROR_CMD9);
  }
  return SD_L1_ReadData(dst, 16);
}

/**
 * Read the 16 byte CID register of SD card.
 *
 * \param[out] dst Pointer to the location that will receive the data.
 * \return 0 is returned for success, error code otherwise
 */
uint8_t SD_L1_ReadCID(uint8_t *dst)
{
  if (SD_L1_CardCommand(SD_CMD10, 0)) {
    SD_L1_SetCSHigh();
    return(SD_CARD_ERROR_CMD10);
  }
  return SD_L1_ReadData(dst, 16);
} 

/*        
 SD_L1_GetCardType: 2, ReadCSD: 00 7f ff 32 5b 59 83 b9  f6 db df ff 12 40 00 8f  
 SD_L1_GetCardType: 3, ReadCSD: 40 0e 00 32 5b 59 00 00  3b 33 7f 80 0a 40 00 a5  
*/
/**
 * Determine the size of an SD flash memory card.
 *
 * \return The number of 512 byte data blocks in the card
 *         or zero if an error occurs.
 */
uint32_t SD_L1_GetCardSize() 
{
  uint8_t csddata[16];
  if (SD_L1_ReadCSD(&csddata[0])) return 0;
  if ((csddata[0] & 0xc0) == 0) {
    // version 1.00 cards
    uint8_t read_bl_len = ((csddata[5] & 0x0f));
    uint16_t c_size = ((csddata[6] & 0x03) << 10)
                      | (csddata[7] << 2) 
                      | ((csddata[8] & 0xc0)>>6);
    uint8_t c_size_mult = ((csddata[9] & 0x03) << 1)
                          | ((csddata[10] & 0x80)>>7);
    return (uint32_t)(c_size + 1) << (c_size_mult + read_bl_len - 7);
  } else if ((csddata[0] & 0xc0) == 0x40) {
    // version 2.00 cards
    uint32_t c_size = (((uint32_t)csddata[7] & 0xfc) << 14)
                      | (csddata[8] << 8) 
                      | csddata[9];
    return (c_size + 1) << 10;
  } else {
    // error(SD_CARD_ERROR_BAD_CSD);
    return 0;
  }
}
#endif

#if SD_ENABLE_WRITE_ACCESS
/**
 * Writes a 512 byte block to an SD card.
 *
 * \param[in] blockNumber Logical block to be written.
 * \param[in] src Pointer to the location of the data to be written.
 *
 * \return 0 is returned for success, error code otherwise
 */
uint8_t SD_L1_WriteBlock(uint32_t blockNumber, const uint8_t *src) 
{
  uint8_t response;

  // use address if not SDHC card
  if (SD_L1_type != SD_CARD_TYPE_SDHC) blockNumber <<= 9;
  if (SD_L1_CardCommand(SD_CMD24, blockNumber)) {
    SD_L1_SetCSHigh();
    return(SD_CARD_ERROR_CMD24);
  }
  
  // Write single block start block token (0xfe), data block and dummy crc to card
  response = SD_L1_WriteData(0xfe, src);
  if (response) return(response);

  // wait for flash programming to complete
  if (!SD_L1_WaitNotBusy(SD_WRITE_TIMEOUT)) {
    SD_L1_SetCSHigh();
    return(SD_CARD_ERROR_WRITE_TIMEOUT);
  }
  
  // response is r2 so get and check two bytes for nonzero
  if (SD_L1_CardCommand(SD_CMD13, 0) || SD_L0_SpiRecvByte()) {
    SD_L1_SetCSHigh();
    return(SD_CARD_ERROR_WRITE_PROGR);
  }
  SD_L1_SetCSHigh();
  return 0;
}

#if SD_ENABLE_MULTIBLOCK_ACCESS
/** 
 * Start a write multiple blocks sequence.
 *
 * \param[in] blockNumber Address of first block in sequence.
 * \param[in] eraseCount The number of blocks to be pre-erased.
 *            Set to 0 if not used.
 *
 * \note This function is used with WriteMB() and StopWriteMB() for optimized
 * multiple block writes.  SPI chipSelect must be low for the entire sequence.
 *
 * \return 0 is returned for success, error code otherwise
 */
uint8_t SD_L1_WriteMBStart(uint32_t blockNumber, uint32_t eraseCount) 
{
  // send pre-erase count
  if(eraseCount) {
      if (SD_L1_CardACommand(SD_ACMD23, eraseCount)) {
        SD_L1_SetCSHigh();
        return(SD_CARD_ERROR_ACMD23);
      }
  }
  // use address if not SDHC card
  if (SD_L1_type != SD_CARD_TYPE_SDHC) blockNumber <<= 9;
  if (SD_L1_CardCommand(SD_CMD25, blockNumber)) {
    SD_L1_SetCSHigh();
    return(SD_CARD_ERROR_CMD25);
  }
  SD_L1_SetCSHigh();
  return 0;
}

/** 
 * Write one data block in a multiple block write sequence
 *
 * \param[in] src Pointer to the location of the data to be written.
 *
 * \return 0 is returned for success, error code otherwise
 */
uint8_t SD_L1_WriteMB(const uint8_t* src) 
{
  SD_L1_SetCSLow();
  // wait for previous write to finish
  if (!SD_L1_WaitNotBusy(SD_WRITE_TIMEOUT)) {
    SD_L1_SetCSHigh();
    return(SD_CARD_ERROR_WRITE_MB);
  }
  // 0xfc = WRITE_MULTIPLE_TOKEN
  if (SD_L1_WriteData(0xfc, src)) {
    SD_L1_SetCSHigh();
    return(SD_CARD_ERROR_WRITE_MB);
  }
  SD_L1_SetCSHigh();
  return 0;
}

/** 
 * End a write multiple blocks sequence.
 *
 * \return 0 is returned for success, error code otherwise
 */
uint8_t SD_L1_WriteMBStop() 
{
  SD_L1_SetCSLow();
  if (!SD_L1_WaitNotBusy(SD_WRITE_TIMEOUT)) {
    SD_L1_SetCSHigh();
    return(SD_CARD_ERROR_STOP_TRAN);
  }
  SD_L0_SpiSendByte(0xfd); // 0xfd = STOP_TRAN_TOKEN
  if (!SD_L1_WaitNotBusy(SD_WRITE_TIMEOUT)) {
    SD_L1_SetCSHigh();
    return(SD_CARD_ERROR_STOP_TRAN);
  }
  SD_L1_SetCSHigh();
  return 0; 
}
#endif /* SD_ENABLE_MULTIBLOCK_ACCESS */
#endif /* SD_ENABLE_WRITE_ACCESS */

//------------------------------------------------------------------------------

