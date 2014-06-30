/**
 * panstamp.cpp
 *
 * Copyright (c) 2011 Daniel Berenguer <dberenguer@usapiens.com>
 * 
 * This file is part of the panStamp project.
 * 
 * panStamp  is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * any later version.
 * 
 * panLoader is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with panLoader; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 
 * USA
 * 
 * Author: Daniel Berenguer
 * Creation date: 03/03/2011
 */

#include "panstamp.h"
#include "commonregs.h"

#define enableIRQ_GDO0()        attachInterrupt(0, isrGDO0event, FALLING);
#define disableIRQ_GDO0()       detachInterrupt(0);

DEFINE_COMMON_REGINDEX_START()
DEFINE_COMMON_REGINDEX_END()

/**
 * Array of registers
 */
extern REGISTER* regTable[];
extern byte regTableSize;

/**
 * PANSTAMP
 *
 * Class constructor
 */
PANSTAMP::PANSTAMP(void)
{
  statusReceived = NULL;
}

/**
 * getRegister
 *
 * Return pointer to register with ID = regId
 *
 * 'regId'  Register ID
 */
REGISTER * getRegister(byte regId)
{
  if (regId >= regTableSize)
    return NULL;

  return regTable[regId]; 
}

/**
 * isrGDO0event
 *
 * Event on GDO0 pin (INT0)
 */
void isrGDO0event(void)
{
  CCPACKET ccPacket;
  SWPACKET swPacket;
  REGISTER *reg;

  // Disable interrupt
  disableIRQ_GDO0();

  if (panstamp.cc1101.receiveData(&ccPacket) > 0)
  {
    if (ccPacket.crc_ok)
    {
      swPacket = SWPACKET(ccPacket);
      // Function
      switch(swPacket.function)
      {
        case SWAPFUNCT_CMD:
          // Broadcasted commands are not allowed
          if (swPacket.destAddr == SWAP_BCAST_ADDR)
            break;
          // Current version does not support data recording mode
          // so destination address and register address must be the same
          if (swPacket.destAddr != swPacket.regAddr)
            break;
          // Valid register?
          if ((reg = getRegister(swPacket.regId)) == NULL)
            break;
          // Filter incorrect data lengths
          if (swPacket.value.length == reg->length)
            reg->setData(swPacket.value.data);
          else
            reg->sendSwapStatus();
          break;
        case SWAPFUNCT_QRY:
          // Only Product Code can be broadcasted
          if (swPacket.destAddr == SWAP_BCAST_ADDR)
          {
            if (swPacket.regId != REGI_PRODUCTCODE)
              break;
          }
          // Current version does not support data recording mode
          // so destination address and register address must be the same
          if (swPacket.destAddr != swPacket.regAddr)
            break;
          // Valid register?
          if ((reg = getRegister(swPacket.regId)) == NULL)
            break;
          reg->getData();
          break;
        case SWAPFUNCT_STA:
          // User callback function declared?
          if (panstamp.statusReceived != NULL)
            panstamp.statusReceived(&swPacket);
          break;
        default:
          break;
      }
    }
  }
  // Enable interrupt
  enableIRQ_GDO0();
}

/**
 * ISR(WDT_vect)
 *
 * Watchdog ISR. Called whenever a watchdog interrupt occurs
 */
ISR(WDT_vect)
{
}

/**
 * setup_watchdog
 * 
 * 'time'	Watchdog timer value
 */
void PANSTAMP::setup_watchdog(byte time) 
{
  byte bb;

  bb = time & 7;
  if (time > 7)
    bb|= (1<<5);

  bb|= (1<<WDCE);

  MCUSR &= ~(1<<WDRF);
  // start timed sequence
  WDTCSR |= (1<<WDCE) | (1<<WDE);
  // set new watchdog timeout value
  WDTCSR = bb;
  WDTCSR |= _BV(WDIE);    // Enable Watchdog interrupt
}

/**
 * init
 * 
 * Initialize panStamp board
 */
void PANSTAMP::init() 
{
  // Setup CC1101
  cc1101.init();

  // Read security option byte from EEPROM
  security = EEPROM.read(EEPROM_SECU_OPTION);
  // Read periodic Tx interval from EEPROM
  txInterval[0] = EEPROM.read(EEPROM_TX_INTERVAL);
  txInterval[1] = EEPROM.read(EEPROM_TX_INTERVAL + 1);

  delayMicroseconds(50);  

  // Enter RX state
  cc1101.setRxState();

  // Attach callback function for GDO0 (INT0)
  enableIRQ_GDO0();

  // Default values
  nonce = 0xFF;
  systemState = SYSTATE_RXON;
}

/**
 * reset
 * 
 * Reset panStamp
 */
void PANSTAMP::reset() 
{
  // Tell the network that our panStamp is restarting
  systemState = SYSTATE_RESTART;
  getRegister(REGI_SYSSTATE)->sendSwapStatus();

  // Reset panStamp
  wdt_disable();  
  wdt_enable(WDTO_15MS);
  while (1) {}
}

/**
 * sleepWd
 * 
 * Put panStamp into Power-down state during "time".
 * This function uses the internal watchdog timer in order to exit (interrupt)
 * from the power-down state
 * 
 * 'time'	Sleeping time:
 *  WDTO_15MS  = 15 ms
 *  WDTO_30MS  = 30 ms
 *  WDTO_60MS  = 60 ms
 *  WDTO_120MS  = 120 ms
 *  WDTO_250MS  = 250 ms
 *  WDTO_500MS  = 500 ms
 *  WDTO_1S = 1 s
 *  WDTO_2S = 2 s
 *  WDTO_4S = 4 s
 *  WDTO_8S = 8 s
 */
void PANSTAMP::sleepWd(byte time) 
{
  // Power-down CC1101
  cc1101.setPowerDownState();
  // Power-down panStamp
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();
  setup_watchdog(time);
  delayMicroseconds(10);
  // Disable ADC
  ADCSRA &= ~(1 << ADEN);
  // Unpower functions
  PRR = 0xFF;
  //power_all_disable();
  //clock_prescale_set(clock_div_8);
  // Enter sleep mode
  sleep_mode();

  // ZZZZZZZZ...

  // Wake-up!!
  wakeUp();
}

/**
 * wakeUp
 *
 * Wake from sleep mode
 */
void PANSTAMP::wakeUp(void) 
{
  // Exit from sleep
  sleep_disable();
//  wdt_disable();
  // Re-enable functions
  //clock_prescale_set(clock_div_1);
  power_all_enable();
  // Enable ADC
  ADCSRA |= (1 << ADEN);

  // Reset CC1101 IC
  cc1101.wakeUp();
}

/**
 * goToSleep
 *
 * Sleep whilst in power-down mode. This function currently uses sleepWd in a loop
 */
void PANSTAMP::goToSleep(void)
{
  // Get the amount of seconds to sleep from the internal register
  int intInterval = txInterval[0] * 0x100 + txInterval[1];
  int i, loops;
  byte minTime;
  
  // No interval? Then return
  if (intInterval == 0)
    return;

  // Search the maximum sleep time passed as argument to sleepWd that best
  // suits our desired interval
  if (intInterval % 8 == 0)
  {
    loops = intInterval / 8;
    minTime = WDTO_8S;
  }
  else if (intInterval % 4 == 0)
  {
    loops = intInterval / 4;
    minTime = WDTO_4S;
  }
  else if (intInterval % 2 == 0)
  {
    loops = intInterval / 2;
    minTime = WDTO_2S;
  }
  else
  {
    loops = intInterval;
    minTime = WDTO_1S;
  }

  // Sleep
  for (i=0 ; i<loops ; i++)
    sleepWd(minTime);
}

/**
 * enterSystemState
 *
 * Enter system state
 *
 * 'state'  New system state
 */
void PANSTAMP::enterSystemState(SYSTATE state)
{
  // Enter SYNC mode (full Rx mode)
  byte newState[] = {state};
  regTable[REGI_SYSSTATE]->setData(newState);
}

/**
 * getInternalTemp
 * 
 * Read internal (ATMEGA328 only) temperature sensor
 * 
 * Return:
 * 	Temperature in milli-degrees Celsius
 */
long PANSTAMP::getInternalTemp(void) 
{
  long result;

  // Read temperature sensor against 1.1V reference
  ADMUX = _BV(REFS1) | _BV(REFS0) | _BV(MUX3);
  delay(2); // Wait for Vref to settle
  ADCSRA |= _BV(ADSC); // Convert
  while (bit_is_set(ADCSRA,ADSC));
  result = ADCL;
  result |= ADCH<<8;
  result = (result - 125) * 1075;

  return result;
}

/**
 * setSecurity
 * 
 * Set security option
 * 
 * 'secu'	New option
 * 'save' If TRUE, save parameter in EEPROM
 */
void PANSTAMP::setSecurity(byte secu, bool save)
{
  if (security != secu)
  {
    security = secu;
    // Save in EEPROM
    if (save)
      EEPROM.write(EEPROM_SECU_OPTION, secu);
  }
}

/**
 * setTxInterval
 * 
 * Set interval for periodic transmissions
 * 
 * 'interval'	New periodic interval. 0 for asynchronous devices
 * 'save'     If TRUE, save parameter in EEPROM
 */
void PANSTAMP::setTxInterval(byte* interval, bool save)
{
  memcpy(txInterval, interval, sizeof(txInterval));

  // Save in EEPROM
  if (save)
  {
    EEPROM.write(EEPROM_TX_INTERVAL, interval[0]);
    EEPROM.write(EEPROM_TX_INTERVAL + 1, interval[1]);
  }
}

/**
 * Pre-instantiate PANSTAMP object
 */
PANSTAMP panstamp;

