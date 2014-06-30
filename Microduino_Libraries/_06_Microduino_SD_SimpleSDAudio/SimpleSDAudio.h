/*
 SimpleSDAudio 
 
 Easily play audio files with Arduino from SD card in decent quality but 
 with very low external hardware.
 
 Visit our hackerspace website for more information: 
 http://www.hackerspace-ffm.de/wiki/index.php?title=SimpleSDAudio
 
 Pin-Mappings:
 =================================================================================
 PLATTFORM                             SD_CS MOSI MISO SCK SS  PWM1 PWM2 Ocx1 Ocx2
 mega168, mega328 (standard Arduinos)  4     11   12   13  10  9    10** 1A   1B
 mega1280, mega2560 (mega Arduinos)    4     51   50   52  53  44   45   5C   5B
 mega644 (Sanguino)                    4     5    6    7   4   13   12   1A   1B
 mega32U4 (Teensy 2.0)                 0     2    3    1   0   14   15   1A   1B
 mega32U4 (Leonardo)                   0     na   na   na  na  9*   10*  1A   1B
 90USB646, 90USB1286 (Teensy++)        20    22   23   21  20  25   26   1A   1B

 *change manual to output mode or change in SimpleSDAudio.h 
 **may collide with eth-shield cs-pin. Move eth-cs-pin or use only mono mode
 
 SD MMC Card Pinout
 =================================================================================
  /-----------------+  Pin   Arduino       Level shift network req.? 
 /  1 2 3 4 5 6 7 8 |   1    SD_CS         yes
 |9                 |   2    MOSI          yes       Arduino-Pin            GND
 |   Contact side   I   3    GND           -             |                   |
 |                  |   4    3.3V          -             +-[1.8k]--+--[3.3k]-+
 |                  |   5    SCK           yes                     |
 |                  |   6    GND           -                     SD-Pin
 |                  |   7    MISO          no                                 
 |                  |   8    not connect   -          Level shifting network 
 +------------------+   9    not connect   -             for 5V Arduinos
 
 Audio connection options
 =================================================================================
 For mode SSDA_MODE_MONO:
   - Very very simple (but dangerous due DC-offset voltage and too loud)
     - Connect GND to GND of active speakers
     - Connect PWM1 to L or R input of active speakers

   - Very very simple for loudspeaker (also not good due DC-offset voltage)
     - GND --[100R to 500R]--- Speaker --- PWM1

   - Better (for active speakers)
     - PWM1 ---||----[10k]---+----[1k]--- GND
              100nF          |
                             Line in of active speaker
                             
 For mode SSDA_MODE_STEREO:  
   - Use same circuits like for SSDA_MODE_MONO, but build it two times, 
     PWM1 for left, PWM2 for right channel
 
 For mode SSDA_MODE_MONO_BRIDGE: (only usefull for direct speaker drive, louder)
   - Very very simple for loudspeaker (also not good due DC-offset voltage)
     - PWM1 --[100R to 500R]--- Speaker --- PWM2

   - Better for loudspeaker 
     - PWM1 --[100R]--||----- Speaker --- PWM2
                     10uF   

  Version history:
  1.02 		Added 4 channel or 16-Bit Stereo support, 
            rewritten interrupt in assembler
  1.01      Added compatibility for Arduino 1.0 IDE
  1.00      Initial release  
 
 Copyright (c) 2012, Lutz Lisseck (lutz. lisseck AT gmx. de)
 
 MIT-License: 
 Permission is hereby granted, free of charge, to any person obtaining a 
 copy of this software and associated documentation files (the 
 "Software"), to deal in the Software without restriction, including 
 without limitation the rights to use, copy, modify, merge, publish, 
 distribute, sublicense, and/or sell copies of the Software, and to 
 permit persons to whom the Software is furnished to do so, subject to 
 the following conditions: The above copyright notice and this permission 
 notice shall be included in all copies or substantial portions of the 
 Software. 
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY 
 KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF 
 MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. 
 IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY 
 CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, 
 TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE 
 SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE. 

*/
#ifndef __SIMPLESDAUDIO_H__
#define __SIMPLESDAUDIO_H__

#if ARDUINO < 100
#include <WProgram.h>
#else
#include <Arduino.h>
#endif
#include <sd_l2.h>
#include <SimpleSDAudioDefs.h>

#define SSDA_VERSIONSTRING      "1.02"

// Sound Mode Flags
#define SSDA_MODE_FULLRATE      0x00    // 62.500 kHz @ 16 MHz, 31.250 kHz @ 8 MHz
#define SSDA_MODE_HALFRATE      0x10    // 31.250 kHz @ 16 MHz, 15.625 kHz @ 8 MHz

#define SSDA_MODE_MONO          0x00    // Use only 1st PWM pin
#define SSDA_MODE_STEREO        0x01    // Use both PWM pins for stereo output
#define SSDA_MODE_QUADRO		0x04	// Uses four PWM pins for either 4 speakers or Stereo 16 Bit
#define SSDA_MODE_MONO_BRIDGE   0x02    // Use both PWM pins for more power

// Error codes from SimpleSDAudio, see other sd_l*.h for more error codes
#define SSDA_ERROR_NULL         0x80    // Null pointer
#define SSDA_ERROR_BUFTOSMALL   0x81    // Buffer to small
#define SSDA_ERROR_NOT_INIT     0x82    // System not initialized properly
    
class SdPlayClass {
// don't change this struct from here ...
  private:
    volatile uint8_t  _flags;
    uint8_t  _flags2;           // flags not touched in interrupt
    volatile uint16_t _Buflen;  // how much bytes are availible in buffer
    uint8_t *_pBuf;             // pointer to working buffer, used for audio and all kind of file access
    volatile uint8_t *_pBufout; // pointer where next byte can read from the buffer
    uint8_t *_pBufoutend;       // points to _pBuf + _Bufsize
    uint16_t _Bufsize;          // size of working buffer, must be a multiple of 512, at least 1024    
    uint16_t _Bufin;            // index where next byte can put into the buffer
// ...to here as it is adressed directly by assembler functions!
    
    boolean  _BufViaMalloc;     // Set to true if Buf created dynamically
    uint8_t _oc_cr1_bup;        // Backup of 1st control register 
    uint8_t _oc_cr2_bup;        // Backup of 2nd control register 
    
    SD_L2_File_t _fileinfo;
    uint8_t _lastError;

    void    interruptOn(void);
    void    interruptOff(void);
  
  public:
    SdPlayClass(void);  // constructor
    ~SdPlayClass(void); // destructor
    
    void    interrupt(void); // Only for internal use!
    
    // Optional: call this before init to set SD-Cards CS-Pin to other than default    
    void    setSDCSPin(uint8_t csPin); 
    
    // Optional: call this if you want to use your own buffer (at least 1024 bytes, must be multiple of 512)
    void    setWorkBuffer(uint8_t *pBuf, uint16_t bufSize); 
    
    // Call this to set sound mode, see SSDA_MODE_* flags above for modes
    boolean init(uint8_t soundMode);
    
    // Optional: call this to free resources 
    void    deInit(void);
    
    // Optional: output directory list
    void    dir(void (*callback)(char *));  

    // After  init, call this to select audio file
    boolean setFile(char *fileName);
    
    // Call this continually in main loop 
    void    worker(void);    
    
    void    stop(void);  // stops playback if playing, sets playposition to zero
    void    play(void);  // if not playing, start playing. if playing start from zero again
    void    pause(void); // pauses playing if not playing, resumes playing if was paused
    
    boolean isStopped(void);
    boolean isPlaying(void);
    boolean isPaused(void);
    
    boolean isUnderrunOccured(void); 
    uint8_t getLastError(void);
};

extern SdPlayClass SdPlay;

#endif
