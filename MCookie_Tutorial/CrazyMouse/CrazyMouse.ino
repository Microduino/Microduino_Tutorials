#define PIN_SHACK 2
#define PIN_MOTOR 6

int sensorReading = LOW;

void setup(){
  pinMode(PIN_SHACK, INPUT);    
  pinMode(PIN_MOTOR, OUTPUT);
}

void loop(){
  sensorReading = digitalRead(PIN_SHACK);
  if(sensorReading == HIGH)
  {
    digitalWrite(PIN_MOTOR,LOW);
    delay(3000);
    digitalWrite(PIN_MOTOR,HIGH);
  }
  delay(1000);
  
}
