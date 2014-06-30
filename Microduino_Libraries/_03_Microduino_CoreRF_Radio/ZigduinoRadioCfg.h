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

#ifndef ZigduinoRadioCfg_h

#define zigduino

//#define ENABLE_DIG3_DIG4 // this enables the DIG3 and DIG4 indicators by enabling PA_EXT_EN

// define the LED behaviour macros here
// leave them blank if you want to disable the LEDs
// PD5 and PD6 are used on the Zigduino
#define ZR_RFRX_LED_OUTPUT() do{DDRD|=_BV(6);}while(0)
#define ZR_RFTX_LED_OUTPUT() do{DDRD|=_BV(5);}while(0)
#define ZR_RFRX_LED_ON() do{PORTD|=_BV(6);}while(0)
#define ZR_RFTX_LED_ON() do{PORTD|=_BV(5);}while(0)
#define ZR_RFRX_LED_OFF() do{PORTD&=~_BV(6);}while(0)
#define ZR_RFTX_LED_OFF() do{PORTD&=~_BV(5);}while(0)

#define ZigduinoRadioCfg_h
#endif