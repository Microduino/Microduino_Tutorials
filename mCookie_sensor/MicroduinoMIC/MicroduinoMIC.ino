#define INTERVAL_SENSOR          10 

#define buzzer_pin 6


int sensorPin = A2;    // select the input pin for the potentiometer
int sensorValue = 0;  // variable to store the value coming from the sensor

int value;

unsigned long sensor_time = millis();  

void setup() {

  Serial.begin(9600);
  pinMode(buzzer_pin,OUTPUT);
  pinMode(sensorPin,INPUT);
}

void loop() {

  //传感器采样时间间隔
  if (sensor_time > millis()) sensor_time = millis();
  if (millis() - sensor_time > INTERVAL_SENSOR) {
    updateSensorData();
    sensor_time = millis();
  }

}


void updateSensorData() {
  sensorValue = analogRead(sensorPin);
  
  value = map(sensorValue, 0, 1023, 0, 255);


  //Serial.println(value);
  tone(buzzer_pin,value);    //在端口输出频率  
  // if(value>200) {
  //   tone(soundOut,500);    //在端口输出频率
  //   //Serial.println(value);
  // }else {
  //   noTone(soundOut);
  // }

  //analogWrite(soundOut, value);
  

}