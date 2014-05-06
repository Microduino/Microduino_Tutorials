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

#ifndef ZigduinoRadio_h

#ifdef __cplusplus
extern "C" {
#endif

#include "ZigduinoRadioCfg.h"

#include "radio.h"
#include "board.h"
#include "transceiver.h"

#ifdef __cplusplus
} /* extern "C" */
#endif

#define ZR_RXFRMBUFF_SIZE MAX_FRAME_SIZE
#define ZR_FIFO_SIZE 128 // size for the RX FIFO ring buffer
#define ZR_TXTMPBUFF_SIZE MAX_FRAME_SIZE // size for the TX non-immediate transmit buffer

#define ZR_TXWAIT_BEFORE // when you call any TX functions, it will wait until the previous transmission has finished before initiating a new transmission
#define ZR_TXWAIT_AFTER // when you call any TX functions, it will transmit and then wait until that transmission finished

// just a class definition, for usage and comments, see the cpp file
class cZigduinoRadio
{
	private:
		static uint8_t rxFrameBuffer[];
		static uint8_t rxRingBuffer[];
		static uint8_t rxRingBufferHead;
		static uint8_t rxRingBufferTail;
		static uint8_t txTmpBuffer[];
		static uint8_t txTmpBufferLength;
		static uint8_t lastLqi;
		static uint8_t lastRssi;
		static uint8_t hasAttachedRxEvent;
		static uint8_t hasAttachedTxEvent;
		static uint8_t usedBeginTransmission;
		volatile static uint8_t txIsBusy;
		
		static uint8_t* (*zrEventReceiveFrame)(uint8_t, uint8_t*, uint8_t, uint8_t);
		static void (*zrEventTxDone)(radio_tx_done_t);
		
		static uint8_t* onReceiveFrame(uint8_t, uint8_t*, uint8_t, uint8_t);
		static void onTxDone(radio_tx_done_t);
		
	public:
		cZigduinoRadio();
		void begin(channel_t);
		void begin(channel_t, uint8_t*);
		void setFrameHeader(uint8_t*);
		void attachError(void(*)(radio_error_t));
		void attachIrq(void(*)(uint8_t));
		void attachReceiveFrame(uint8_t* (*)(uint8_t, uint8_t*, uint8_t, uint8_t));
		void attachTxDone(void(*)(radio_tx_done_t));
		int8_t available();
		int16_t peek();
		int16_t read();
		void flush();
		void write(uint8_t);
		void write(char*);
		void write(uint8_t*, uint8_t);
		void send(uint8_t);
		void txFrame(uint8_t*, uint8_t);
		void beginTransmission();
		void endTransmission();
		void cancelTransmission();
		void setParam(radio_attribute_t, radio_param_t);
		radio_cca_t doCca();
		void setState(radio_state_t, uint8_t);
		void setState(radio_state_t);
		void setStateRx();
		void setChannel(channel_t);
		void forceState(radio_state_t);
		void waitTxDone(uint16_t);
		int8_t getRssiNow();
		int8_t getLastRssi();
		uint8_t getLqi();
		int8_t getLastEd();
		int8_t getEdNow();
};

extern cZigduinoRadio ZigduinoRadio; // make single instance accessible

#define ZigduinoRadio_h
#endif