#include <SoftwareSerial.h>

SoftwareSerial mySerial(2, 3); //RX,TX

//#define my_Serial mySerial  //core
#define my_Serial Serial1  //Core+,CoreUSB

#define servo_pin 6  //PWM

#define motor_pin 7  //PWM 

int pos = 1950;    // variable to store the servo position 

#define MAX_STEERING 250 //最大转向 0~90

#include <Servo.h> 

Servo  throttle_servo;  // create servo object to control a servo 
Servo  steering_servo;  // create servo object to control a servo 

String currentInfo;

char senderData[100];

int angle=90, speed=0;

int robot_throttle=0;
int robot_steering=90;

unsigned long safe_ms=millis();

void setup() 
{
  Serial.begin(115200);
  my_Serial.begin(9600); 

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
  ble();

  vorobot();

  //===============================================================
  if(safe_ms>millis()) safe_ms=millis();
  if(millis()-safe_ms>5000)
  {
    robot_steering=90;
    robot_throttle=0;
    safe_ms=millis();
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
  throttle_servo.writeMicroseconds(map(robot_throttle,255,-255,1000,2000));//head

  //steering_servo.writeMicroseconds(map(robot_steering,0,180,pos-MAX_STEERING,pos+MAX_STEERING));
  //steering_servo.writeMicroseconds(map(robot_steering,0,180,4000,1000));
  steering_servo.write(robot_steering);
}

void ble()
{
  if (my_Serial.available() > 0) {
    currentInfo = my_Serial.readStringUntil('\n');

    if(currentInfo=="ERROR"||currentInfo=="Connected") {
      return;
    }
    else
    {
      if(currentInfo!=NULL) {

        currentInfo.toCharArray(senderData, sizeof(senderData)); 
        sscanf((char *)strstr((char *)senderData, "D:"), "D:%d,%d", &angle,&speed);

        robot_throttle=speed;
        robot_steering=angle;
      }
    }
  }
}
