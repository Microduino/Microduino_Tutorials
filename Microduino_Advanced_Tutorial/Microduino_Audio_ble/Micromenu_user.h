#include "Arduino.h"

//用户自定义部分------------------------
#include <Joypad.h>

#include "def.h"
#include "rtc.h"
#include "audio.h"
#include "alarm.h"
#include "eep.h"
//用户自定义部分------------------------

int num_cache = 0;

//用户自定义部分------------------------

//用户自定义部分------------------------

//"_task_out"是返回的任务ID，值对应"task_action[*][0]"
void task_out(int _task_out)    
{
  if(_task_out >= 0 && _task_out <= 0)	//在执行对应任务ID范围时，赋对应值给用户变量（不加判断，自由赋值也行）
  {
    alarm_switch=task_action[0][0];

    eeprom_WRITE();
  }

  if(_task_out >= 1 && _task_out <= 2)
  {
    alarm_hour=task_action[1][0];
    alarm_minute=task_action[2][0];

    set_alarm(alarm_hour,alarm_minute);

    eeprom_WRITE();
  }

  if(_task_out >= 3 && _task_out <= 3)
  {    
    alarm_tone=task_action[3][0];

    audio_pause();
    test_audio=false;

    eeprom_WRITE();
  }

  if(_task_out >= 4 && _task_out <= 8)
  {
    Year=task_action[4][0];
    Month=task_action[5][0];
    Day=task_action[6][0];

    Hour=task_action[7][0];
    Minute=task_action[8][0];

    getDateStamp(getTimeStamp(Hour,Minute,Second,Day,Month,Year));
    //    Serial.println(getTimeStamp(Hour,Minute,Second,Day,Month,Year));
    setRTC();
  }

  if(_task_out >= 9 && _task_out <= 9)
  {
    MENU_FONT=task_action[9][0];

    eeprom_WRITE();
  }

  if(_task_out >= 10 && _task_out <= 10)
  {
    alarm_vol = task_action[10][0];

    audio_pause();
    test_audio_vol=false;

    eeprom_WRITE();
  }

}

//"_task_in"是返回的任务ID，值对应"task_action[*][0]"
void task_in(int _task_in)
{
  if(_task_in >= 0 && _task_in <= 0)	//在执行对应任务ID范围时，用户变量赋对应值给任务（不加判断，自由赋值也行）
  {
    task_action[0][0]=alarm_switch;
  }

  if(_task_in >= 1 && _task_in <= 2)
  {
    task_action[1][0]=alarm_hour;
    task_action[2][0]=alarm_minute;
  }

  if(_task_in >= 3 && _task_in <= 3)
  {
    task_action[3][0]=alarm_tone;

    do_alarm(alarm_tone);

    test_audio=true;
  }

  if(_task_in >= 4 && _task_in <= 8)
  {
    getRTC();

    task_action[4][0] =Year;
    task_action[5][0] =Month;
    task_action[6][0] =Day;
    task_action[7][0] =Hour;
    task_action[8][0] =Minute;
  }

  if(_task_in >= 9 && _task_in <= 9)
  {
    task_action[9][0] =MENU_FONT;
  }

  if(_task_in >= 10 && _task_in <= 10)
  {
    task_action[10][0] = alarm_vol;

    do_alarm(alarm_tone);

    test_audio_vol=true;
  }
}

