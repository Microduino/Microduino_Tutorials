#include <avr/io.h>
#include <SimpleSDAudioDefs.h>

#if(SSDA_USE_ASM)    
.global SSDA_OC_INTERRUPT
SSDA_OC_INTERRUPT:
    /* save stack */
    push r16
    in r16,_SFR_IO_ADDR(SREG)
    push r16
    push r17
    
    // if(flags & SSDA_F_PLAYING) {
    lds r16, SdPlay  /* .flags */
    sbrs r16, SSDA_F_PLAYING
    rjmp LEAVE

    // if(!(flags & SSDA_F_HALFRATE) || ((flags ^= SSDA_F_HRFLAG) & SSDA_F_HRFLAG)) {
    sbrs r16, SSDA_F_HALFRATE
    rjmp GOON1
    ldi r17, 1<<SSDA_F_HRFLAG
    eor r16, r17
    sbrs r16, SSDA_F_HRFLAG
    rjmp GOOUT1
    
GOON1:
    push r26
    push r27
    lds r26, SdPlay + 2  // ._Buflen
    lds r27, SdPlay + 3
    // if(_Buflen > 1) {
    // if(flags & SSDA_F_STEREO) {

#ifdef SSDA_OC3L    
	sbrs r16, SSDA_F_QUAD
	rjmp SKIPQUAD
	
QUAD:
	sbiw r26, 4
	brsh QUAD_W1
	rjmp UNDERRUN
QUAD_W1:
	// _Buflen -= 4
    sts SdPlay + 2, r26 // ._Buflen
    sts SdPlay + 3, r27
    ldi r17, 0
    #ifdef SSDA_OC1H
      sts SSDA_OC1H, r17
    #endif
    #ifdef SSDA_OC3H
      sts SSDA_OC3H, r17
    #endif 
    // temp = *_pBufout++;
    lds r26, SdPlay + 6 // ._pBufout
    lds r27, SdPlay + 7 // ._pBufout
    ld r17, X+
    sts SSDA_OC1L, r17
    ld r17, X+
    sts SSDA_OC2L, r17	
    ld r17, X+
    sts SSDA_OC3L, r17
    ld r17, X+
    sts SSDA_OC4L, r17
    rjmp GEN_W1
#endif	
SKIPQUAD:	
    sbrs r16, SSDA_F_STEREO
    rjmp MONO
STEREO:    
    sbiw r26, 2
    brsh STEREO_W1
    rjmp UNDERRUN
STEREO_W1:
    // _Buflen-=2; 
    sts SdPlay + 2, r26 // ._Buflen
    sts SdPlay + 3, r27
    #ifdef SSDA_OC1H
      ldi r17, 0
      sts SSDA_OC1H, r17
    #endif
    // temp = *_pBufout++;
    lds r26, SdPlay + 6 // ._pBufout
    lds r27, SdPlay + 7 // ._pBufout
    ld r17, X+
    sts SSDA_OC1L, r17
    ld r17, X+
    sts SSDA_OC2L, r17
    rjmp GEN_W1
    
MONO:
    sbiw r26, 1
    brsh MONO_W1
    rjmp UNDERRUN
MONO_W1:
    // _Buflen-=1; 
    sts SdPlay + 2, r26
    sts SdPlay + 3, r27  
    #ifdef SSDA_OC1H
      ldi r17, 0
      sts SSDA_OC1H, r17
    #endif
    // temp = *_pBufout++;
    lds r26, SdPlay + 6 // ._pBufout
    lds r27, SdPlay + 7 // ._pBufout 
    ld r17, X+
    sts SSDA_OC1L, r17
    // if(flags & SSDA_F_BRIDGE) {
    sbrc r16, SSDA_F_BRIDGE
    sts SSDA_OC2L, r17
    
GEN_W1:
    push r18
    // if(_pBufout >= _pBufoutend) _pBufout -= _Bufsize;
    lds r17, SdPlay + 8 // ._pBufoutend
    lds r18, SdPlay + 9
    
    cp  r26, r17
    cpc r27, r18
    brlo GEN_W2
    lds r17, SdPlay + 10 // ._Bufsize
    lds r18, SdPlay + 11
    sub r26, r17
    sbc r27, r18

GEN_W2:    
    // Store _pBufout
    sts SdPlay + 6, r26
    sts SdPlay + 7, r27
    
    pop r18
    rjmp GOOUT2
    
UNDERRUN:
    // flags |= SSDA_F_UNDERRUN;
    sbr r16, 1<<SSDA_F_UNDERRUN    
    
GOOUT2:
    pop r27
    pop r26

GOOUT1:
    /* store flags back to RAM */
    sts SdPlay, r16  /* .flags */

LEAVE: /* restore stack */
    pop r17
    pop r16
    out _SFR_IO_ADDR(SREG),r16
    pop r16
    reti
#endif    
.end

/*
  if(flags & SSDA_F_PLAYING) {
    if(!(flags & SSDA_F_HALFRATE) || ((flags ^= SSDA_F_HRFLAG) & SSDA_F_HRFLAG)) {
      if(_Buflen > 1) {
         #ifdef SSDA_OC1H
           SSDA_OC1H = 0;
         #endif
         uint8_t temp;
         temp = *_pBufout++;
         SSDA_OC1L = temp; 
         if(flags & SSDA_F_STEREO) {
            _Buflen-=2; 
            temp = *_pBufout++;
            SSDA_OC2L = temp;
         } else {
           _Buflen--;
           if(flags & SSDA_F_BRIDGE) {
             SSDA_OC2L = temp;
           }
         }
         if(_pBufout >= _pBufoutend) _pBufout -= _Bufsize;
      } else {
        flags |= SSDA_F_UNDERRUN;
      }            
    } 
    _flags = flags;
  }
*/