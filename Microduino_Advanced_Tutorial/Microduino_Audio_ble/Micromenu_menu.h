#include "Arduino.h"

#include "Micromenu_key.h"
#include "Micromenu_task.h"

boolean menu_redraw_required = true;    // force initial redraw

int MENU_ITEMS;			//菜单等级

#define osd_scroll_bar 9		//滚动条宽度
#define osd_print_start_x 4		//菜单字体起始x坐标

//根据输入动作更新菜单项---------------
void updateMenu(int _updateMenu_type) 
{
  if ( uiKeyCode != KEY_NONE && last_key_code == uiKeyCode )
    return;

  last_key_code = uiKeyCode;

  switch ( uiKeyCode ) 	//有按键动作时
  {
  case KEY_NEXT:
    if(sta)				//配置模式
    {
      task(KEY_NEXT,_updateMenu_type);
    }
    else				//菜单模式
    {
      menu_current[menu_level-1]++;
      if ( menu_current[menu_level-1] >= MENU_ITEMS )
      {
        menu_current[menu_level-1] = 0;
      }
    }

    menu_redraw_required = true;
    break;
  case KEY_PREV:
    if(sta)				//配置模式
    {
      task(KEY_PREV,_updateMenu_type);
    }
    else				//菜单模式
    {
      if ( menu_current[menu_level-1] == 0 )
      {
        menu_current[menu_level-1] = MENU_ITEMS;
      }
      menu_current[menu_level-1]--;
    }

    menu_redraw_required = true;
    break;
  case KEY_SELECT:
    if(sta)				//配置模式
    {
      task(KEY_BACK,_updateMenu_type);		//退出配置
    }
    else				//菜单模式
    {
      if(menu_level != 0 && menu_current[menu_level-1] == 0)
      {
        menu_level--;

        menu_current[1]=1;
        menu_current[2]=1;
      }
      else if(sta == 0)
      {
        menu_level++;
      }

      task(KEY_SELECT,_updateMenu_type);

      if (menu_level > MENU_LEVEL) menu_level=MENU_LEVEL;		//限值
      //Serial.print("menu_level:");
      //Serial.println(menu_level);
    }

    menu_redraw_required = true; 
    break;
  case KEY_BACK:
    if(sta == 0)		//菜单模式
    {
      if (menu_level != 0 )
      {
        menu_level--;

        menu_current[1]=1;
        menu_current[2]=1;
      }
    }

    task(KEY_BACK,_updateMenu_type);	//退出配置

    menu_redraw_required = true;
    break;
  }
}

//菜单界面------------------
void drawMenu(void) 
{
  switch(MENU_FONT)	//字体大小
  {
  case 0:
    setFont_S;
    break;
  case 1:
    setFont_M;
    break;
  case 2:
    setFont_L;
    break;
  }
  u8g.setFontRefHeightText();	//参考坐标
  u8g.setFontPosTop();			//置顶

  //获取必要值以自适应屏幕-------
  u8g_uint_t osd_font_h = u8g.getFontAscent()-u8g.getFontDescent();	//获取字体高度
  u8g_uint_t osd_w = u8g.getWidth();	//获取屏幕宽
  u8g_uint_t osd_h = u8g.getHeight();	//获取屏幕高

  ////菜单高亮---------------------------
  u8g.setDefaultForegroundColor();
  u8g.drawFrame(osd_w-osd_scroll_bar, 0, osd_scroll_bar, osd_h);

  u8g.drawBox(osd_w-osd_scroll_bar+1, map(menu_current[menu_level-1],0,MENU_ITEMS-1,1,osd_h-2), osd_scroll_bar-2, 1);
  //  u8g.setDefaultForegroundColor();

  //绘制菜单-------------------------------
  for(int i = 0; i < MENU_ITEMS; i++ )
  {
    u8g.setDefaultForegroundColor();

    if ( i == menu_current[menu_level-1]) 
    {
      if((osd_font_h+2)*(menu_current[menu_level-1]+1) > osd_h)
        u8g.drawBox(0, osd_h-(osd_font_h+2), osd_w-osd_scroll_bar-1, osd_font_h+2);
      else
        u8g.drawBox(0, i*(osd_font_h+2), osd_w-osd_scroll_bar-1, osd_font_h+2);
      u8g.setDefaultBackgroundColor();
    }

    if((osd_font_h+2)*(menu_current[menu_level-1]+1) > osd_h)
      u8g.setPrintPos(osd_print_start_x, osd_h+1-(menu_current[menu_level-1]+1-i)*(osd_font_h+2));
    else
      u8g.setPrintPos(osd_print_start_x, 1+i*(osd_font_h+2));

    switch(menu_level)
    {
    case 1:   
      u8g.print(xjj[i][0][0]);
      break;
    case 2:
      if(i==0)
        u8g.print(xjj[menu_current[0]][0][0]);
      else
        u8g.print(xjj[menu_current[0]][i-1][1]);
      break;
    case 3:
      u8g.print(xjj[menu_current[0]][menu_current[1]-1][i+1]);
      break;
    }
  }

  u8g.setDefaultForegroundColor();
}

//配置模式------------------
void drawRun(int _drawRun_type,int _drawRun_num)
{
  switch(_drawRun_type)
  {
  case RUN_SWITCH:		//开关量
    {
      setFont_L;
      u8g.setFontRefHeightText();

      u8g.setPrintPos(4, 2+u8g.getFontAscent()-u8g.getFontDescent());
      u8g.print("Switch:");
      u8g.setPrintPos(4, 2*(2+u8g.getFontAscent()-u8g.getFontDescent())); 
      u8g.print(_drawRun_num ? "ON" : "OFF");
    }
    break;
  case RUN_NUM:			//数字量
    {
      setFont_L;
      u8g.setFontRefHeightText();

      u8g.setPrintPos(4, 2+u8g.getFontAscent()-u8g.getFontDescent());
      u8g.print("Num:");
      u8g.setPrintPos(4, 2*(2+u8g.getFontAscent()-u8g.getFontDescent())); 
      u8g.print(_drawRun_num);
    }
    break;
  }
}
