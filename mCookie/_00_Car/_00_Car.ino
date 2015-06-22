#define servo_pin 7  //PWM

#define motor_pin 8  //PWM 

int pos = 1950;    // variable to store the servo position 

#define MAX_STEERING 250 //最大转向 0~90

#include <Servo.h> 

Servo  throttle_servo;  // create servo object to control a servo 
Servo  steering_servo;  // create servo object to control a servo 

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

  //nRF==============================
  SPI.begin();		//初始化SPI总线
  radio.begin();
  network.begin(/*channel*/ 70, /*node address*/ this_node);

  throttle_servo.attach(motor_pin);  // attaches the servo on pin 9 to the servo object 
  steering_servo.attach(servo_pin);  // attaches the servo on pin 9 to the servo object 

  //  throttle_servo.writeMicroseconds(1500);              // tell servo to go to position in variable 'pos' 
  steering_servo.write(pos);              // tell servo to go to position in variable 'pos' 

  Serial.println("===========start===========");
}


int motor_vol[2];
// 主循环//////////////////////////////////////////////////////////////////////////
void loop() 
{	   
  //===============================================================
  if(nRF())
  {
    vorobot();
  }

  //===============================================================
  if(safe_ms>millis()) safe_ms=millis();
  if(millis()-safe_ms>1000)
  {
    robot_steering=1500;
    robot_throttle=1500;

    vorobot();
  }
}

void vorobot()
{
  Serial.print(robot_throttle);
  Serial.print(",");
  Serial.print(robot_steering);
  Serial.println("");

  //----------------------------------
  throttle_servo.writeMicroseconds(map(robot_throttle,1000,2000,1000,2000));

  steering_servo.writeMicroseconds(map(robot_steering,1000,2000,pos-MAX_STEERING,pos+MAX_STEERING));
}


boolean nRF()
{
  boolean ok;
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
      send_a sen = { millis() };	//把这些数据发送出去，对应前面的发送数组
      RF24NetworkHeader header(0);
      ok = network.write(header,&sen,sizeof(sen));
    }

    safe_ms=millis();

    return ok;
  }
}



