int potPin = A0; //Potentiometer input pin
int motorPin = 9;//Motor pin
int potValue = 0;//Defined potentiometer value
int motorValue = 0;//Value recorded motor
void setup() {
 Serial.begin(9600);
}
void loop() {
 potValue = analogRead(potPin); //Reading the value of the potentiometer
 motorValue = map(potValue, 0, 1023, 0, 255);//Mapped to a value between 0 and 255
 analogWrite(motorPin, motorValue);//PWM pass motor 
 //display
 Serial.print("potentiometer = " );
 Serial.print(potValue);
 Serial.print("\t motor = ");
 Serial.println(motorValue);
 delay(2);
}
