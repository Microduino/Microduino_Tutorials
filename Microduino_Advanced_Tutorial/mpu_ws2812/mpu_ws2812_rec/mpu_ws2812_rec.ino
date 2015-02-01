//rf=======================================
#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>

// nRF24L01(+) radio attached using Getting Started board 
RF24 radio(9,10);
RF24Network network(radio);
const uint16_t this_node = 1;	//设置本机ID
const uint16_t other_node=0;

#include <Adafruit_NeoPixel.h>

#define PIN A0

Adafruit_NeoPixel strip = Adafruit_NeoPixel(20, PIN, NEO_GRB + NEO_KHZ800);

//--------------------------------
struct send_a	//发送
{
  uint32_t node_ms;		//节点运行时间
};

unsigned long last_sent=0;	//定时器

//--------------------------------
struct receive_a	//接收
{
  uint32_t ms;
  uint32_t rf_x;
  uint32_t rf_y;
  uint32_t rf_z;
};

unsigned long clock;	//主机运行时间
int tem_xuan=0;		//主机请求时序

//----------------------------
boolean node_STA=false;

int Angel_accX,Angel_accY,Angel_accZ;

unsigned long safe_ms=millis();

void setup() 
{
  Serial.begin(115200);

  strip.begin();
  strip.show(); // Initialize all pixels to 'off'

  //nRF==============================
  SPI.begin();		//初始化SPI总线
  radio.begin();
  network.begin(/*channel*/ 70, /*node address*/ this_node);

  Serial.println("===========start===========");
}

// 主循环//////////////////////////////////////////////////////////////////////////
void loop() 
{	   
  //===============================================================
  nRF();

  //===============================================================
  if(safe_ms>millis()) safe_ms=millis();
  if(millis()-safe_ms>2000)
  {
    colorWipe(strip.Color(0, 0, 0), 50);
  }

}

void nRF()
{
  network.update();
  // Is there anything ready for us?
  while ( network.available() )
  {
    // If so, grab it and print it out
    RF24NetworkHeader header;
    receive_a rec;
    network.read(header,&rec,sizeof(rec));

    clock=rec.ms;		//接收主机运行时间赋值
    Angel_accX=rec.rf_x;		//接收请求时序赋值
    Angel_accY=rec.rf_y;
    Angel_accZ=rec.rf_z;

    Serial.print(Angel_accX);
    Serial.print(",");
    Serial.print(Angel_accY);
    Serial.print(",");
    Serial.print(Angel_accZ);
    Serial.println("");

    colorWipe(strip.Color(Angel_accX, Angel_accY, Angel_accZ), 10);

    {
      //Serial.print("Sending...");
      send_a sen = {
        millis()
        };	//把这些数据发送出去，对应前面的发送数组

        RF24NetworkHeader header(0);
      boolean ok = network.write(header,&sen,sizeof(sen));

    }

    safe_ms=millis();
  }
}

void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}
