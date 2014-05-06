/*
 2012 Copyright (c) Seeed Technology Inc.

 Author: Zhangkun  
 
 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.

 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc.,51 Franklin St,Fifth Floor, Boston, MA 02110-1301 USA

*/

#ifndef Eink_h
#define Eink_h
 
#ifndef INT8U
#define INT8U unsigned char
#endif
#ifndef INT16U
#define INT16U unsigned int
#endif
#ifndef INT32U
#define INT32U unsigned long
#endif
#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)

#define Eink_CS1_LOW  {DDRH |= 0x08;PORTH &=~ 0x08;}
#define Eink_CS1_HIGH {DDRH |= 0x08;PORTH |=  0x08;}
#define Eink_DC_LOW   {DDRE |= 0x08;PORTE &=~ 0x08;}
#define Eink_DC_HIGH  {DDRE |= 0x08;PORTE |=  0x08;}
#define GT_CS2_LOW    {DDRG |= 0x20;PORTG &=~ 0x20;}
#define GT_CS2_HIGH   {DDRG |= 0x20;PORTG |=  0x20;} 
 
#elif defined(__AVR_ATmega32U4__)

#define Eink_CS1_LOW  {DDRD |= 0x80;PORTD &=~ 0x80;}
#define Eink_CS1_HIGH {DDRD |= 0x80;PORTD |=  0x80;}
#define Eink_DC_LOW   {DDRC |= 0x40;PORTC &=~ 0x40;}
#define Eink_DC_HIGH  {DDRC |= 0x40;PORTC |=  0x40;}
#define GT_CS2_LOW    {DDRD |= 0x10;PORTD &=~ 0x10;}
#define GT_CS2_HIGH   {DDRD |= 0x10;PORTD |=  0x10;} 

#else

#define Eink_CS1_LOW  {DDRD |= 0x40;PORTD &=~ 0x40;}
#define Eink_CS1_HIGH {DDRD |= 0x40;PORTD |=  0x40;}
#define Eink_DC_LOW   {DDRD |= 0x20;PORTD &=~ 0x20;}
#define Eink_DC_HIGH  {DDRD |= 0x20;PORTD |=  0x20;}
#define GT_CS2_LOW    {DDRD |= 0x10;PORTD &=~ 0x10;}
#define GT_CS2_HIGH   {DDRD |= 0x10;PORTD |=  0x10;} 


#endif
class E_ink
{
public:
 void clearScreen(void);
 void refreshScreen(void);
 void getCharMatrixData(INT16U unicode_Char);
 void getChineseMatrixData(INT16U unicode_Chinese);
 void converCharMatrixData (void);
 void converChineseMatrixData(void);
 void displayChar(INT8U x1,INT8U y1,INT16U unicode_Char);
 void displayChinese(INT8U x1,INT8U y1,INT16U unicode_Chinese);
 void displayTwoDimensionalCode(INT8U x,INT8U y);
 
private:
 INT8U matrixdata[32];
 INT8U matrixdata_conver[200]; 
 
 void  writeComm(INT8U Command);
 void  writeData(INT8U data);
 INT16U  GTRead(INT32U Address);
 INT16U convertData(INT8U Original_data); 
 void  converDimensionalCode(void);
 
 void  initEink(void);
 void  closeBump(void);
 void  configureLUTRegister(void);
 void  setPositionXY(INT8U Xs, INT8U Xe,INT8U Ys,INT8U Ye);
 
 INT32U  calculateCharAddress(INT16U unicode_Char);
 INT32U  calculateChineseAddress(INT16U unicode_Chinese);
};

extern E_ink Eink;
extern INT8U dimensionalData[];
#endif
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
  
