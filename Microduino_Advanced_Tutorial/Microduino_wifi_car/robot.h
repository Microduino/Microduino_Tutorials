#define FIX_THROTTLE_A 1
#define FIX_THROTTLE_B 1

#define REVERSE_THROTTLE 1
#define REVERSE_STEERING -1

#define MAX_THROTTLE 255 //最大油门 100~255
#define MAX_STEERING 200 //最大转向 100~512

int motor_vol[2];

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

void vorobot()
{
  //===============================================================
  int motor_speed=0;
  motor_speed=REVERSE_THROTTLE*map(angle,-510,510,-MAX_THROTTLE,MAX_THROTTLE);

  motor_vol[0]=motor_speed;
  motor_vol[1]=motor_speed;

  //----------------------------------
  int motor_steer=0;
  motor_steer=REVERSE_STEERING*map(speed,-510,510,-MAX_STEERING,MAX_STEERING);

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

  motor_vol[0]*=FIX_THROTTLE_A;
  motor_vol[1]*=FIX_THROTTLE_B;

  motor_driver(0,-motor_vol[0]);
  motor_driver(1,motor_vol[1]);
}

