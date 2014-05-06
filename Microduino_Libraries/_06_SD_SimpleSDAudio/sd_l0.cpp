#include <Arduino.h>
#include <stdint.h>
#include "pins_arduino.h"
#include <avr/io.h>
#include "sd_l0.h"

uint8_t SD_L0_CSPin;

/**
 * Setup pins and SPI for operation with sd-card.
 *
 * Pin settings:
 * - Set all pins for proper operation
 * - power up card if needed
 *
 * SPI configuration: 
 * - enable uC for SPI master
 * - typical no interrupts are used for SPI
 * - data order: MSB is transmitted first
 * - clock polarity: CLK is low when idle
 * - clock phase: 1-0 > Sample, 0-1 > Setup
 * - clock frequency: less than 400kHz 
 *   (will be switched to higher value after initialization)
 *
 */
void SD_L0_Init(void)
{
	/* Power up card */
	// not for arduino
	
	/* Setup ports */
   pinMode(SD_L0_CSPin, OUTPUT);
   SD_L0_SetCSHigh();
   
   // pinMode(SD_L0_SPI_MISO_PIN, INPUT);
   // pinMode(SD_L0_SPI_MOSI_PIN, OUTPUT);
   // pinMode(SD_L0_SPI_SCK_PIN, OUTPUT);
   // pinMode(SD_L0_SPI_SS_FORCE_OUTPUT, OUTPUT);
   
   pinMode(MISO, INPUT);
   pinMode(SCK, OUTPUT);
   pinMode(MOSI, OUTPUT);
   pinMode(SS, OUTPUT);
  
   digitalWrite(SCK, LOW);
   digitalWrite(MOSI, LOW);
   digitalWrite(SS, HIGH);
   
    /* Powering up takes at least 500us for capacitors to charge */
    // not for arduino

    /* initialize SPI with lowest frequency; max. 400kHz during identification mode of card */
    SPCR = (0 << SPIE) | /* SPI Interrupt Enable */
           (1 << SPE)  | /* SPI Enable */
           (0 << DORD) | /* Data Order: MSB first */
           (1 << MSTR) | /* Master mode */
           (0 << CPOL) | /* Clock Polarity: SCK low when idle */
           (0 << CPHA) | /* Clock Phase: sample on rising SCK edge */
           (1 << SPR1) | /* Clock Frequency: f_OSC / 128 */
           (1 << SPR0);
    SPSR &= ~(1 << SPI2X); /* No doubled clock frequency */

}

/**
 * Setup pins and SPI for minimum power consumption.
 * Deactivate SPI and power from card
 */
void SD_L0_DeInit(void)
{
	SPCR &= ~(1 << SPE);
	
	/* Setup ports */
	/* for safe eject */
   pinMode(SD_L0_CSPin, INPUT);
   pinMode(MISO, INPUT);
   pinMode(MOSI, INPUT);
   pinMode(SCK, INPUT);
   digitalWrite(SD_L0_CSPin, HIGH); // Set CS high
   digitalWrite(SCK, HIGH); 
	
	/* Power down card */
	// not for arduino
}

/**
 * Set SPI for full operation speed (up to 25 MHz).
 *
 * Will be called after first part of card 
 * initialization was successful.
 */
void SD_L0_SpiSetHighSpeed(void)
{
    SPCR &= ~((1 << SPR1) | (1 << SPR0)); /* Clock Frequency: f_OSC / 4 */
    SPSR |= (1 << SPI2X);         /* Doubled Clock Frequency: f_OSC / 2 */
}

/**
 * Receives a raw byte from the SPI.
 *
 * \returns The byte which should be read.
 */
uint8_t SD_L0_SpiRecvByte()
{
    /* send dummy data for receiving some */
    SPDR = 0xff;
    while(!(SPSR & (1 << SPIF)));
    SPSR &= ~(1 << SPIF);

    return SPDR;
}

/**
 * Sends a raw byte to the SPI.
 *
 * \param[in] b The byte to sent.
 */
void SD_L0_SpiSendByte(uint8_t b)
{
    SPDR = b;
    /* wait for byte to be shifted out */
    while(!(SPSR & (1 << SPIF)));
    SPSR &= ~(1 << SPIF);
}

/** 
 * SPI read data - only one call so force inline 
 *
 * Receives nbytes in buf. Send 0xff all the time.
 */
//inline __attribute__((always_inline)) 
void SD_L0_SpiRecvBlock(uint8_t *buf, uint16_t nbyte) 
{
  if (nbyte-- == 0) return;
  SPDR = 0xFF;
  for (uint16_t i = 0; i < nbyte; i++) {
    while (!(SPSR & (1 << SPIF)));
    buf[i] = SPDR;
    SPDR = 0xFF;
  }
  while (!(SPSR & (1 << SPIF)));
  buf[nbyte] = SPDR;
}

/** 
 * SPI send block - only one call so force inline 
 *
 * Sends the token and 512 data bytes from buffer afterwards.
 */
//inline __attribute__((always_inline)) 
void SD_L0_SpiSendBlock(uint8_t token, const uint8_t *buf) 
{
  SPDR = token;
  for (uint16_t i = 0; i < 512; i += 2) {
    while (!(SPSR & (1 << SPIF)));
    SPDR = buf[i];
    while (!(SPSR & (1 << SPIF)));
    SPDR = buf[i + 1];
  }
  while (!(SPSR & (1 << SPIF)));
}

