/**
 * register.h
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
 * Creation date: 04/24/2011
 */

#ifndef _REGISTER_H
#define _REGISTER_H

#include "Arduino.h"

extern byte regIndex;

/**
 * Class: REGISTER
 * 
 * Description:
 * Register class
 */
class REGISTER
{
  private:
    /**
     * Pointer to the register "updater" function
     *
     *  'rId'  Register ID     
     */
    const void (*updateValue)(byte rId);

    /**
     * Pointer to the register "setter" function
     *
     *  'rId'  Register ID     
     *  'v'    New register value
     */
    const void (*setValue)(byte rId, byte *v);

  public:
    /**
     * Register id
     */
    const byte id;
    
    /**
     * Register value
     */
    byte *value;
    
    /**
     * Data length
     */
    const byte length;

    /**
     * REGISTER
     * 
     * Constructor
     * 
     * 'val'	      Pointer to the register value
     * 'len'	      Length of the register value
     * 'getValH'    Pointer to the getter function
     * 'setValH'    Pointer to the setter function
     */
    REGISTER(byte *val, const byte len, const void (*updateValH)(byte rId), const void (*setValH)(byte rId, byte *v)):id(regIndex++), value(val), length(len), updateValue(updateValH), setValue(setValH) {};

    /**
     * getData
     * 
     * Update and get register value
     * 
     */
    void getData();

    /**
     * setData
     * 
     * Set register value
     * 
     * 'data'	New register value
     */
    void setData(byte *data);

    /**
     * sendSwapStatus
     * 
     * Send SWAP status message
     */
    void sendSwapStatus(void);
};

/**
 * Array of registers
 */
extern REGISTER* regTable[];

/**
 * Extern global functions
 */
extern void setupRegisters(void);

#endif

