#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <MultiLCD.h>

LCD_SSD1306 lcd; /* for SSD1306 OLED module */


long E_count = 0; //计数值
long E_num = 0;
boolean E_sta=false;

#define text "Hello! @Microduino"

byte num[12]={
  1,8,6,7,9,2,3,8,1,2,3};

boolean  SMS_sta=false;
/**
 * The loop method gets called on and on after the start of the system.
 */

long sw=0;   //第几位号码

boolean oled_sta=true,key_sta=false;

int KEY;

void setup()
{
  Serial.begin(115200);               // the GPRS baud rate   
  Serial1.begin(115200);               // the GPRS baud rate   
  delay(100);
  //-------------------------

  Serial1.println("AT");    //Ready
  delay(200);
  Serial1.println("AT"); 
  delay(200);

  //-------------------------

  lcd.begin();
  oled_draw(num,sw,E_sta);
}


void loop()
{
  int button = analogRead(A7);
  //Serial.println(button);
  oled_sta=true;
  key_sta=true;

  if(button<40)
    KEY=2;
  else if(button>50&&button<100) 
    KEY=4;
  else if(button>110&&button<150) 
    KEY=1;
  else if(button>195&&button<235) 
    KEY=5;
  else if(button>320&&button<380) 
    KEY=3;
  else
  {
    KEY=0;   
    oled_sta=false;  
    key_sta=false;
  }

  //    Serial.println(KEY);
  delay(100);

  if(KEY==1)
    E_count++; 
  else if(KEY==5)
    E_count--;
  else if(KEY==2)
  {
    sw--;
    E_count=num[sw];
  }
  else if(KEY==4)
  {
    sw++;
    E_count=num[sw];
  }
  else if(KEY==3)
    SMS_sta=true;

  //SW================将每位上更改的号码保存=============//
  while (E_num != E_count)
  {
    if(E_count>9) E_count=0;
    if(E_count<0) E_count=9;

    E_num = E_count;

    num[sw]=E_num;
  }
  //-----------------号码长度-------------------------//
  if(sw>10)
    sw=0;
  if(sw<0)
    sw=10;

  //SMS===============发送短信并显示=========//
  if(SMS_sta)
  {
    SMS_sms(num,text);  

    oled_sms(num,text);

    SMS_sta=false;
    delay(3000); 

    oled_sta=true;

    sw=0;
  }

  //OLED===============================
  if(oled_sta)    //MENU
  {
    oled_draw(num,sw,E_sta);
    delay(50);

    oled_sta=false;
  }
}
//=====================AT命令发送短信===============================//
void SMS_sms(byte* c_num,char* c_text)
{
  Serial1.print("AT+CMGF=1\r");    //Because we want to send the SMS in text mode
  delay(100);
  while(Serial1.available()) Serial.write(Serial1.read());

  Serial1.print("AT+CMGS=\"+86");
  for(int a=0;a<11;a++)
    Serial1.print(c_num[a]);
  Serial1.println("\"");
  delay(100);

  Serial1.println(c_text);//the content of the message
  delay(100);

  Serial1.println((char)26);//the ASCII code of the ctrl+z is 26
  delay(100);

  Serial1.println();
  while(Serial1.available()) Serial.write(Serial1.read());
}
//=====================短信发送显示界面函数===============================//
void oled_sms(byte* c_num,char* c_text)
{
  lcd.clear();
  lcd.setCursor(9, 3);
  lcd.setFont(FONT_SIZE_MEDIUM);

  for(int a=0;a<11;a++)
    lcd.print(c_num[a]);

  lcd.setFont(FONT_SIZE_SMALL);

  lcd.setCursor(9, 6);
  lcd.print(c_text);
}
//=======================设置号码显示界面函数=============================//
void oled_draw(byte* c_num,int i_num,boolean E_sta) 
{
  if(key_sta)
  {
    lcd.clear();
  }
  lcd.setCursor(9+0, 3);
  lcd.setFont(FONT_SIZE_MEDIUM);

  for(int a=0;a<i_num;a++)
    lcd.print(c_num[a]);

  if(!E_sta)
  {
    lcd.setCursor(9+i_num*9, 1);
    if(c_num[i_num]>0)
      lcd.print(c_num[i_num]-1);
    else
      lcd.print(9-c_num[i_num]);
  }  
  lcd.setCursor(9+i_num*9, 3);
  lcd.print(c_num[i_num]);
  if(!E_sta)
  {
    lcd.setCursor(9+i_num*9, 5);
    if(c_num[i_num]<9)
      lcd.print(c_num[i_num]+1);
    else
      lcd.print(9-c_num[i_num]);
  }

  lcd.setCursor(9+9+i_num*9, 3);
  for(int b=i_num+1;b<11;b++)
    lcd.print(c_num[b]);
}

