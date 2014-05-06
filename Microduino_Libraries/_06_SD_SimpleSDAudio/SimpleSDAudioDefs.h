/*
 SimpleSDAudioDef
 Defines used by C and ASM parts

*/
#ifndef __SIMPLESDAUDIODEFS_H__
#define __SIMPLESDAUDIODEFS_H__

// Flags 
#define SSDA_F_PLAYING  0  // 1 if playing active
#define SSDA_F_STOPPED  1  // 1 if stopped or file reached end
#define SSDA_F_UNDERRUN 2  // 1 if buffer underrun occured
#define SSDA_F_HALFRATE 3  // If 1, only every 2nd interrupt is used for sample refresh
#define SSDA_F_HRFLAG   4  // Flag to find every 2nd interrupt
#define SSDA_F_STEREO   5  // If 1, OC2 outputs the second channel
#define SSDA_F_BRIDGE   6  // If 1, OC2 outputs the same signal but inverted (for more output power)
#define SSDA_F_QUAD		7  // If 1, also OC3 and OC4 outputs the signal

#define SSDA_USE_ASM    1

#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
// Mega
    #define SSDA_PWM1_PIN 44 // OC5C    L
    #define SSDA_PWM2_PIN 45 // OC5B    H
    #define SSDA_PWM3_PIN  2 // OC3B	L
    #define SSDA_PWM4_PIN  3 // OC3C    H
    
    // Output-compare settings
    #define SSDA_OC1L    OCR5CL
    #define SSDA_OC2L    OCR5BL
    #define SSDA_OC3L    OCR3BL
    #define SSDA_OC4L    OCR3CL
    #define SSDA_OC1H    OCR5CH
    #define SSDA_OC2H    OCR5BH
    #define SSDA_OC3H    OCR3BH
    #define SSDA_OC4H    OCR3CH
    
    // Register to backup and restore
    #define SSDA_OC_CR1_REG             TCCR5A
    #define SSDA_OC_CR2_REG             TCCR5B
    #define SSDA_OC_CR3_REG             TCCR3A
    #define SSDA_OC_CR4_REG             TCCR3B
    
    // Always: Prescaler = 1, Fast-PWM-Mode with 8-Bit
    #define SSDA_SINGLE_OC_ENABLE()     { TCCR5A = _BV(WGM50) | _BV(COM5C1); TCCR5B = _BV(WGM52) | _BV(CS50);}
    #define SSDA_DUAL_OC_ENABLE()       { TCCR5A = _BV(WGM50) | _BV(COM5C1) | _BV(COM5B1); TCCR5B = _BV(WGM52) | _BV(CS50);}
    #define SSDA_QUAD_OC_ENABLE()       { TCCR3A = _BV(WGM30) | _BV(COM3C1) | _BV(COM3B1); TCCR3B = _BV(WGM32) | _BV(CS30);}
    #define SSDA_QUAD_SYNC()			{ TCNT5 = 0; TCNT3 = 0; }
	#define SSDA_DUAL_OC_BRIDGING()     { TCCR5A |= _BV(COM5B0); }

    #define SSDA_OC_INT_DISABLE()       { TIMSK5 &= ~_BV(TOIE5); TIFR5 |= _BV(TOV5); }
    #define SSDA_OC_INT_ENABLE()        { TIMSK5 |=  _BV(TOIE5); TIFR5 |= _BV(TOV5); }

    #define SSDA_OC_INTERRUPT           TIMER5_OVF_vect
    
//------------------------------------------------------------------------------
#elif defined(__AVR_ATmega644P__) || defined(__AVR_ATmega644__)
// Sanguino
    #define SSDA_PWM1_PIN 13 // OC1A
    #define SSDA_PWM2_PIN 12 // OC1B
    
    // Output-compare settings
    #define SSDA_OC1L    OCR1AL
    #define SSDA_OC2L    OCR1BL
    #define SSDA_OC1H    OCR1AH
    #define SSDA_OC2H    OCR1BH
    
    // Register to backup and restore
    #define SSDA_OC_CR1_REG             TCCR1A
    #define SSDA_OC_CR2_REG             TCCR1B
    
    // Always: Prescaler = 1, Fast-PWM-Mode with 8-Bit
    #define SSDA_SINGLE_OC_ENABLE()     { TCCR1A = _BV(WGM10) | _BV(COM1A1); TCCR1B = _BV(WGM12) | _BV(CS10);}
    #define SSDA_DUAL_OC_ENABLE()       { TCCR1A = _BV(WGM10) | _BV(COM1A1) | _BV(COM1B1); TCCR1B = _BV(WGM12) | _BV(CS10);}
    #define SSDA_DUAL_OC_BRIDGING()     { TCCR1A |= _BV(COM1B0); }

    #define SSDA_OC_INT_DISABLE()       { TIMSK1 &= ~_BV(TOIE1); TIFR1 |= _BV(TOV1); }
    #define SSDA_OC_INT_ENABLE()        { TIMSK1 |=  _BV(TOIE1); TIFR1 |= _BV(TOV1); }

    #define SSDA_OC_INTERRUPT           TIMER1_OVF_vect

//------------------------------------------------------------------------------
#elif defined(__AVR_ATmega32U4__)
// Teensy 2.0  
    
    // For Teensy
    #define SSDA_PWM1_PIN 14 // OC1A PB5
    #define SSDA_PWM2_PIN 15 // OC1B PB6
    
    // For Leonardo
    //#define SSDA_PWM1_PIN  9 // OC1A PB5
    //#define SSDA_PWM2_PIN 10 // OC1B PB6
    
    // Output-compare settings
    #define SSDA_OC1L    OCR1AL
    #define SSDA_OC2L    OCR1BL
    #define SSDA_OC1H    OCR1AH
    #define SSDA_OC2H    OCR1BH
    
    // Register to backup and restore
    #define SSDA_OC_CR1_REG             TCCR1A
    #define SSDA_OC_CR2_REG             TCCR1B
    
    // Always: Prescaler = 1, Fast-PWM-Mode with 8-Bit
    #define SSDA_SINGLE_OC_ENABLE()     { TCCR1A = _BV(WGM10) | _BV(COM1A1); TCCR1B = _BV(WGM12) | _BV(CS10);}
    #define SSDA_DUAL_OC_ENABLE()       { TCCR1A = _BV(WGM10) | _BV(COM1A1) | _BV(COM1B1); TCCR1B = _BV(WGM12) | _BV(CS10);}
    #define SSDA_DUAL_OC_BRIDGING()     { TCCR1A |= _BV(COM1B0); }

    #define SSDA_OC_INT_DISABLE()       { TIMSK1 &= ~_BV(TOIE1); TIFR1 |= _BV(TOV1); }
    #define SSDA_OC_INT_ENABLE()        { TIMSK1 |=  _BV(TOIE1); TIFR1 |= _BV(TOV1); }

    #define SSDA_OC_INTERRUPT           TIMER1_OVF_vect

//------------------------------------------------------------------------------
#elif defined(__AVR_AT90USB646__) || defined(__AVR_AT90USB1286__)
// Teensy++ 1.0 & 2.0
    // For Teensy
    #define SSDA_PWM1_PIN 25 // OC1A PB5
    #define SSDA_PWM2_PIN 26 // OC1B PB6
    
    // Output-compare settings
    #define SSDA_OC1L    OCR1AL
    #define SSDA_OC2L    OCR1BL
    #define SSDA_OC1H    OCR1AH
    #define SSDA_OC2H    OCR1BH
    
    // Register to backup and restore
    #define SSDA_OC_CR1_REG             TCCR1A
    #define SSDA_OC_CR2_REG             TCCR1B
    
    // Always: Prescaler = 1, Fast-PWM-Mode with 8-Bit
    #define SSDA_SINGLE_OC_ENABLE()     { TCCR1A = _BV(WGM10) | _BV(COM1A1); TCCR1B = _BV(WGM12) | _BV(CS10);}
    #define SSDA_DUAL_OC_ENABLE()       { TCCR1A = _BV(WGM10) | _BV(COM1A1) | _BV(COM1B1); TCCR1B = _BV(WGM12) | _BV(CS10);}
    #define SSDA_DUAL_OC_BRIDGING()     { TCCR1A |= _BV(COM1B0); }

    #define SSDA_OC_INT_DISABLE()       { TIMSK1 &= ~_BV(TOIE1); TIFR1 |= _BV(TOV1); }
    #define SSDA_OC_INT_ENABLE()        { TIMSK1 |=  _BV(TOIE1); TIFR1 |= _BV(TOV1); }

    #define SSDA_OC_INTERRUPT           TIMER1_OVF_vect    

//------------------------------------------------------------------------------
#else  
// 168 and 328 Arduinos (standard pinning)
    #define SSDA_PWM1_PIN  9 // OC1A PB1
    #define SSDA_PWM2_PIN 10 // OC1B PB2 (collision with SS!)
    #define SSDA_PWM3_PIN  6 // OC0A PD6
    #define SSDA_PWM4_PIN  5 // OC0B PD5
    
    // Output-compare settings
    #define SSDA_OC1L    OCR1AL
    #define SSDA_OC2L    OCR1BL
    #define SSDA_OC3L    OCR0A
    #define SSDA_OC4L    OCR0B
    #define SSDA_OC1H    OCR1AH
    #define SSDA_OC2H    OCR1BH
   
    // Register to backup and restore
    #define SSDA_OC_CR1_REG             TCCR1A
    #define SSDA_OC_CR2_REG             TCCR1B
    #define SSDA_OC_CR3_REG             TCCR0A
    #define SSDA_OC_CR4_REG             TCCR0B
	
    // Always: Prescaler = 1, Fast-PWM-Mode with 8-Bit
    #define SSDA_SINGLE_OC_ENABLE()     { TCCR1A = _BV(WGM10) | _BV(COM1A1); TCCR1B = _BV(WGM12) | _BV(CS10);}
    #define SSDA_DUAL_OC_ENABLE()       { TCCR1A = _BV(WGM10) | _BV(COM1A1) | _BV(COM1B1); TCCR1B = _BV(WGM12) | _BV(CS10);}
    #define SSDA_DUAL_OC_BRIDGING()     { TCCR1A |= _BV(COM1B0); }
    #define SSDA_QUAD_OC_ENABLE()       { TCCR0A = _BV(WGM00) | _BV(WGM01) | _BV(COM0A1) | _BV(COM0B1); TCCR0B = _BV(WGM02) | _BV(CS00);}
    #define SSDA_QUAD_SYNC()			{ TCNT1 = 0; TCNT0 = 0; }
	
    #define SSDA_OC_INT_DISABLE()       { TIMSK1 &= ~_BV(TOIE1); TIFR1 |= _BV(TOV1); }
    #define SSDA_OC_INT_ENABLE()        { TIMSK1 |=  _BV(TOIE1); TIFR1 |= _BV(TOV1); }

    #define SSDA_OC_INTERRUPT           TIMER1_OVF_vect 

/*	
// 168 and 328 Arduinos (alternative pinning that uses timer0 ports)
//   use with patched wiring.c - FREE_TIMER0 must be defined
//
    #define SSDA_PWM1_PIN  6 // OC0A PD6
    #define SSDA_PWM2_PIN  5 // OC0B PD5
    #define SSDA_PWM3_PIN  9 // OC1A PB1
    #define SSDA_PWM4_PIN 10 // OC1B PB2 (collision with SS!)  
	
    // Output-compare settings
    #define SSDA_OC1L    OCR0A
    #define SSDA_OC2L    OCR0B
    #define SSDA_OC3L    OCR1AL
    #define SSDA_OC4L    OCR1BL
    #define SSDA_OC3H    OCR1AH
    #define SSDA_OC4H    OCR1BH
   
    // Register to backup and restore
    #define SSDA_OC_CR1_REG             TCCR0A
    #define SSDA_OC_CR2_REG             TCCR0B
    #define SSDA_OC_CR3_REG             TCCR1A
    #define SSDA_OC_CR4_REG             TCCR1B

    // Always: Prescaler = 1, Fast-PWM-Mode with 8-Bit
    #define SSDA_SINGLE_OC_ENABLE()     { TCCR0A = _BV(WGM01) | _BV(WGM00) | _BV(COM0A1); TCCR0B = _BV(CS00);}
    #define SSDA_DUAL_OC_ENABLE()       { TCCR0A = _BV(WGM01) | _BV(WGM00) | _BV(COM0A1) | _BV(COM0B1); TCCR0B = _BV(CS00);}
    #define SSDA_DUAL_OC_BRIDGING()     { TCCR0A |= _BV(COM0B0); }
    #define SSDA_QUAD_OC_ENABLE()       { TCCR1A = _BV(WGM10) | _BV(COM1A1) | _BV(COM1B1); TCCR1B = _BV(WGM12) | _BV(CS10);}
    #define SSDA_QUAD_SYNC()			{ TCNT0 = 0; TCNT1 = 0; }
	
    #define SSDA_OC_INT_DISABLE()       { TIMSK0 &= ~_BV(TOIE0); TIFR0 |= _BV(TOV0); }
    #define SSDA_OC_INT_ENABLE()        { TIMSK0 |=  _BV(TOIE0); TIFR0 |= _BV(TOV0); }

    #define SSDA_OC_INTERRUPT           TIMER0_OVF_vect 
*/	
#endif
    


#endif
