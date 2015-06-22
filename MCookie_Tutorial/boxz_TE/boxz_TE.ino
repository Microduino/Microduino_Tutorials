#define motor_pin0A 5 //PWM
#define motor_pin0B 7
#define motor_pin1A 6  //PWM 
#define motor_pin1B 8

#define FIX_THROTTLE_A 1
#define FIX_THROTTLE_B 1

#define REVERSE_THROTTLE 1
#define REVERSE_STEERING -1

#define MAX_THROTTLE 255 //最大油门 100~255
#define MAX_STEERING 200 //最大转向 100~512

int  val; 
int num;

int robot_throttle;
int robot_steering;

int motor_vol[2];

const int TrigPin = 10;
const int EchoPin = 11;

float cm;

void setup()
{
  Serial.begin(115200);
  pinMode(TrigPin, OUTPUT);
  pinMode(EchoPin, INPUT);

  pinMode(motor_pin0A,OUTPUT);
  pinMode(motor_pin0B,OUTPUT);
  pinMode(motor_pin1A,OUTPUT);
  pinMode(motor_pin1B,OUTPUT);
}

void loop()
{
  digitalWrite(TrigPin, LOW); 
  delayMicroseconds(2);
  digitalWrite(TrigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(TrigPin, LOW);

  cm = pulseIn(EchoPin, HIGH) / 58.0; //Echo time will be converted into cm
  cm = (int(cm * 100.0)) / 100.0; //Two decimal places
  //Serial.println(cm);

  if(cm<6)
  {
    robot_throttle=1300;
    robot_steering=1500;
    vorobot();
    delay(800);
    robot_throttle=1500;
    robot_steering=1000;
    vorobot();
    delay(600);
  }
  else
  {
    robot_throttle=1700;
    robot_steering=1500;
  }

  vorobot();
}

void vorobot()
{
  //===============================================================
  int motor_speed=0;
  motor_speed=REVERSE_THROTTLE*map(robot_throttle,1000,2000,-MAX_THROTTLE,MAX_THROTTLE);

  motor_vol[0]=motor_speed;
  motor_vol[1]=motor_speed;

  //----------------------------------
  int motor_steer=0;
  motor_steer=REVERSE_STEERING*map(robot_steering,1000,2000,-MAX_STEERING,MAX_STEERING);

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
  /*
  Serial.print(motor_vol[0]);
   Serial.print(",");
   Serial.print(motor_vol[1]);
   Serial.println("");
   */
  motor_vol[0]*=FIX_THROTTLE_A;
  motor_vol[1]*=FIX_THROTTLE_B;

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
      digitalWrite(motor_pin1A,LOW);
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
