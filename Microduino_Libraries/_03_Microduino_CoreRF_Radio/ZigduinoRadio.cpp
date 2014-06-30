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
   
#include "ZigduinoRadio.h"
#include "ZigduinoRadioEvents.h"

// this is used as the main buffer for all received data frames
uint8_t cZigduinoRadio::rxFrameBuffer[ZR_RXFRMBUFF_SIZE];

// this is a ring FIFO buffer, used for the read/peek/available/flush functions
uint8_t cZigduinoRadio::rxRingBuffer[ZR_FIFO_SIZE];
uint8_t cZigduinoRadio::rxRingBufferHead = 0;
uint8_t cZigduinoRadio::rxRingBufferTail = 0;

// this is a temporary storage buffer for use with non-immediate transmission functions
uint8_t cZigduinoRadio::txTmpBuffer[ZR_TXTMPBUFF_SIZE];
uint8_t cZigduinoRadio::txTmpBufferLength = 0;

// these two stores the last link quality
uint8_t cZigduinoRadio::lastLqi = 0;

// these are used to indicate whether or not the user has attached event handlers
uint8_t cZigduinoRadio::hasAttachedRxEvent = 0;
uint8_t cZigduinoRadio::hasAttachedTxEvent = 0;

// the write function checks if beginTransmission was called prior to write
// in order to determine whether to use immediate or non-immediate transmission
uint8_t cZigduinoRadio::usedBeginTransmission = 0;

// a busy indicator so transmit functions can wait until the last transmission has finished
volatile uint8_t cZigduinoRadio::txIsBusy = 0;

// function pointers for events
uint8_t* (*cZigduinoRadio::zrEventReceiveFrame)(uint8_t, uint8_t*, uint8_t, uint8_t);
void (*cZigduinoRadio::zrEventTxDone)(radio_tx_done_t);

// empty constructor, should not be called by user
cZigduinoRadio::cZigduinoRadio()
{
	// default event handlers
	user_radio_error = 0;
	user_radio_irq = 0;
	zr_attach_receive_frame(onReceiveFrame);
	zr_attach_tx_done(onTxDone);
}

/**
 * @brief Radio Initialization
 *
 * The function initializes all IO ressources,
 * needed for the usage of the radio and performs
 * a reset to the radio.
 * It prepares the frame header.
 * Then it sets the channel number and defaults to RX state.
 *
 * @param chan the channel number for the radio to use, 11 to 26
 */
void cZigduinoRadio::begin(channel_t chan)
{
	begin(chan, 0);
}

/**
 * @brief Radio Initialization
 *
 * Overload for radio initalization that allows for the user to set a custom frame header
 *
 * @param chan channel number for the radio to use, 11 to 26
 * @param frameHeader 7 byte custom frame header
 */
void cZigduinoRadio::begin(channel_t chan, uint8_t* frameHeader)
{
	radio_init(rxFrameBuffer, MAX_FRAME_SIZE);
	
	if (frameHeader)
	{
		// copy custom frame header
		int i;
		for (i = 0; i < 7; i++)
			txTmpBuffer[i] = frameHeader[i];
	}
	else
	{
		// fixed frame header
		txTmpBuffer[0] = 0x01; txTmpBuffer[1] = 0x80; txTmpBuffer[2] =  0; txTmpBuffer[3] = 0x11; txTmpBuffer[4] = 0x22; txTmpBuffer[5] = 0x33; txTmpBuffer[6] = 0x44;
	}
	
	// set the channel
	radio_set_param(RP_CHANNEL(chan));
	
	// default to receiver
	radio_set_state(STATE_RX);
	
	#ifdef ENABLE_DIG3_DIG4
	trx_bit_write(SR_PA_EXT_EN, 1);
	#endif
	
	ZR_RFRX_LED_OUTPUT();
	ZR_RFTX_LED_OUTPUT();
	ZR_RFRX_LED_OFF();
	ZR_RFTX_LED_OFF();
}

/**
 * @brief Set Frame Header
 *
 * changes the custom frame header
 *
 * @param frameHeader 7 byte custom frame header
 */
void cZigduinoRadio::setFrameHeader(uint8_t* frameHeader)
{
	// copy custom frame header
	int i;
	for (i = 0; i < 7; i++)
		txTmpBuffer[i] = frameHeader[i];
}

/**
 * @brief Attach Error Event Handler
 *
 * Allows the user to set a error handling function
 * An empty one is used if none is set
 *
 * @param funct the function pointer to the event handler
 */
void cZigduinoRadio::attachError(void (*funct)(radio_error_t))
{
	user_radio_error = funct;
}

/**
 * @brief Attach IRQ Event Handler
 *
 * Allows the user to set a IRQ handling function
 * An empty one is used if none is set
 *
 * @param funct the function pointer to the event handler
 */
void cZigduinoRadio::attachIrq(void (*funct)(uint8_t))
{
	user_radio_irq = funct;
}

/**
 * @brief Attach RX Event Handler
 *
 * Allows the user to set a RX handling function
 * The default handler will read in the received frame and place it into the RX FIFO ring buffer
 * If the user chooses to use this attach function, then the default handler will not be used
 * This means read/peek/available/flush will stop working
 *
 * @param funct the function pointer to the event handler
 */
void cZigduinoRadio::attachReceiveFrame(uint8_t* (*funct)(uint8_t, uint8_t*, uint8_t, uint8_t))
{
	zrEventReceiveFrame = funct;
	hasAttachedRxEvent = (funct == 0) ? 0 : 1;
}

/**
 * @brief Attach TX Complete Event Handler
 *
 * Allows the user to set a TX complete handling function
 * An empty one is used if none is set
 * The event will occur before the busy flag is reset and returning to RX state
 *
 * @param funct the function pointer to the event handler
 */
void cZigduinoRadio::attachTxDone(void (*funct)(radio_tx_done_t))
{
	zrEventTxDone = funct;
	hasAttachedTxEvent = (funct == 0) ? 0 : 1;
}

/**
 * @brief Default RX Event Handler
 *
 * If the user has not attached a custom event handler, then the bytes are placed into the FIFO ring buffer
 * If the frame contains a header, then the frame header is ignored, only the payload is read
 * The above does not occur if a user handler is attached, which will be called instead
 * The LQI and RSSI is always remembered
 *
 * This should not be called by the user
 *
 * @param len length of the frame
 * @param frm array containing frame data
 * @param lqi link quality indicator
 * @param crc_fail boolean indicating whether the received frame failed FCS verification, not used
 */
uint8_t* cZigduinoRadio::onReceiveFrame(uint8_t len, uint8_t* frm, uint8_t lqi, uint8_t crc_fail)
{
	lastLqi = lqi;
	if (hasAttachedRxEvent == 0)
	{
		// no event handler, so write it into the FIFO
		
		if (len >= 8)
		{
			// frame header exists
			// copy only payload
			
			for (uint8_t i = 7; i < len - 2; i++)
			{
				uint16_t j = ((uint16_t)((uint16_t)rxRingBufferHead + 1)) % ZR_FIFO_SIZE;
				if (j != rxRingBufferTail)
				{
					// push into FIFO
					rxRingBuffer[rxRingBufferHead] = frm[i];
					rxRingBufferHead = j;
				}
				else
				{
					// FIFO full
					break;
				}
			}
		}
		else
		{
			// frame header does not exist
			// copy everything
			
			for (uint8_t i = 0; i < len; i++)
			{
				uint16_t j = ((uint16_t)((uint16_t)rxRingBufferHead + 1)) % ZR_FIFO_SIZE;
				if (j != rxRingBufferTail)
				{
					// push into FIFO
					rxRingBuffer[rxRingBufferHead] = frm[i];
					rxRingBufferHead = j;
				}
				else
				{
					// FIFO full
					break;
				}
			}
		}
		
		return rxRingBuffer;
	}
	else
	{
		// user event is attached so call it
		return zrEventReceiveFrame(len, frm, lqi, crc_fail);
	}
}

/**
 * @brief RX Buffer Flush
 *
 * Flush the RX FIFO ring buffer
 */
void cZigduinoRadio::flush()
{
	rxRingBufferHead = rxRingBufferTail;
}

/**
 * @brief RX Buffer Read
 *
 * pops and returns the next byte from the FIFO ring buffer
 *
 * @return the next byte, or -1 if buffer is empty
 */
int16_t cZigduinoRadio::read()
{
	// if the head isn't ahead of the tail, we don't have any characters
	if (rxRingBufferHead == rxRingBufferTail)
	{
		return -1;
	}
	else
	{
		uint8_t c = rxRingBuffer[rxRingBufferTail];
		rxRingBufferTail = (rxRingBufferTail + 1) % ZR_FIFO_SIZE; // pop
		return c;
	}
}

/**
 * @brief RX Buffer Peek
 *
 * returns the next byte from the FIFO ring buffer without removing it
 *
 * @return the next byte, or -1 if buffer is empty
 */
int16_t cZigduinoRadio::peek()
{
	// if the head isn't ahead of the tail, we don't have any characters
	if (rxRingBufferHead == rxRingBufferTail)
	{
		return -1;
	}
	else
	{
		uint8_t c = rxRingBuffer[rxRingBufferTail];
		return c;
	}
}

/**
 * @brief RX Buffer Size
 *
 * Shows how many bytes are in the RX FIFO ring buffer
 *
 * @return how many bytes are in the RX FIFO ring buffer
 */
int8_t cZigduinoRadio::available()
{
	return ((int16_t)((int16_t)ZR_FIFO_SIZE + (int16_t)rxRingBufferHead - (int16_t)rxRingBufferTail)) % ZR_FIFO_SIZE;
}

/**
 * @brief Raw Frame Transmit
 *
 * Transmits a frame
 * Warning: no frame header or FCS is added
 *
 * @param frm array containing frame data
 * @param len length of the frame
 */
void cZigduinoRadio::txFrame(uint8_t* frm, uint8_t len)
{
	#ifdef ZR_TXWAIT_BEFORE
	waitTxDone(0xFFFF);
	#endif
	txIsBusy = 1;
	radio_set_state(STATE_TX);
	ZR_RFTX_LED_ON();
	radio_send_frame(len, frm, 0);
	#ifdef ZR_TXWAIT_AFTER
	waitTxDone(0xFFFF);
	radio_set_state(STATE_RX);
	txIsBusy = 0;
	#endif
}

/**
 * @brief Prepare for Trasmission
 *
 * Goes into non-immediate transmit mode, resets the transmit payload
 * Non-immediate mode sends multiple bytes per frame
 *
 */
void cZigduinoRadio::beginTransmission()
{
	usedBeginTransmission = 1;
	
	// add frame header
	txTmpBufferLength = 7;
}

/**
 * @brief Finalize Trasmission
 *
 * Finalize the payload and transmits it when ready
 *
 */
void cZigduinoRadio::endTransmission()
{
	usedBeginTransmission = 0;
	
	// empty FCS field
	txTmpBufferLength += 2;
	
	#ifdef ZR_TXWAIT_BEFORE
	waitTxDone(0xFFFF);
	#endif
	txIsBusy = 1;
	radio_set_state(STATE_TX);
	ZR_RFTX_LED_ON();
	radio_send_frame(txTmpBufferLength, txTmpBuffer, 0);
	#ifdef ZR_TXWAIT_AFTER
	waitTxDone(0xFFFF);
	radio_set_state(STATE_RX);
	txIsBusy = 0;
	#endif
}

/**
 * @brief Cancel Trasmission
 *
 * Clears payload buffer
 *
 * Warning: does not actually cancel a transmission in progress
 *
 */
void cZigduinoRadio::cancelTransmission()
{
	usedBeginTransmission = 0;
	
	// add frame header
	txTmpBufferLength = 7;
}

/**
 * @brief TX a Byte
 *
 * Wrapper for "write", since the "Wire" library uses "send"
 *
 */
void cZigduinoRadio::send(uint8_t c)
{
	write(c);
}

/**
 * @brief TX a Byte
 *
 * If "beginTrasmission" was used, then it writes into the transmit buffer for non-immediate mode
 * If "beginTrasmission" was not used, then it transmits the single byte immediately (slower for multiple bytes)
 *
 * @param c character to be sent
 */
void cZigduinoRadio::write(uint8_t c)
{
	if (usedBeginTransmission)
	{
		if (txTmpBufferLength < ZR_TXTMPBUFF_SIZE - 2)
		{
			txTmpBuffer[txTmpBufferLength] = c;
			txTmpBufferLength++;
			
			if (txTmpBufferLength >= ZR_TXTMPBUFF_SIZE - 2)
			{
				// buffer is now full
				// just send it all out so we have more room
				endTransmission();
				beginTransmission();
			}
		}
	}
	else
	{
		txTmpBuffer[7] = c; // set payload
		txTmpBuffer[8] = 0; // empty FCS
		txTmpBuffer[9] = 0; // empty FCS
		
		#ifdef ZR_TXWAIT_BEFORE
		waitTxDone(0xFFFF);
		#endif
		txIsBusy = 1;
		radio_set_state(STATE_TX);
		ZR_RFTX_LED_ON();
		radio_send_frame(10, txTmpBuffer, 0);
		#ifdef ZR_TXWAIT_AFTER
		waitTxDone(0xFFFF);
		radio_set_state(STATE_RX);
		txIsBusy = 0;
		#endif
	}
}

/**
 * @brief TX a String
 *
 * A overload for "write" that sends a null-terminated string
 *
 * @param str null-terminated string to be sent
 */
void cZigduinoRadio::write(char* str)
{
	while (*str)
		write(*str++);
}

/**
 * @brief TX an Array
 *
 * A overload for "write" that sends an array
 *
 * @param arr data array to be sent
 * @param len length of data array
 */
void cZigduinoRadio::write(uint8_t* arr, uint8_t len)
{
	uint8_t i;
	for (i = 0; i < len; i++)
		write(arr[i]);
}

/**
 * @brief Default TX Complete Event Handler
 *
 * Calls the user event function if one is attached
 * Clear the TX busy status flag
 * Defaults back to RX state
 *
 * this should not be called by the user
 *
 * @param x one of the radio_tx_done_t enumerations indicating transmission success
 */
void cZigduinoRadio::onTxDone(radio_tx_done_t x)
{
	if (hasAttachedTxEvent)
	{
		zrEventTxDone(x);
	}
	
	txIsBusy = 0;
}

/**
 * @brief radio_set_param Wrapper
 *
 * set a radio parameter
 *
 * see radio.h for more info
 */
void cZigduinoRadio::setParam(radio_attribute_t attr, radio_param_t parm)
{
	radio_set_param(attr, parm);
}

/**
 * @brief radio_do_cca Wrapper
 *
 * perform CCA measure
 *
 * see radio.h for more info
 */
radio_cca_t cZigduinoRadio::doCca()
{
	return radio_do_cca();
}

/**
 * @brief Set Radio State Wrapper
 *
 * sets or forces the radio into a state
 *
 * see radio.h for more info
 *
 * @param state requested radio state
 * @param force boolean indicating to force the state
 */
void cZigduinoRadio::setState(radio_state_t state, uint8_t force)
{
	if (force)
		radio_force_state(state);
	else
		radio_set_state(state);
}

/**
 * @brief radio_set_state Wrapper
 *
 * bring the the radio in the requested state
 *
 * see radio.h for more info
 */
void cZigduinoRadio::setState(radio_state_t state)
{
	radio_set_state(state);
}

/**
 * @brief radio_set_state to STATE_RX
 *
 * bring the the radio in the requested state of STATE_RX
 *
 * the radio state does not return to STATE_RX automatically if ZR_TXWAIT_AFTER is not used
 * thus why this is provided
 *
 * see radio.h for more info
 */
void cZigduinoRadio::setStateRx()
{
	radio_set_state(STATE_RX);
}

/**
 * @brief radio_force_state Wrapper
 *
 * force the radio to the requested state
 *
 * see radio.h for more info
 */
void cZigduinoRadio::forceState(radio_state_t state)
{
	radio_force_state(state);
}

/**
 * @brief Sets Radio Channel
 *
 * changes the radio channel by setting the radio channel state
 *
 * @param chan channel number, 11 to 26
 */
void cZigduinoRadio::setChannel(channel_t chan)
{
	radio_set_param(RP_CHANNEL(chan));
}

/**
 * @brief Read Receiver Signal Strength Indicator Now
 *
 * returns the current RSSI
 *
 * range is between -91 and -9 dBm
 * where -9 is the best
 *
 * @return RSSI of the last transmission received
 */
int8_t cZigduinoRadio::getRssiNow()
{
    int16_t rssi = ((int16_t)(trx_reg_read(RG_PHY_RSSI) & 0x1F)); // mask only important bits
    rssi = (rssi == 0) ? (RSSI_BASE_VAL - 1) : ((rssi < 28) ? ((rssi - 1) * 3 + RSSI_BASE_VAL) : -9);
    // if 0, then rssi < -90, if 28, then >= -10
	
	return rssi;
}

// this provides access to the temporary RSSI global inside radio_rfa.c
extern "C" {
extern uint8_t temprssi;
}

/**
 * @brief Read Last Receiver Signal Strength Indicator
 *
 * returns the RSSI of the last transmission
 *
 * range is between -91 and -9 dBm
 * where -9 is the best
 *
 * @return RSSI of the last transmission received
 */
int8_t cZigduinoRadio::getLastRssi()
{
    int16_t rssi = ((int16_t)(temprssi & 0x1F)); // mask only important bits
    rssi = (rssi == 0) ? (RSSI_BASE_VAL - 1) : ((rssi < 28) ? ((rssi - 1) * 3 + RSSI_BASE_VAL) : -9);
    // if 0, then rssi < -90, if 28, then >= -10
	
	return rssi;
}

/**
 * @brief Read Link Quality Indicator
 *
 * returns the LQI of the last transmission received
 *
 * range is from 0 to 255
 * 255 is the best
 *
 * @return LQI of the last transmission received
 */
uint8_t cZigduinoRadio::getLqi()
{
	return lastLqi;
}

/**
 * @brief Read Last Energy Detection Level
 *
 * returns the ED level of the last transmission received
 *
 * range is between -90 and -6 dBm
 * where -6 is the best
 *
 * @return ED level of the last transmission received
 */
int8_t cZigduinoRadio::getLastEd()
{
    int8_t ed = trx_reg_read(RG_PHY_ED_LEVEL);
	
	return ed == 0xFF ? 0 : (ed + RSSI_BASE_VAL);
}

/**
 * @brief Read Energy Detection Level Now
 *
 * forces a reading of the ED level
 *
 * range is between -90 and -6 dBm
 * where -6 is the best
 *
 * @return ED level
 */
int8_t cZigduinoRadio::getEdNow()
{
    trx_reg_write(RG_PHY_ED_LEVEL, 0); // forces a reading
	
	return getLastEd();
}

/**
 * @brief Wait for TX to Complete
 *
 * waits until the last transmission is complete, or timed out
 *
 * @param timeout iterations to countdown before timeout
 */
inline void cZigduinoRadio::waitTxDone(uint16_t timeout)
{
	volatile uint16_t cnt = timeout;
	while (txIsBusy && cnt--);
}

// create single instance that is accessible to the user
cZigduinoRadio ZigduinoRadio = cZigduinoRadio();