/**
 * panstamp.h
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

#ifndef _PANSTAMP_H
#define _PANSTAMP_H

#include "Arduino.h"
#include "EEPROM.h"
#include "cc1101.h"
#include "nvolat.h"
#include "register.h"
#include "swpacket.h"
#include <avr/wdt.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <avr/power.h>

/**
 * Macros
 */
#define setSwapStatusCallBack(ptrFunc)   statusReceived = ptrFunc

#define eepromToFactoryDefaults()                             \
  EEPROM.write(EEPROM_SYNC_WORD, CC1101_DEFVAL_SYNC1);        \
  EEPROM.write(EEPROM_SYNC_WORD + 1, CC1101_DEFVAL_SYNC0);    \
  EEPROM.write(EEPROM_DEVICE_ADDR, CC1101_DEFVAL_ADDR);       \
  EEPROM.write(EEPROM_FREQ_CHANNEL, CC1101_DEFVAL_CHANNR);    \
  EEPROM.write(EEPROM_SECU_OPTION, 0);                        \
  EEPROM.write(EEPROM_TX_INTERVAL, 0xFF);                     \
  EEPROM.write(EEPROM_TX_INTERVAL + 1, 0xFF)

/**
 * System states
 */
enum SYSTATE
{
  SYSTATE_RESTART = 0,
  SYSTATE_RXON,
  SYSTATE_RXOFF,
  SYSTATE_SYNC,
  SYSTATE_LOWBAT
};

/**
 * Class: PANSTAMP
 * 
 * Description:
 * panStamp main class
 */
class PANSTAMP
{
  private:
    /**
     * setup_watchdog
     * 
     * 'time'	Watchdog timer value
     */
    void setup_watchdog(byte time);

  public:
    /**
     * CC1101 radio interface
     */
    CC1101 cc1101;
    
    /**
     * Security options
     */
    byte security;

    /**
     * Security cyclic nonce
     */
    byte nonce;
    
    /**
     * System state
     */
    byte systemState;

    /**
     * Interval between periodic transmissions. 0 for asynchronous transmissions
     */
    byte txInterval[2];
 
    /**
     * SWAP status packet received. Callaback function
     */
    void (*statusReceived)(SWPACKET *status);

    /**
     * PANSTAMP
     *
     * Class constructor
     */
    PANSTAMP(void);

    /**
     * init
     * 
     * Initialize panStamp board
     */
    void init(void);

    /**
     * reset
     * 
     * Reset panStamp
     */
    void reset(void);

    /**
     * sleepWd
     * 
     * Put panStamp into Power-down state during "time".
     * This function uses the internal watchdog timer in order to exit (interrupt)
     * from the power-doen state
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
    void sleepWd(byte time);

    /**
     * wakeUp
     *
     * Wake from sleep mode
     */
    void wakeUp(void);

    /**
     * goToSleep
     *
     * Sleep whilst in power-down mode. This function currently uses sleepWd in a loop
     */
    void goToSleep(void);

    /**
     * enterSystemState
     *
     * Enter system state
     *
     * 'state'  New system state
     */
    void enterSystemState(SYSTATE state);

    /**
     * getInternalTemp
     * 
     * Read internal (ATMEGA328 only) temperature sensor
     * 
     * Return:
     * 	Temperature in milli-degrees Celsius
     */
    long getInternalTemp(void);

    /**
     * setSecurity
     * 
     * Set security option
     * 
     * 'secu'	New option
     * 'save' If TRUE, save parameter in EEPROM
     */
    void setSecurity(byte secu, bool save);

    /**
     * setTxInterval
     * 
     * Set interval for periodic transmissions
     * 
     * 'interval'	New periodic interval. 0 for asynchronous devices
     * 'save'     If TRUE, save parameter in EEPROM
     */
    void setTxInterval(byte* interval, bool save);
};

/**
 * Global PANSTAMP object
 */
extern PANSTAMP panstamp;

/**
 * getRegister
 *
 * Return pointer to register with ID = regId
 *
 * 'regId'  Register ID
 */
REGISTER * getRegister(byte regId);

#endif

