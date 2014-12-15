#include "Arduino.h"

#include "U8glib.h"

#include "Micromenu_def.h"
#include "Micromenu_core.h"
#include "Micromenu_user.h"
#include "Micromenu_draw.h"
#include "Micromenu_menu.h"

unsigned long time_drawMain;	//定时器 主界面更新

//界面条件初始化
void osd_smart_init()
{
  for(int a=0;a<MENU_ITEMS1+1;a++)	//判断第一层
  {
//Serial.print(" level1 run:[");
    if(xjj[a][0][0] == NULL)
    {
      //Serial.print(" level1 run:[");
      //Serial.print(a);
      //Serial.println("]: NULL");
      break;	//NULL，结束当前层判断
    }
    else
    {
      //Serial.print(a);
      //Serial.println("]: OK");
      menu_num_level1 = a+1;

      //Serial.println(" level2 run:[ ");
      for(int b=0;b<MENU_ITEMS2;b++)	//判断第二层
      {
        if(xjj[a][b][1] == NULL)
        {
          //Serial.print(b);
          //Serial.print(":NULL, ");
          break;	//NULL，结束当前层判断
        }
        else
        {
          //Serial.print(b);
          //Serial.print(" :OK, ");
          menu_num_level2[a] = b+1;

          //Serial.print(" level3 run:[ ");
          for(int c=0;c<MENU_ITEMS3;c++)	//判断第三层
          {
            if(xjj[a][b][2+c] == NULL)
            {
              //Serial.print(c);
              //Serial.print(":NULL, ");
              break;	//NULL，结束当前层判断
            }
            else
            {
              //Serial.print(c);
              //Serial.print(" :OK, ");
              menu_num_level3[a][b] = c+1;
            }
          }

          //Serial.print(" ] level3 ok: ");
          //Serial.println(menu_num_level3[a][b]);
          //Serial.println(" ------------------------ ");

        }
      }

      //Serial.print(" ] level2 ok: ");
      //Serial.println(menu_num_level2[a]);
      //Serial.println(" ------------------------ ");
    }
  }
  //Serial.print(" level1 ok: ");
  //Serial.println(menu_num_level1);

}

//菜单等级
void osd_smart_begin()
{
  switch(menu_level)
  {
  case 0:    
    break;
  case 1:
    MENU_ITEMS=menu_num_level1;
    break;
  case 2:
    MENU_ITEMS=menu_num_level2[menu_current[0]] + 1;
    break;
  case 3:
    MENU_ITEMS=menu_num_level3[menu_current[0]] [menu_current[1]-1] + 1;
    break;
  }
}

//主图形函数
void osd_smart_run(boolean _osd_smart_run)
{
  u8g.firstPage();
  do  {
    if(!_osd_smart_run)			//主界面
    {
      drawMain();				
    }
    else
    {
      if(sta)				//配置模式
        drawRun(type,num);		
      else				//菜单界面
        drawMenu();				
    }
  } 
  while( u8g.nextPage() );
}
