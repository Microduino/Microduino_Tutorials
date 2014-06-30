/* Copyright (c) 2009 Axel Wachtler
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

/* $Id: board_derfa.h,v 1.12 2011/01/09 14:23:18 awachtler Exp $ */
/**
 * @file
 * @brief Dresden Elektronik Radio Module deRFmega128-22A001
 * 
 *
 *
<pre>

Fuses/Locks:
     LF: 0xe2 - 8MHz internal RC Osc.
     HF: 0x11 - without boot loader
     HF: 0x10 - with boot loader
     EF: 0xff
     LOCK: 0xef - protection of boot section

Bootloader:
    Start at byte=0x1e000, address=0xf000, size = 4096 instructions/ 8192 bytes


funkomat:
  LEDS: PG1, PG2

</pre>


@par Build Options

  - derfa1 : Radio Module deRFmega128-22A001

  - funkomat : wireless Ardino board
  - funkomat_v1 : wireless Ardino board

  - zigduino : Logos Electromechanical LLC
               http://www.logos-electro.com/blog/2010/5/16/zigduino.html
 */

#ifndef BOARD_DERFA_H
#define BOARD_DERFA_H

#if defined(derfa1)
# define BOARD_TYPE BOARD_DERFA1
# define BOARD_NAME "derfa1"
#elif defined(funkomat)
# define BOARD_TYPE BOARD_FUNKOMAT
# define BOARD_NAME "funkomat"
#elif defined(funkomat_v1)
# define BOARD_TYPE BOARD_FUNKOMAT_V1
# define BOARD_NAME "funkomat v1"
#elif defined(zigduino)
# define BOARD_TYPE BOARD_ZIGDUINO
# define BOARD_NAME "zigduino"
#endif
/*=== Compile time parameters ========================================*/

/*=== Hardware Components ============================================*/
#define RADIO_TYPE (RADIO_ATMEGA128RFA1_C)

/** timestamp register for RX_START event */
#define TRX_TSTAMP_REG TCNT1
#if BOARD_TYPE == BOARD_FUNKOMAT
# define LED_PORT      PORTG
# define LED_DDR       DDRG
# define LED_MASK      (0x06)
# define LED_SHIFT     (1)
# define LEDS_INVERSE  (1)
# define LED_NUMBER    (2)
#else
# define LED_NUMBER    (0)
# define NO_LEDS       (1)
#endif
#define NO_KEYS        (1)

/*=== Host Interface ================================================*/
#if BOARD_TYPE == BOARD_FUNKOMAT_V1 || BOARD_TYPE == BOARD_ZIGDUINO
# define HIF_TYPE    HIF_UART_0
#else
# define HIF_TYPE    HIF_UART_1
#endif

#define TRX_RESET_LOW()   do { TRXPR &= ~_BV(TRXRST); } while (0)
#define TRX_RESET_HIGH()  do { TRXPR |= _BV(TRXRST); } while (0)
#define TRX_SLPTR_LOW()   do { TRXPR &= ~_BV(SLPTR); } while (0)
#define TRX_SLPTR_HIGH()  do { TRXPR |= _BV(SLPTR); } while (0)

/*=== TIMER Interface ===============================================*/
#define HWTMR_PRESCALE  (1)
#define HWTIMER_TICK    ((1.0*HWTMR_PRESCALE)/F_CPU)
#define HWTIMER_TICK_NB (0xFFFFUL)
#define HWTIMER_REG     (TCNT1)
#define TIMER_TICK      (HWTIMER_TICK_NB * HWTIMER_TICK)
#define TIMER_POOL_SIZE (4)
#define TIMER_INIT() \
    do{ \
        TCCR1B |= (_BV(CS10)); \
        TIMSK1 |= _BV(TOIE1); \
    }while(0)
#define TIMER_IRQ_vect   TIMER1_OVF_vect

#endif /* BOARD_DERFA_H */
