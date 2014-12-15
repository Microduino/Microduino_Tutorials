#include "Arduino.h"

boolean sta=false;	//配置模式
int type=0;		//配置模式类型
int num=0;		//配置模式参数

#define MENU_LEVEL 3	//菜单等级，固定值

uint8_t menu_level = 0;	//菜单等级，实时
uint8_t menu_current[MENU_LEVEL] = //菜单光标指向
{
  1,1,1
};

int menu_num_level1;	                        //菜单第一层状态，初始化于osd_smart_init()
int menu_num_level2[MENU_ITEMS1];        	//菜单第二层状态，初始化于osd_smart_init()
int menu_num_level3[MENU_ITEMS1][MENU_ITEMS2];	//菜单第三层状态，初始化于osd_smart_init()

//菜单光标指向等级判断
boolean set_lev(int _s_lev1,int _s_lev2,int _s_lev3)
{
  /*
   //Serial.println("");
   //Serial.print("menu_current 1,2,3:");
   //Serial.print(menu_current[0]);
   //Serial.print(",");
   //Serial.print(menu_current[1]);
   //Serial.print(",");
   //Serial.println(menu_current[2]);
   //Serial.println("-------");
   
   //Serial.print("_s_lev 1,2,3:");
   //Serial.print(_s_lev1);
   //Serial.print(",");
   //Serial.print(_s_lev2);
   //Serial.print(",");
   //Serial.println(_s_lev3);
   //Serial.println("");
   
   //Serial.print("menu_num_level 1,2,3:");
   //Serial.print(menu_num_level1);
   //Serial.print(",");
   //Serial.print(menu_num_level2[_s_lev1]);
   //Serial.print(",");
   //Serial.println(menu_num_level3[_s_lev1][_s_lev2-1]);
   //Serial.println("");
   */

  int p;

  if(_s_lev1 && _s_lev1 <= menu_num_level1)
    p=1;
  if(_s_lev2 && _s_lev2 <= menu_num_level2[_s_lev1])
    p=2;
  if(_s_lev3 && _s_lev3 <= menu_num_level3[_s_lev1][_s_lev2-1])
    p=3;

  switch(p)
  {
  case 1:
    if(_s_lev1 ==  menu_current[0]) 
    {
      //Serial.print("p1");
      //Serial.println(p);
      //Serial.println();
      return true;
    }   
    else  return false;
    break;
  case 2: 
    if(_s_lev1 ==  menu_current[0] && _s_lev2 ==  menu_current[1])  
    {
      //Serial.print("p2");
      //Serial.println(p);
      //Serial.println();      
      return true;
    }
    else  return false;
    break;
  case 3: 
    if(_s_lev1 ==  menu_current[0] && _s_lev2 ==  menu_current[1] && _s_lev3 ==  menu_current[2])
    {
      //Serial.print("p3");
      //Serial.println(p);
      //Serial.println();      
      return true;
    }
    else  return false;
    break;
  default:
    //Serial.print("pNO");
    //Serial.println(p);
    //Serial.println();
    return false;
  }
}

//菜单光标指向等级判断
int change(boolean b_change,int i_change,int _lev1,int _lev2,int _lev3)
{
  int abc=3;
  if(!_lev3) abc=2;
  if(!_lev2) abc=1;
  if(!_lev1) return 0;
  //Serial.print("i_change ");
  //Serial.print(i_change);
  if(!b_change || menu_level > abc)  if(set_lev(_lev1,_lev2,_lev3))
  {
    type=i_change;
    //Serial.println("is OK--");
    return abc;
  }
  //Serial.println("is ERROR--");
  return 0;
}

