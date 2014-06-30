/* Copyright (c) 2011 Frank Zhao
   All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:

   * Redistributions of source code must retain the above copyright
     notice, this list of conditions and the following disclaimer.
   * Redistributions in binary form must reproduce the above copyright
     notice, this list of conditions and the following disclaimer in the
     documentation and/or other materials provided with the distribution.
   * Neither the name of the authors nor the names of its contributors
     may be used to endorse or promote products derived from this software
     without specific prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
   AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
   IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
   ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
   LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
   CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
   SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
   INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
   CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
   ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
   POSSIBILITY OF SUCH DAMAGE. */

#ifndef ZigduinoRadioInC_h

#include "ZigduinoRadioCfg.h"

#include "radio.h"
#include "board.h"
#include "transceiver.h"

#define ZRC_RXFRMBUFF_SIZE MAX_FRAME_SIZE
#define ZRC_FIFO_SIZE 128 // size for the RX FIFO ring buffer
#define ZRC_TXTMPBUFF_SIZE MAX_FRAME_SIZE // size for the TX non-immediate transmit buffer

#define ZRC_TXWAIT_BEFORE // when you call any TX functions, it will wait until the previous transmission has finished before initiating a new transmission
#define ZRC_TXWAIT_AFTER // when you call any TX functions, it will transmit and then wait until that transmission finished

// function prototypes
void zr_begin(channel_t, uint8_t*);
void zr_setFrameHeader(uint8_t*);
void zr_attachError(void(*)(radio_error_t));
void zr_attachIrq(void(*)(uint8_t));
void zr_attachReceiveFrame(uint8_t* (*)(uint8_t, uint8_t*, uint8_t, uint8_t));
void zr_attachTxDone(void(*)(radio_tx_done_t));
int8_t zr_available();
int16_t zr_peek();
int16_t zr_read();
void zr_flush();
void zr_write(uint8_t);
void zr_writeStr(char*);
void zr_writeArr(uint8_t*, uint8_t);
void zr_send(uint8_t);
void zr_txFrame(uint8_t*, uint8_t);
void zr_beginTransmission();
void zr_endTransmission();
void zr_cancelTransmission();
void zr_setParam(radio_attribute_t, radio_param_t);
radio_cca_t zr_doCca();
void zr_setState(radio_state_t);
void zr_setStateRx();
void zr_setChannel(channel_t);
void zr_forceState(radio_state_t);
void zr_waitTxDone(uint16_t);
int8_t zr_getRssiNow();
int8_t zr_getLastRssi();
uint8_t zr_getLqi();
int8_t zr_getLastEd();
int8_t zr_getEdNow();

#define ZigduinoRadioInC_h
#endif