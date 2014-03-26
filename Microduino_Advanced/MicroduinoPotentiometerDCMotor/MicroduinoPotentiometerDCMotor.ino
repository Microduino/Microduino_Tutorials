int potPin = A0; //电位器输入引脚
int motorPin = 9;//电机引脚
int potValue = 0;//定义电位器的值
int motorValue = 0;//记录电机的值
void setup() {
 Serial.begin(9600);
}
void loop() {
 potValue = analogRead(potPin);  //读取电位器的值
 motorValue = map(potValue, 0, 1023, 0, 255);//映射为0~255之间的数值
 analogWrite(motorPin, motorValue);//PWM传给电机
 //显示
 Serial.print("potentiometer = " );     
 Serial.print(potValue);
 Serial.print("\t motor = ");
 Serial.println(motorValue);
 delay(2);    
}
