/*
OUT1A========>>Right-1
 OUT1B========>>Right-2
 OUT2A========>>Left-1
 OUT2B========>>Left-2
 */
//===========Motor A=============//
int mic_left = 6;
int mic_right = 8;
int mic_head = 5;
int mic_back = 7;

//===========Motor B=============//
//int mic_left = 19;
//int mic_right = 23;
//int mic_head = 18;
//int mic_back = 22;

#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>

// nRF24L01(+) radio attached using Getting Started board 
RF24 radio(9,10);
RF24Network network(radio);
const uint16_t this_node = 1;	//设置本机ID
const uint16_t other_node=0;

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
  uint32_t rf_STA;
  uint32_t rf_throttle;
  uint32_t rf_steering;
};

unsigned long clock;	//主机运行时间
int tem_xuan=0;			//主机请求时序

//----------------------------
boolean node_STA=false;
int robot_throttle;
int robot_steering;

void setup() 
{
  Serial.begin(115200);

  pinMode(mic_left,OUTPUT);
  pinMode(mic_right,OUTPUT);
  pinMode(mic_head,OUTPUT);
  pinMode(mic_back,OUTPUT);

  //nRF==============================
  SPI.begin();		//初始化SPI总线
  radio.begin();
  network.begin(/*channel*/ 70, /*node address*/ this_node);

  Serial.println("===========start===========");
}
// 主循环//////////////////////////////////////////////////////////////////////////+++++++++++++++++++++++++++++++++++++++++++++++++++++++
void loop() 
{	   
  //===============================================================
  nRF();

  robot();
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
    node_STA=rec.rf_STA;		//接收请求时序赋值
    robot_throttle=rec.rf_throttle;
    robot_steering=rec.rf_steering;

    {
      Serial.print("Sending...");
      send_a sen = {
        millis()
        };	//把这些数据发送出去，对应前面的发送数组

        RF24NetworkHeader header(0);
      boolean ok = network.write(header,&sen,sizeof(sen));
      if (ok)
        Serial.println("ok.");

      else
        Serial.println("failed.");
    }
  }
}

void robot()
{
  if(robot_throttle==1500&&robot_steering==1500||robot_throttle==0||robot_steering==0)
    stoop();
  else
  {
    if(robot_throttle>1500)
      head();
    else if(robot_throttle<1500)
      back();
    if(robot_steering>1500)
      right();
    else if(robot_steering<1500)
      left();
  }
}

void head()
{
  digitalWrite(mic_left,LOW);
  digitalWrite(mic_right,HIGH);
  // analogWrite(mic_right, 100);
  digitalWrite(mic_head,HIGH);
  // analogWrite(mic_head, 100);
  digitalWrite(mic_back,LOW);
}

void back()
{
  digitalWrite(mic_left,HIGH);
  digitalWrite(mic_right,LOW);
  // analogWrite(mic_right, 100);
  digitalWrite(mic_head,LOW);
  // analogWrite(mic_head, 100);
  digitalWrite(mic_back,HIGH);
}

void left()
{
  digitalWrite(mic_left,HIGH);
  // analogWrite(mic_left, val_L); 
  digitalWrite(mic_right,LOW);
  digitalWrite(mic_head,HIGH);
  //analogWrite(mic_head, val_R); 
  digitalWrite(mic_back,LOW  );
}

void right()
{
  digitalWrite(mic_left,LOW);
  digitalWrite(mic_right,HIGH);
  //analogWrite(mic_right, val_L);
  digitalWrite(mic_head,LOW);
  digitalWrite(mic_back,HIGH);
  //analogWrite(mic_back, val_R); 
}

void stoop()
{
  digitalWrite(mic_left,LOW);
  digitalWrite(mic_right,LOW);
  digitalWrite(mic_head,LOW);
  digitalWrite(mic_back,LOW);
}









