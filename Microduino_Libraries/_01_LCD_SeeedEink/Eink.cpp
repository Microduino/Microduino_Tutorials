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

#include "Eink.h"
#include <SPI.h>
#include <Arduino.h> 


/* -------------------------------------------------------------
private class function, called by public fuction: clearScreen(), to init the screen;
--------------------------------------------------------------*/
void E_ink::initEink(void) //initial code
{
  writeComm(0x10);//exit deep sleep mode
  writeData(0x00);
  writeComm(0x11);//data enter mode
  writeData(0x03);
  writeComm(0x44);//set RAM x address start/end, in page 36
  writeData(0x00);//RAM x address start at 00h;
  writeData(0x11);//RAM x address end at 11h(17)->72: [because 1F(31)->128 and 12(18)->76] 
  writeComm(0x45);//set RAM y address start/end, in page 37
  writeData(0x00);//RAM y address start at 00h;
  writeData(0xAB);//RAM y address start at ABh(171)->172: [because B3(179)->180]
  writeComm(0x4E);//set RAM x address count to 0;
  writeData(0x00);
  writeComm(0x4F);//set RAM y address count to 0;
  writeData(0x00);
  writeComm(0xF0);//booster feedback used, in page 37
  writeData(0x1F);
  writeComm(0x22);//display updata sequence option ,in page 33
  writeData(0xC0);//enable sequence: clk -> CP
  
  configureLUTRegister();
  
  writeComm(0x2C);//vcom
  writeData(0xA0);
  writeComm(0x3C);//board
  writeData(0x63);
  writeComm(0x22);//display updata sequence option ,in page 33
  writeData(0xC4);//enable sequence: clk -> CP -> LUT -> initial display -> pattern display
}

/*-------------
refresh the screen, need to be called by application every time  the screen changed
--------------*/
void E_ink::refreshScreen(void)
{
  writeComm(0x20);
  closeBump();
  delay(5000);
  
  
}


/*-------------
clear the original screen, need to be called before writing any data or command to screen ;
------------*/
void E_ink::clearScreen(void)
{
   INT16U i;
   initEink();
   writeComm(0x24);
   for(i=0;i<3096;i++)
   {
     writeData(0xff);
   }
   delay(1000); 
 }
/*By the character machine code,get dot matrix data of the character*/ 
void E_ink::getCharMatrixData(INT16U unicode_Char)
{
    INT8U i;
    INT8U tempdata;
    INT32U Add=0;
    Add=calculateCharAddress(unicode_Char);
    GT_CS2_HIGH;
    delayMicroseconds(10);
    GT_CS2_LOW;
    SPI.transfer(0x03);
    SPI.transfer(Add>>16);
    SPI.transfer(Add>>8);
    SPI.transfer(Add);
    SPI.setBitOrder(LSBFIRST);
    for(i=0;i<16;i++)
    {
      tempdata=SPI.transfer(0x00);
      matrixdata[i]=(255-tempdata);
      delay(10);
    }
    SPI.setBitOrder(MSBFIRST);
    GT_CS2_HIGH;
}

/*-----By the chinese machine code ,get dot matrix data of the chinese------*/ 
void E_ink::getChineseMatrixData(INT16U unicode_Chinese)
{
    INT8U i;
    INT8U tempdata;
    INT32U Add=0;
    Add=calculateChineseAddress(unicode_Chinese);
    GT_CS2_HIGH;
    delayMicroseconds(10);
    GT_CS2_LOW;
    SPI.transfer(0x03);
    SPI.transfer(Add>>16);
    SPI.transfer(Add>>8);
    SPI.transfer(Add);
    SPI.setBitOrder(LSBFIRST);
    for(i=0;i<32;i++)
   {
      
      tempdata=SPI.transfer(0x00);
      matrixdata[i]=(255-tempdata);   /*save dot matrix data in matrixdata[i]*/
      delay(10);
    }
    SPI.setBitOrder(MSBFIRST);
    GT_CS2_HIGH;   
}


/*
calculate character address  according to the character machine code.
unicode_Char:the character machine code
return value: the address of  storing character dot matrix data
*/
INT32U E_ink::calculateCharAddress(INT16U unicode_Char)
{
    INT16U BaseAdd=0;
    INT32U Address;
    if(unicode_Char>=0x20&&unicode_Char<=0x7f)
        Address=16*(unicode_Char-0x20)+BaseAdd;
    else if(unicode_Char>=0xa0&&unicode_Char<=0xff)
        Address=16*(96+unicode_Char-0xa0)+BaseAdd;
    else if(unicode_Char>=0x100&&unicode_Char<=0x17f)
        Address=16*(192+unicode_Char-0x100)+BaseAdd;    
    else if(unicode_Char>=0x1a0&&unicode_Char<=0x1cf)
        Address=16*(320+unicode_Char-0x1a0)+BaseAdd;
    else if(unicode_Char>=0x1f0&&unicode_Char<=0x1ff)
        Address=16*(368+unicode_Char-0x1f0)+BaseAdd;
    else if(unicode_Char>=0x210&&unicode_Char<=0x21f)
        Address=16*(384+unicode_Char-0x210)+BaseAdd;
    else if(unicode_Char>=0x1ea0&&unicode_Char<=0x1eff)
        Address=16*(400+unicode_Char-0x1ea0)+BaseAdd;
    else if(unicode_Char>=0x370&&unicode_Char<=0x3cf)
        Address=16*(496+unicode_Char-0x370)+BaseAdd;
    else if(unicode_Char>=0x400&&unicode_Char<=0x45f)
        Address=16*(592+unicode_Char-0x400)+BaseAdd;    
    else if(unicode_Char>=0x490&&unicode_Char<=0x4ff)
        Address=16*(688+unicode_Char-0x490)+BaseAdd;
    else if(unicode_Char>=0x590&&unicode_Char<=0x5ff)
        Address=16*(800+unicode_Char-0x100)+BaseAdd;    
    else if(unicode_Char>=0xe00&&unicode_Char<=0xe7f)
        Address=16*(912+unicode_Char-0xe00)+BaseAdd;
    else  Address=BaseAdd;
    return Address;
}



/*
calculate chinese address  according to the Chinese machine code.
unicode_Chinese:the Chinese machine code from 0xA1A1 to 0xF7FE
return value: the address of  storing Chinese dot matrix data
*/ 
INT32U E_ink::calculateChineseAddress(INT16U unicode_Chinese)
{
    INT32U  ZFAdd,HZAdd;
    INT8U  MSB,LSB;
    INT32U ChineseTab;
    INT16U data;
    INT32U  Add_Chinese;
    MSB =unicode_Chinese>>8;
    LSB =unicode_Chinese;
    ZFAdd=36224;
    HZAdd=93452;
    ChineseTab=87436;
    if(MSB>=0xA1&&MSB<=0xA5)  //char area
    {
      if(MSB==0xA1&&LSB>=0xA1&&LSB<=0xBF)
        Add_Chinese=32*(LSB-0xA1)+ZFAdd;
      else if(MSB==0xA3&&LSB>=0xA1&&LSB<=0xFE)
        Add_Chinese=32*(31+LSB-0xA1)+ZFAdd;
      else if(MSB==0xA4&&LSB>=0xA1&&LSB<=0xF3)
        Add_Chinese=32*(125+LSB-0xA1)+ZFAdd;
      else if(MSB==0xA5&&LSB>=0xA1&&LSB<=0xF6)
        Add_Chinese=32*(208+LSB-0xA1)+ZFAdd;
      else
        Add_Chinese=ZFAdd;
    }
    else if((MSB>=0xB0&&MSB<=0xD7)&&(LSB>=0xA1&&LSB<=0xFE)) //chinese 5720
    
    { 
        Add_Chinese=(MSB-176)*94+(LSB-161)+1;
        Add_Chinese=Add_Chinese*32 +HZAdd;
        
    }
    else if((MSB>=0xD8&&MSB<=0xF7)&&(LSB>=0xA1&&LSB<=0xFE)) //chinese 5720~6763
    {
       Add_Chinese=(MSB-0xD8)*94+(LSB-0xA1);
       Add_Chinese=Add_Chinese*2+ChineseTab;
       data=GTRead(Add_Chinese);
       Add_Chinese=32*data+HZAdd;
    }
    return Add_Chinese;
}


INT16U E_ink::GTRead(INT32U Address)
{    
    INT8U i;
    INT8U buffer[2]={0};
    INT16U data;
    delayMicroseconds(10);
    GT_CS2_LOW;
    SPI.transfer(0x03);
    SPI.transfer(Address>>16);
    SPI.transfer(Address>>8);
    SPI.transfer(Address);
    for(i=0;i<2;i++)
    {
      buffer[i]=SPI.transfer(0x00);
    }
    GT_CS2_HIGH; 
    data=buffer[0]*256+buffer[1];
    delay(1000);
    return data;
 }
 
 /*
display character in the Eink screen:
x:the X start address,X value can be 0 to 14;
y:the Y start  address, Y vlue can  be 171 to 7;
the charater diplay erea is from x to x+3 in X position and from y to y-7 in Y position
unicode_char:the character machine code
*/
void E_ink::displayChar(INT8U x,INT8U y,INT16U unicode_Char)
{
  INT16U i;
  getCharMatrixData(unicode_Char);
  converCharMatrixData();
  setPositionXY(x,x+3,y,y-7);
  writeComm(0x11);     /*data enter mode command */
  writeData(0x05);     /*set Y-mode:X address is increment and y address decrement */                       
  writeComm(0x24);
  for(i=0;i<32;i++)
  {
      writeData(matrixdata_conver[i]);
   }
}


/*
display chinese in the Eink screen:
x:the X start address,X value can be 0 to 14;
y:the Y start  address, Y vlue can  be 171 to 15;
the charater erea is from x to x+3 in X position and from y to y-15 in Y position
unicode_chinese:the chinese machine code
*/
void E_ink::displayChinese(INT8U x,INT8U y,INT16U unicode_Chinese)
{
  INT16U i;
  getChineseMatrixData(unicode_Chinese);
  converChineseMatrixData();
  setPositionXY(x,x+3,y,y-15);
  writeComm(0x11);    /*data enter mode command */
  writeData(0x05);    /*set Y-mode:X increment and y decrement */  
  writeComm(0x24);
  for(i=0;i<64;i++)
  {
    writeData(matrixdata_conver[i]);
  }
}
/*
display Two-Dimensional Code in the Eink screen:
x:the X start address,X value can be 0 to 14;
y:the Y start  address, Y vlue can  be 171 to 15;
the charater erea is from x to x+6 in X position and from y to y-24 in Y position
*/
void E_ink::displayTwoDimensionalCode(INT8U x,INT8U y)
{
  INT16U i;
  writeComm(0x11);    /*data enter mode command */
  writeData(0x05);    /*set Y-mode:X increment and y decrement */
  setPositionXY(x,x+7,y,y-24);  
  converDimensionalCode();
  writeComm(0x24);
  for(i=0;i<200;i++)
  {
    writeData(matrixdata_conver[i]);
  }
}
 
/*
setPositionXY:
Xs --> X start  address   0~17
Xe --> X end    address   0~17
Ys --> Y start  address   0~171
Ye --> Y end    address   0~171
*/
void E_ink::setPositionXY(INT8U Xs, INT8U Xe,INT8U Ys,INT8U Ye)
{
  writeComm(0x44);//set RAM x address start/end  command
  writeData(Xs); 
  writeData(Xe); 
  writeComm(0x45);//set RAM y address start/end  command
  writeData(Ys); 
  writeData(Ye);
  writeComm(0x4E);//set RAM x address count to Xs;
  writeData(Xs);
  writeComm(0x4F);//set RAM y address count to Ys;
  writeData(Ys);
}


void E_ink::closeBump(void)
{
  writeComm(0x22);   
  writeData(0x03);
  writeComm(0x20);
}
 
 
void E_ink::writeComm(INT8U Command)
{
  Eink_CS1_HIGH;
  Eink_DC_LOW;
  Eink_CS1_LOW; 
  SPI.transfer(Command);
  Eink_CS1_HIGH; 
}
 
void E_ink::writeData(INT8U data)
{
  Eink_CS1_HIGH;
  Eink_DC_HIGH;
  Eink_CS1_LOW;
  SPI.transfer(data);
  Eink_CS1_HIGH;  
}

void E_ink::configureLUTRegister(void)
{
  INT8U i;
  INT8U init_data[]={
  0x00,0x00,0x00,0x55,0x00,0x00,0x55,0x55,0x00,0x55,0x55,0x55,0xAA,0xAA,0xAA,0xAA,
  0x15,0x15,0x15,0x15,0x05,0x05,0x05,0x05,0x01,0x01,0x01,0x01,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x22,0xFB,0x22,0x1B,0x00,0x00,0x00,0x00,0x00,0x00,};
  writeComm(0x32);  //write data to LUT register
  for(i=0;i<90;i++)
    writeData(init_data[i]); 
}


/*
convert the matrix data:
origital_data:it is a 8 bit data
return value:16 bit data
for example:origital_data is 10100010,and the return data will be 11001100 00001100
*/
INT16U E_ink::convertData(INT8U Original_data)
{
  INT8U i,j;
  INT8U data_L,data_H;
  INT8U temp;
  INT16U final_data;
  temp=Original_data;
  data_L=0;          /* save low  4 bit convert  result */
  data_H=0;          /* save high 4 bit convert  result */
  
  /*--------- convert low 4 bit--------*/
    for(i=0;i<4;i++)
    {
       if(temp&(0x01<<i))           /* judge the i bit if it is 1*/
        data_L |= 0x03<<(2*i);      /* convert the 2*i bit and the (2*i+1��to 1 */
       else
        data_L  &= ~(0x03<<(2*i));  /* convert the 2*i bit and the (2*i+1��to 0 */
    } 
  /*------ convert high 4 bit----------*/
    for(i=4;i< 8;i++)    
    {
        j=i-4;
      if(temp &(0x01<<i)) 
        data_H |= 0x03<<(2*j);   
      else
        data_H  &= ~(0x03<<(2*j));
    } 
  final_data =(data_H<<8)+data_L;
   return final_data;    
 }
 
/*
conver the character matrix data:
convert the array  matrixdata[] data,and save convert result in array matrixdata_conver[]; 
*/ 
void E_ink::converCharMatrixData (void)
{
  INT16U tempData;
  INT8U i,j,m;
  INT8U temp;
    for(i=0;i<8;i++)    //convert the first 8 byte data
    {
        tempData=convertData(matrixdata[i]);
        matrixdata_conver[i]=tempData>>8;          //save data at matrixdata_conver[0]~matrixdata_conver[7]
        matrixdata_conver[i+8]=tempData;           //save data at matrixdata_conver[8]~matrixdata_conver[15]
    }
    for(i=8;i<16;i++)  //convert the last 8 byte data
    {
        tempData=convertData(matrixdata[i]);
        matrixdata_conver[i+8]=tempData>>8;       //save data at matrixdata_conver[16]~matrixdata[23]
        matrixdata_conver[i+16]=tempData;         //save data at matrixdata_conver[24]~matrixdata_conver[31]
    }

  }
  
  
 
/*
conver the chinese matrix data:
convert the array  matrixdata[] data,and save convert result in array matrixdata_conver[]; 
*/
void E_ink::converChineseMatrixData(void)
{
  INT16U tempData;
  INT8U i,j,m;
  INT8U temp;
    for(i=0;i<16;i++) //convert the first 16 byte data   
    { 
      tempData=convertData(matrixdata[i]);
      matrixdata_conver[i]=tempData>>8;  //save data at matrixdata_conver[0] ~matrixdata_conver[15]
      matrixdata_conver[i+16]=tempData;  //save data at matrixdata_conver[16]~matrixdata_conver[31]
    }
    for(i=16;i<32;i++)//convert the last 16 byte data
    {
      tempData=convertData(matrixdata[i]);
      matrixdata_conver[i+16]=tempData>>8;  //save data at matrixdata_conver[32]~matrixdata_conver[47]
      matrixdata_conver[i+32]=tempData;     //save data at matrixdata_conver[48]~matrixdata_conver[63]
    }

}
/*
conver the two-dimensional data:
convert the array dimensionalData[] data,and save convert result in array matrixdata_conver[]; 
*/
void E_ink::converDimensionalCode(void)
{
  int tempData;
  INT8U i,j;
  INT8U adder,Adder;

 for(j=0;j<4;j++) 
 {
  adder=25*j; 
  Adder=adder+25;
  for(i=adder;i<Adder;i++)    
  {
    tempData=convertData(dimensionalData[i]);
        matrixdata_conver[i+adder]=tempData>>8;   
        matrixdata_conver[i+Adder]=tempData;   
  }
 
 }
 
} 

E_ink Eink;
