#ifndef SD_L0_H
#define SD_L0_H
#include <Arduino.h>
#include <avr/io.h>
/**
 * SD Chip Select pin
 *
 * Warning if this pin is redefined the hardware SS will pin will be enabled
 * as an output by init().  An avr processor will not function as an SPI
 * master unless SS is set to output mode.
 */
/** The default chip select pin for the SD card is SS. */

#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
// Mega
uint8_t const SD_L0_CHIP_SELECT_PIN_DEFAULT = 4;

//------------------------------------------------------------------------------
#elif defined(__AVR_ATmega644P__) || defined(__AVR_ATmega644__)
// SPI port
uint8_t const SD_L0_CHIP_SELECT_PIN_DEFAULT = 4;

//------------------------------------------------------------------------------
#elif defined(__AVR_ATmega32U4__)
// Teensy 2.0
uint8_t const SD_L0_CHIP_SELECT_PIN_DEFAULT = 0;

//------------------------------------------------------------------------------
#elif defined(__AVR_AT90USB646__) || defined(__AVR_AT90USB1286__)
// Teensy++ 1.0 & 2.0
uint8_t const SD_L0_CHIP_SELECT_PIN_DEFAULT = 20;

//------------------------------------------------------------------------------
#else  
// 168 and 328 Arduinos
uint8_t const SD_L0_CHIP_SELECT_PIN_DEFAULT = 4;

#endif  

/**
 * Set chip-select from sd-card to low.
 * Can be a makro or a function.
 */
#define SD_L0_SetCSLow()	{ digitalWrite(SD_L0_CSPin, LOW); }

/**
 * Set chip-select from sd-card to high.
 * Can be a makro or a function.
 */
#define SD_L0_SetCSHigh()	{ digitalWrite(SD_L0_CSPin, HIGH); }

/**
 * Get timestamp. Put your favourite function
 * here to get a timestamp. Timestamp should
 * count not faster than 500us.
 * Return value must be uint16_t! 
 * Overflows doesn't matter as long as 
 * overflow_period is bigger than largest
 * timeout.
 */
#define SD_L0_GetTimestamp()	((uint16_t)millis())

/** 
 * Define timeouts for different functions here
 * based on your GetTimestamp value.
 * All timeouts values should smaller than 30000.
 */
/** init timeout typ. 2000 ms */
#define SD_INIT_TIMEOUT 2000
/** erase timeout typ. 10000 ms */
#define SD_ERASE_TIMEOUT 10000
/** read timeout typ. 300 ms */
#define SD_READ_TIMEOUT 300
/** write time out typ. 600 ms */
#define SD_WRITE_TIMEOUT 600 
/** command timeout typ. 300 ms */
#define SD_COMMAND_TIMEOUT 300

void SD_L0_Init(void);
void SD_L0_DeInit(void);
void SD_L0_SpiSetHighSpeed(void);
uint8_t SD_L0_SpiRecvByte();
void SD_L0_SpiSendByte(uint8_t b);
void SD_L0_SpiRecvBlock(uint8_t *buf, uint16_t nbyte);
void SD_L0_SpiSendBlock(uint8_t token, const uint8_t *buf);

extern uint8_t SD_L0_CSPin;

#endif


