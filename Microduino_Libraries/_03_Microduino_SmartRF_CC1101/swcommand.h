/**
 * Copyright (c) 2011 Daniel Berenguer <dberenguer@usapiens.com>
 * 
 * swcommand.h
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

#ifndef _SWCOMMAND_H
#define _SWCOMMAND_H

#include "Arduino.h"
#include "swpacket.h"


class SWCOMMAND : public SWPACKET
{
  public:
    /**
     * SWCOMMAND
     * 
     * Class constructor
     * 
     * 'destAddr'	Destination address
     * 'secNonce'	Security nonce
     * 'rAddr'	  Register address
     * 'rId'	    Register id
     * '*val'	    New value
     * 'len'	    Buffer length
     */
    SWCOMMAND(byte destAddr, byte secNonce, byte rAddr, byte rId, byte *val, byte len);
};
#endif
