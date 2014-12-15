#include "Arduino.h"

//执行各按键任务
void task(int _task,int _task_type)
{
  switch(_task)
  {
  case KEY_NEXT:
    {
      for(int a=0;a<TASK_NUM;a++)		//扫描任务
      {
        if(change(0,task_action[a][0+3],task_action[a][1+3],task_action[a][2+3],task_action[a][3+3]))	//可以被执行时
        {
          switch(_task_type)		//改变值
          {
          case RUN_NUM:
            if(num<task_action[a][2]) num++;
            else num=task_action[a][1];
            break;
          case RUN_SWITCH:
            num=!num;

            break;
          }
          break;
        }
      }
    }
    break;
  case KEY_PREV:
    { 
      for(int a=0;a<TASK_NUM;a++)		//扫描任务
      {
        if(change(0,task_action[a][0+3],task_action[a][1+3],task_action[a][2+3],task_action[a][3+3]))	//可以被执行时
        {
          switch(_task_type)		//改变值
          {
          case RUN_NUM:
            if(num>task_action[a][1]) num--;
            else num=task_action[a][2];
            break;
          case RUN_SWITCH:
            num=!num;
            break;
          }
          break;
        }
      }
    }
    break;
  case KEY_SELECT:
    {
      int cba=0;
      for(int a=0;a<TASK_NUM;a++)		//扫描任务
      {
        cba=change(1,task_action[a][0+3],task_action[a][1+3],task_action[a][2+3],task_action[a][3+3]);	//可以被执行时
        if(cba)
        {
          task_in(a);		//载入任务程序

          num=task_action[a][0];	//载入任务值给osd
          sta=1;

          if (menu_level > cba)  menu_level=cba;	//标记执行层
        }
      }
    }
    break;

  case KEY_BACK:
    {
      if(sta)
      {
        for(int a=0;a<TASK_NUM;a++)		//扫描任务
        {
          if(change(0,task_action[a][0+3],task_action[a][1+3],task_action[a][2+3],task_action[a][3+3]))	//可以被执行时
          {
            task_action[a][0]=num;		//osd值返回给任务

            task_out(a);		//载出任务程序
          }
        }
        sta=0;
      }
    }
    break;
  }

}
