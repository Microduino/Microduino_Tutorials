/**
 * Copyright (c) 2011 Daniel Berenguer <dberenguer@usapiens.com>
 * 
 * swpacket.cpp
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

#include "swpacket.h"
#include "panstamp.h"

/**
 * SWPACKET
 * 
 * Class constructor
 * 
 * 'packet'	Raw CC1101 packet
 */
SWPACKET::SWPACKET(CCPACKET packet) 
{
  destAddr = packet.data[0];
  srcAddr = packet.data[1];
  hop = (packet.data[2] >> 4) & 0x0F;
  security = packet.data[2] & 0x0F;
  nonce = packet.data[3];
  function = packet.data[4];
  regAddr = packet.data[5];
  regId = packet.data[6];
  value.data = packet.data + 7;
  value.length = packet.length - SWAP_DATA_HEAD_LEN - 1;
}

/**
 * SWPACKET
 * 
 * Class constructor
 */
SWPACKET::SWPACKET(void) 
{
}

/**
 * send
 * 
 * Send SWAP packet. Do up to 5 retries if necessary
 *
 * Return:
 *  True if the transmission succeeds
 *  False otherwise
 */
boolean SWPACKET::send(void)
{
  CCPACKET packet;
  byte i;
  boolean res;

  packet.length = value.length + SWAP_DATA_HEAD_LEN + 1;
  packet.data[0] = destAddr;
  packet.data[1] = srcAddr;
  packet.data[2] = (hop << 4) & 0xF0;
  packet.data[2] |= security & 0x0F;
  packet.data[3] = nonce;
  packet.data[4] = function;
  packet.data[5] = regAddr;
  packet.data[6] = regId;

  for(i=0 ; i<value.length ; i++)
    packet.data[i+7] = value.data[i];

  i = SWAP_NB_TX_TRIES;
  while(!(res = panstamp.cc1101.sendData(packet)) && i>1)
  {
    i--;
    delay(SWAP_DY_TX_TRIES);
  }

  return res;
}

