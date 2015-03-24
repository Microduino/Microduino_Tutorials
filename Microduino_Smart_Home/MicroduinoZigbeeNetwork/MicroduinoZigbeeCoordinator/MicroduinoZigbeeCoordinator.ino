#include <U8glib.h>

#include <SoftwareSerial.h>
SoftwareSerial mySerial(2, 3); // RX, TX


String myStringSerial1="";
String myString="";
int dataLength=2;

//String splitString[3];

String temp,humid,lux;

//-------字体设置，大、中、小
//#define setFont_L u8g.setFont(u8g_font_fur25)
#define setFont_L u8g.setFont(u8g_font_fur20)
#define setFont_M u8g.setFont(u8g_font_fixed_v0r)
#define setFont_S u8g.setFont(u8g_font_chikitar)
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE);//定义OLED连接方式


#define INTERVAL_LCD 10 //定义OLED刷新时间间隔 

unsigned long lcd_time = millis(); //OLED刷新时间计时器


void setup()  
{
  Serial.begin(9600);
  mySerial.begin(9600);
}

void loop() // run over and over
{
  while (mySerial.available() > 0)  
  {
    myStringSerial1 += char(mySerial.read());
    delay(2);
  }

  if (myStringSerial1.length() > 0)
  {

    myStringSerial1=myStringSerial1.substring(5);

    int offSet=0;

    offSet=myStringSerial1.indexOf(',');
    String address=myStringSerial1.substring(0,offSet);


    if(address=="7170") {
      dataLength=2;
    } else if(address=="5DCB") {
      dataLength=1;
    } else {
      dataLength=0;
    }


    splitData(dataLength, offSet);

  }
  myStringSerial1="";
  myString="";
  delay(100);


  if (lcd_time > millis()) lcd_time = millis();
  if(millis()-lcd_time>INTERVAL_LCD) {
    volcd(); //调用显示库
    lcd_time = millis();
  }


}


void splitData(int dataInfoLength, int offSet) {

  if(dataInfoLength==0) {
    return;
  }

  String splitString[dataInfoLength];

  for(int i=0; i<dataInfoLength; i++) {

    int lastOffSet=offSet;
    offSet=myStringSerial1.indexOf(',',offSet+1);
    splitString[i]=myStringSerial1.substring(lastOffSet+1,offSet);

    if(dataInfoLength==2) {
      if(i==0) {
        temp=splitString[i];
      }else if(i==1) {
        humid=splitString[i];
      }
    } else if(dataInfoLength==1) {
      lux=splitString[i];
    }



    Serial.println(splitString[i]);
  }
}


//显示函数
void volcd() {
  u8g.firstPage();
  do {
    setFont_M;
    u8g.setPrintPos(5, 10);
    u8g.print("Temp: ");
    u8g.setPrintPos(40, 10);
    u8g.print(temp);
    u8g.setPrintPos(5, 20);
    u8g.print("Humidity: ");
    u8g.setPrintPos(60, 20);
    u8g.print(humid);
    u8g.setPrintPos(5, 30);
    u8g.print("Lux: ");
    u8g.setPrintPos(40, 30);
    u8g.print(lux);
  }
  while( u8g.nextPage() );
} 