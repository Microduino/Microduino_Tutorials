/**
 * Copyright (c) 2011 Daniel Berenguer <dberenguer@usapiens.com>
 * 
 * swstatus.cpp
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

#include "swstatus.h"
#include "panstamp.h"

/**
 * SWSTATUS
 * 
 * Class constructor
 * 
 * 'rId'	Register id
 * '*val'	New value
 * 'len'	Buffer length
 */
SWSTATUS::SWSTATUS(byte rId, byte *val, byte len) 
{
  destAddr = SWAP_BCAST_ADDR;
  srcAddr = panstamp.cc1101.devAddress;
  hop = 0;
  security = panstamp.security & 0x0F;
  nonce = ++panstamp.nonce;
  function = SWAPFUNCT_STA;
  regAddr = panstamp.cc1101.devAddress;
  regId = rId;
  value.length = len;
  value.data = val;
}

