#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <MultiLCD.h>

LCD_SSD1306 lcd; /* for SSD1306 OLED module */


unsigned long E_time = 0; 
long E_count = 0; //计数值
long E_num = 0;
#define E_enter 6
boolean E_sta=false;


boolean key1;			//按键
boolean key1_cache;		//检测按键松开缓存

#define text "Hello! Microduino"

boolean   SMS_ready=false,SMS_sta=false;

byte num[12]={
  1,8,6,7,9,2,3,6,6,9,5,0};
/**
 * The loop method gets called on and on after the start of the system.
 */

long sw=0,sw_cache=0;

boolean oled_sta=true;



void setup()
{
  Serial.begin(115200);               // the GPRS baud rate   
  delay(100);
  //-------------------------

  pinMode(2,INPUT_PULLUP); //D2脚为输入
  pinMode(3,INPUT_PULLUP); //D3脚为输入
  pinMode(E_enter,INPUT_PULLUP); //D3脚为输入

  attachInterrupt(0, blinkA, LOW);  //注册中断0调用函数blinkA
  attachInterrupt(1, blinkB, LOW);  //注册中断1调用函数blinkB

  E_time = millis(); //时间初值

  lcd.begin();
}


void loop()
{
  if(oled_sta)    //MENU
  {
    oled_draw(num,sw,E_sta);
    delay(50);

    oled_sta=false;
  }

  //KEY============================
  key1=digitalRead(E_enter);

  if(key1 && key1_cache)		//按下松开后
  {
    sw_cache++;
    if(sw_cache>11)
      sw_cache=0;
    //------------------------
    E_sta=boolean(sw_cache==11);

    if(!E_sta) 
      sw=sw_cache;

    /*
Serial.print("sw[ ");
     Serial.print(sw);
     Serial.print("] :");
     for(int a=0;a<11;a++)
     Serial.print(num[a]);
     Serial.println(" ");
     */
    //-------------------------
    if(SMS_ready)
    {
      SMS_ready=false;
      SMS_sta=true;
    }
    //--------------------
    E_count=num[sw];    
    //--------------------

    delay(5);

    oled_sta=true;
  }

  key1_cache=!key1;		//缓存作判断用

  //SMS=================================
  if(SMS_sta)
  {
    SMS_sms(num,text);  

    oled_sms(num,text);

    delay(3000); 
    SMS_sta=false;
  }
  //SW=================================
  while (E_num != E_count)
  {
    if(E_count>9) E_count=0;
    if(E_count<0) E_count=9;

    E_num = E_count;

    if(!E_sta)
      num[sw]=E_num;
    else
      SMS_ready=!SMS_ready;

    oled_sta=true;
  }

}


void SMS_sms(byte* c_num,char* c_text)
{
  Serial.print("AT+CMGF=1\r");    //Because we want to send the SMS in text mode
  delay(100);
  while(Serial.available()) char cache=Serial.read();

  Serial.print("AT+CMGS=\"+86");
  for(int a=0;a<11;a++)
    Serial.print(c_num[a]);
  Serial.println("\"");
  delay(100);

  Serial.println(c_text);//the content of the message
  delay(100);

  Serial.println((char)26);//the ASCII code of the ctrl+z is 26
  delay(100);

  Serial.println();
  while(Serial.available()) char cache=Serial.read();
}


void oled_sms(byte* c_num,char* c_text)
{
  lcd.clear();
  lcd.setCursor(0, 3);
  lcd.setFont(FONT_SIZE_MEDIUM);

  for(int a=0;a<11;a++)
    lcd.print(c_num[a]);

  lcd.setFont(FONT_SIZE_SMALL);

  lcd.setCursor(0, 6);
  lcd.print(c_text);
}


void oled_draw(byte* c_num,int i_num,boolean b_sta) 
{
  lcd.clear();
  lcd.setCursor(0, 3);
  lcd.setFont(FONT_SIZE_MEDIUM);

  for(int a=0;a<i_num;a++)
    lcd.print(c_num[a]);

  if(!E_sta)
  {
    lcd.setCursor(i_num*9, 1);
    if(c_num[i_num]>0)
      lcd.print(c_num[i_num]-1);
    else
      lcd.print(9-c_num[i_num]);
  }  
  lcd.setCursor(i_num*9, 3);
  lcd.print(c_num[i_num]);
  if(!E_sta)
  {
    lcd.setCursor(i_num*9, 5);
    if(c_num[i_num]<9)
      lcd.print(c_num[i_num]+1);
    else
      lcd.print(9-c_num[i_num]);
  }


  lcd.setCursor(9+i_num*9, 3);
  for(int b=i_num+1;b<11;b++)
    lcd.print(c_num[b]);

  //-----------------------------
  if(!E_sta)
    lcd.print("<?>");
  else
  {
    lcd.print("[");
    if(SMS_ready)
      lcd.print("Y");
    else
      lcd.print("N");
    lcd.print("]");
  }
}

//中断0调用函数
void blinkA()
{
  if ((millis() - E_time) > 2) //防抖动处理
    E_count ++; 
  E_time = millis();
}

//中断1调用函数
void blinkB()
{
  if ((millis() - E_time) > 2)  //防抖动处理
    E_count --;
  E_time = millis();
}
