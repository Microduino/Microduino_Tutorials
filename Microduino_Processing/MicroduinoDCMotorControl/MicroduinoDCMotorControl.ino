int motorPin = 9;//Motor pin
int leitura = 0;//Defined potentiometer value
int vel = 0;//Value recorded motor

void setup() {
  Serial.begin(9600);
    pinMode(motorPin,OUTPUT);  
}
void loop() {
  if(Serial.available())//if serial is available
  {
    leitura=Serial.read();//read serial data
    Serial.println(leitura);
    vel=map(leitura,0,255,0,1023);//map
    if(leitura>0)//run motor by leitura
    {
      analogWrite(motorPin,vel);
      delay(1);
    }
    else//stop motor
    {
      analogWrite(motorPin,LOW);
    }
  }
}
