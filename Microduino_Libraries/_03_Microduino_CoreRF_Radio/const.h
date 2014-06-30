/* Copyright (c) 2007, 2008, 2009, 2010
    Marco Arena
    Axel Wachtler

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

/* $Id: const.h,v 1.36 2010/12/05 20:21:54 awachtler Exp $ */
/**
 * @file
 * @brief General Definitions.
 *
 */
/*====================================================================*/
#ifndef CONST_H
#define CONST_H
#include <stdint.h>
#include <stdbool.h>

/**
 * @addtogroup grpRadio
 * @{
 */

/*=== radio constants ================================================*/
#define  RADIO_AT86RF230 (1) /**< Identifier for radio AT86RF230 */
#define  RADIO_AT86RF230A (RADIO_AT86RF230) /**< Identifier for radio AT86RF230 Rev A */
#define  RADIO_AT86RF230B (2) /**< Identifier for radio AT86RF230 Rev B */
#define  RADIO_AT86RF231 (3) /**< Identifier for radio AT86RF231 */
#define  RADIO_AT86RF212 (4) /**< Identifier for radio AT86RF212 */
#define  RADIO_ATMEGA128RFA1_A (5) /**< Identifier for radio ATmega128RFA1 Rev. A */
#define  RADIO_ATMEGA128RFA1_B (6) /**< Identifier for radio ATmega128RFA1 Rev. B */
#define  RADIO_ATMEGA128RFA1_C (7) /**< Identifier for radio ATmega128RFA1 Rev. C */
#define  RADIO_ATMEGA128RFA1_D (8) /**< Identifier for radio ATmega128RFA1 Rev. D */

#define RADIO_BAND_700 (1) /**< 700MHz frequency band (china) */
#define RADIO_BAND_800 (2) /**< 868MHz frequency band (europe)*/
#define RADIO_BAND_900 (3) /**< 900MHz frequency band (north america)*/
#define RADIO_BAND_2400 (4)  /**< 2.4GHz frequency band (international)*/

/*=== modulation schemes =============================================*/
#define MOD_BPSK_20    (0) /**< PHY modulation BPSK, 20 kbit/s */
#define MOD_BPSK_40    (1) /**< PHY modulation BPSK, 40 kbit/s */
#define MOD_OQPSK_100  (2) /**< PHY modulation O-QPSK, 100 kbit/s */
#define MOD_OQPSK_200  (3) /**< PHY modulation O-QPSK, 200 kbit/s */
#define MOD_OQPSK_250  (4) /**< PHY modulation O-QPSK, 250 kbit/s */
#define MOD_OQPSK_400  (5) /**< PHY modulation O-QPSK, 400 kbit/s */
#define MOD_OQPSK_500  (6) /**< PHY modulation O-QPSK, 500 kbit/s */
#define MOD_OQPSK_1000 (7) /**< PHY modulation O-QPSK, 1000 kbit/s */
#define MOD_OQPSK_2000 (8) /**< PHY modulation O-QPSK, 2000 kbit/s */

/*=== RSSI base value (see datasheet) =====================*/
#define RSSI_BASE_VAL (-90)


/** @} */

/**
 * @addtogroup grpHIF
 * @{
 */

/*=== HIF type constants =============================================*/
#define  HIF_NONE     (0)  /**< Identifier for no host interface */
#define  HIF_UART_0   (10) /**< Identifier for HIF type UART 0 */
#define  HIF_UART_1   (11) /**< Identifier for HIF type UART 1 */
#define  HIF_FT245    (20) /**< Identifier for HIF type USB/FT245*/
#define  HIF_AT90USB  (21) /**< Identifier for HIF type USB/ATmega1287 */

/*=== USB constants ==================================================*/
/**
 * uracoli USB Vendor ID
 *
 * The pair of the uracoli vendor id, device id are obtained by
 *
 *  - http://www.voti.nl/pids/
 *  - http://www.frank-buss.de/pid.txt
 */
#define URACOLI_USB_VID (5824)
/**
 * uracoli USB Product ID
 * see also @ref USB_VID_URACOLI
 */
#define URACOLI_USB_PID (2183)

#define URACOLI_USB_BCD_RELEASE (0x100)
#define URACOLI_USB_VENDOR_NAME  L"URACOLI"
#define URACOLI_USB_PRODUCT_NAME L"RZUSBSTICK"

/** @} */

/**
 * @addtogroup grpTrx
 * @{
 */

/*=== spi constants ==================================================*/
#define SPI_RATE_1_2      (4) /**< SPI clock running is 0.5 (1/2) of cpu clock */
#define SPI_RATE_1_4      (0) /**< SPI clock running is 0.25 (1/4) of cpu clock */
#define SPI_RATE_1_8      (5) /**< SPI clock running is 0.125 (1/8) of cpu clock */
#define SPI_RATE_1_16     (1) /**< SPI clock running is 0.0635 (1/16) of cpu clock */
#define SPI_RATE_1_32     (6) /**< SPI clock running is 0.03125 (1/32)of cpu clock */
#define SPI_RATE_1_64     (2) /**< SPI clock running is 0.015625 (1/64)of cpu clock */
#define SPI_RATE_1_128    (3) /**< SPI clock running is 0.0078125 (1/128) of cpuclock */

/** @} */


#if defined (DOXYGEN)
/** macro that forces an enumeration to use 8 bit instead of 16 bit integers. */
#define SHORTENUM
#else
#define SHORTENUM  __attribute__((packed))
#endif

/**
 * @addtogroup grpTrx
 * @{
 */
#define FCTL_DATA _BV(0)        /**< data frame fype in frame control field */
#define FCTL_ACK  _BV(5)        /**< ack request in frame control field */
#define FCTL_IPAN _BV(6)        /**< intra pan bit in frame control field */
#define FCTL_DST_SHORT 0x0800   /**< destination short address in frame control field */
#define FCTL_DST_LONG  0x0c00   /**< destination long address in frame control field */
#define FCTL_SRC_SHORT 0x8000   /**< source short address in frame control field */
#define FCTL_SRC_LONG  0xc000   /**< source long address in frame control field */

#define FCTL_SRC_MASK (FCTL_SRC_LONG)
#define FCTL_DST_MASK (FCTL_DST_LONG)
#define FCTL_IPAN_MASK (FCTL_IPAN)
/** @} */

/* === Types ================================================================= */


/**
 * @addtogroup grpTrx
 * @{
 */

/** transceiver channel type */
typedef int8_t  channel_t;

/** transceiver transmit type */
typedef int8_t  txpwr_t;

/** radio idle state, if true radio idles in state PX_ON
 *  @todo make it state_t variable, so that idle state can explicitely selected.
 */
typedef bool    rxidle_t;

/** transceiver cca mode, 1 : ED, 2: CS, 3: CS & ED */
typedef uint8_t ccamode_t;

/* ... cca_ed_tresh, clkm, pdt, ... */

/**
 * Transceiver parameter structure
 */
typedef struct
{
    /** current channel see sub register @ref SR_CHANNEL*/
    channel_t chan;
    /**  TX power index see sub register @ref SR_TX_PWR*/
    uint8_t txp   : 4;
    /**  CCA mode see sub register @ref SR_CCA_MODE */
    ccamode_t cca   : 2;
    /**  ED threshold see sub register @ref SR_CCA_ED_THRES */
    uint8_t edt   : 4;

    /**  clkm control  see sub register @ref SR_CLKM_CTRL */
    uint8_t clkm  : 3;

} trx_param_t;

/** @} */
#endif  /* #ifndef CONST_H */
