#define motor_pin0A 7  //PWM
#define motor_pin0B 5
#define motor_pin1A 8  //PWM 
#define motor_pin1B 6

#define MAX_THROTTLE 255 //最大油门 100~255
#define MAX_STEERING 400 //最大转向 100~512

//rf=======================================
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

unsigned long safe_ms=millis();

void setup() 
{
  Serial.begin(115200);

  pinMode(motor_pin0A,OUTPUT);
  pinMode(motor_pin0B,OUTPUT);
  pinMode(motor_pin1A,OUTPUT);
  pinMode(motor_pin1B,OUTPUT);

  //nRF==============================
  SPI.begin();		//初始化SPI总线
  radio.begin();
  network.begin(/*channel*/ 70, /*node address*/ this_node);

  Serial.println("===========start===========");
}


int motor_vol[2];
// 主循环//////////////////////////////////////////////////////////////////////////
void loop() 
{	   
  //===============================================================
  nRF();

  //===============================================================
  if(safe_ms>millis()) safe_ms=millis();
  if(millis()-safe_ms>2000)
  {
    robot_steering=0;
    robot_throttle=0;

    digitalWrite(motor_pin0A,LOW);
    digitalWrite(motor_pin0B,LOW);
    digitalWrite(motor_pin1A,LOW);
    digitalWrite(motor_pin1B,LOW);
  }

}

void vorobot()
{
  /*
  if(node_STA)
   {

   }
   */

  //===============================================================
  int motor_speed=0;
  motor_speed=map(robot_throttle,1000,2000,-MAX_THROTTLE,MAX_THROTTLE);

  motor_vol[0]=motor_speed;
  motor_vol[1]=motor_speed;

  //----------------------------------
  int motor_steer=0;
  motor_steer=map(robot_steering,1000,2000,-MAX_STEERING,MAX_STEERING);

  motor_vol[0]-=motor_steer/2;
  motor_vol[1]+=motor_steer/2;

  for(int a=0;a<2;a++)
  {
    if(motor_vol[a] > MAX_THROTTLE)
    {
      motor_vol[0]-=(motor_vol[a]-MAX_THROTTLE);
      motor_vol[1]-=(motor_vol[a]-MAX_THROTTLE);
    }
    else if(motor_vol[a] < -MAX_THROTTLE)
    {
      motor_vol[0]-=(MAX_THROTTLE+motor_vol[a]);
      motor_vol[1]-=(MAX_THROTTLE+motor_vol[a]);
    }
  }

  Serial.print(motor_vol[0]);
  Serial.print(",");
  Serial.print(motor_vol[1]);
  Serial.println("");

  motor_driver(0,-motor_vol[0]);
  motor_driver(1,motor_vol[1]);
}

boolean motor_driver(int _motor_driver_num,int _motor_driver_vol)
{
  switch(_motor_driver_num)
  {
  case 0:
    if(_motor_driver_vol==0)
    {
      //Serial.println("0 OFF");
      digitalWrite(motor_pin0A,LOW);
      digitalWrite(motor_pin0B,LOW);
    }
    else if(_motor_driver_vol>0)
    {
      //Serial.println("0 Z");
      analogWrite(motor_pin0A,_motor_driver_vol);
      digitalWrite(motor_pin0B,LOW);
    }
    else
    {
      //Serial.println("0 F");
      analogWrite(motor_pin0A,255+_motor_driver_vol);
      digitalWrite(motor_pin0B,HIGH);
    }
    break;
  case 1:
    if(_motor_driver_vol==0)
    {
      //Serial.println("1 OFF");
      analogWrite(motor_pin1A,_motor_driver_vol);
      digitalWrite(motor_pin1B,LOW);
    }
    else if(_motor_driver_vol>0)
    {
      //Serial.println("1 Z");
      analogWrite(motor_pin1A,_motor_driver_vol);
      digitalWrite(motor_pin1B,LOW);
    }
    else
    {
      //Serial.println("1 F");
      analogWrite(motor_pin1A,255+_motor_driver_vol);
      digitalWrite(motor_pin1B,HIGH);
    }
    break;
  default :
    return false;
  }
  return true;
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

    //Serial.print(robot_throttle);
    //Serial.print(",");
    //Serial.print(robot_steering);
    //Serial.println("");

    {
      //Serial.print("Sending...");
      send_a sen = {
        millis()
        };	//把这些数据发送出去，对应前面的发送数组

        RF24NetworkHeader header(0);
      boolean ok = network.write(header,&sen,sizeof(sen));
      if (ok)
      {
        //Serial.println("ok.");
        vorobot();
      }

      else
      {
        //Serial.println("failed.");
      }
    }

    safe_ms=millis();
  }
}
