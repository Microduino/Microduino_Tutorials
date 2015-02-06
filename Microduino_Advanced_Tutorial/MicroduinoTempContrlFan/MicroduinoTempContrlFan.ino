int fanpwmPin = 7;                // fanpwm pin
int tempValue, Output;
int pre=0;
int Setpoint = 23;                //inital power on fan above 23

void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  pinMode(fanpwmPin, OUTPUT);

}

// the loop routine runs over and over again forever:
void loop() {
  int n = analogRead(A0); //Read the voltage value of pin A0
  tempValue = n * (5.0 / 1023.0*100); //Storage temperature data, temperature data obtained from the voltage values ​​are translated
  

  if(tempValue>Setpoint) {//if tempValue above setpoint

    if(pre<Setpoint) {//power up fan when stop to power on, some time small pwm value can not make fan circle around
      analogWrite(fanpwmPin,200);
      delay(500);
    }
    Output=convertOutput(tempValue);
    analogWrite(fanpwmPin,Output);
  }else {
    analogWrite(fanpwmPin,0);
  }

/*
    Serial.print(tempValue);
    Serial.print("\t");
    Serial.print(pre);
    Serial.print("\t");
    Serial.println(Output);
*/
    pre=tempValue;
    delay(1000);
  }

  int convertOutput(int temperature) {
    Output = map(temperature, Setpoint, 50, 0, 255);
    return Output+100;
  }
