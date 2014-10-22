#include <TVout.h>
#include <fix_fft.h>
#include <fontALL.h>
#include <stdint.h>
const uint8_t WIDTH = 128;
const uint8_t HEIGHT = 96;
TVout TV;

char im[128], data[128], lastpass[64];
char x=32, ylim=90;
int i=0,val;

void setup() {

    TV.begin(PAL,WIDTH, HEIGHT);                              //  Initialize TV output, 128x96.
    TV.select_font(font4x6);
    TV.print(2,2,"  Realtime Microduino");             //  TVout lib uses x,y for print
    TV.print(2,11,"  Spectrum Analyzer");         //  statements.  8x8 default font.
    analogReference(DEFAULT);                          //  Use default (5v) aref voltage.
    for (int z=0; z<64; z++) {lastpass[z]=80;};       //  fill the lastpass[] array with dummy data

}
void loop() {
  
      for (i=0; i < 128; i++){                                     // We don't go for clean timing here, it's
      val = analogRead(0);                                      // better to get somewhat dirty data fast
      data[i] = val/4 -128;                                       // than to get data that's lab-accurate
      im[i] = 0;                                                       // but too slow, for this application.
      }

    fix_fft(data,im,7,0);
   
    for (i=1; i< 64;i++){                                          // In the current design, 60Hz and noise
      data[i] = sqrt(data[i] * data[i] + im[i] * im[i]);  // in general are a problem.  Future designs
      TV.draw_line(i+x,lastpass[i],i+x,ylim,0);          // and code may fix this, but for now, I
      TV.draw_line(i+x,ylim,i+x,ylim-data[i],1);        // skip displaying the 0-500hz band completely.
      lastpass[i]=ylim-data[i];                                   // if you insist, initialize the loop with 0
      }
}
