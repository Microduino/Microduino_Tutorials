/**
 * nvolat.h
 * Copyright (c) 2011 Daniel Berenguer <dberenguer@usapiens.com>
 * 
 * This file is part of the panStamp project.
 * 
 * panStamp  is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
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
 * Creation date: 07/03/2011
 */

#ifndef _NVOLAT_H
#define _NVOLAT_H

#include "Arduino.h"
#include <EEPROM.h>

/**
 * EEPROM addresses
 */
#define EEPROM_FREQ_CHANNEL      0x00   // 1-byte register
#define EEPROM_SECU_OPTION       0x01   // 1-byte register
#define EEPROM_SYNC_WORD         0x02   // 2-byte register
#define EEPROM_DEVICE_ADDR       0x04   // 1-byte register
#define EEPROM_TX_INTERVAL       0x05   // 2-byte register

#define EEPROM_FIRST_CUSTOM      0x20

#endif
