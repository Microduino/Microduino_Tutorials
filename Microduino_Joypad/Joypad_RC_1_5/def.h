#include "Arduino.h"

//DEBUG-----------
#define Serial_DEBUG

//MWC-------------
#define mwc_port Serial 

//nRF-------------
#define interval_debug  2000	//节点查错间隔
#define nRF_channal 70    //0~125

//TFT-------------
#define THEME 0    //0 is white,1 is black

#if THEME == 0
#define TFT_BUT ST7735_WHITE
#define TFT_TOP ST7735_BLACK
#endif

#if THEME == 1
#define TFT_BUT ST7735_BLACK
#define TFT_TOP ST7735_WHITE
#endif

//Joy-------------
//1000~2000
#define JOYSTICK_DEAD_ZONE 25
#define Joy_s_maximum 200
#define Joy_maximum 500
#define Joy_MID 1500

//Time------------
#define interval_time2 60      //send data
#define interval_TIME1 2000    //setup delay

