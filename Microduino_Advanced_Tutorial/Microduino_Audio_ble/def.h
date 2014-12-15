#include "Arduino.h"

const int temp_RUN[4] = {
  5,5,5,5};
const int temp_STOP[4] ={
  2,2,2,2};
boolean power_SW[4];

const int temp_VOL_ADD[4] = {
  0,10,0,0};
int temp_VOL[4],temp_SET[4],temp_SET_CACHE[4];
boolean temp_STA_H[4]={
  false,false,false,false};
boolean temp_STA_L[4]={
  false,false,false,false};

int mode[4]={0,0,0,0};


//debug--------------------------
#define interval_debug  2000	//节点查错间隔
unsigned long time_debug=0;		//定时器

boolean node_clock_error[4];	//节点响应状态
unsigned long node_clock[4],node_clock_debug[4],node_clock_cache=0;		//节点运行时间、节点响应检查时间、节点时间缓存

//rtc--------------------------
#define interval_rtc  500	//间隔
unsigned long time_rtc=0;		//定时器

