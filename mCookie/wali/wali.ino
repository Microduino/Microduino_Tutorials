#define motor_pin0A 5 //PWM
#define motor_pin0B 7
#define motor_pin1A 6  //PWM 
#define motor_pin1B 8

const int TrigPin = 10;
const int EchoPin = 11;

#define _motor_driver_vol 80

float cm; 
long data;

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

  if(cm<8)
  {
    data+=1;
    analogWrite(motor_pin0A,_motor_driver_vol);
    //digitalWrite(motor_pin0A,HIGH);
    digitalWrite(motor_pin0B,LOW);
    analogWrite(motor_pin1A,_motor_driver_vol);
    //digitalWrite(motor_pin1A,HIGH);
    digitalWrite(motor_pin1B,LOW);
  }
  else
  {
    if(data>0)
    {
      data-=3;
      analogWrite(motor_pin0A,_motor_driver_vol+90);
      //digitalWrite(motor_pin0A,LOW);
      digitalWrite(motor_pin0B,HIGH);
      //digitalWrite(motor_pin1A,LOW);
      analogWrite(motor_pin1A,_motor_driver_vol+90);
      digitalWrite(motor_pin1B,HIGH);
    }
    else
    {
      data=0;
      digitalWrite(motor_pin0B,LOW);
      digitalWrite(motor_pin0A,LOW);
      digitalWrite(motor_pin1B,LOW);
      digitalWrite(motor_pin1A,LOW);
    }
  }
  Serial.print("Time:");
  Serial.println(data);
  Serial.print("Length:");
  Serial.println(cm);
}


