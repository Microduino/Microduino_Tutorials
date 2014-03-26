int motorPin1 =  5;    // 一条线连接 digital pin 5
int motorPin2 =  6;    // 另一条线连接 digital pin 6


void setup()   {                
  // 初始化，设置两个引脚为输出
  pinMode(motorPin1, OUTPUT); 
  pinMode(motorPin2, OUTPUT);  
}


void loop()                     
{
  rotateLeft(150, 500);//左转速率150,持续500ms
  rotateRight(50, 1000);//右转速率50，持续1000ms
  rotateRight(150, 1000);//右转速率150,持续1000ms
  rotateRight(200, 1000);//右转速率200,持续1000ms
  rotateLeft(255, 500);//左转速率255,持续500ms
  rotateRight(10, 1500);//右转速率10，持续1500ms
}

//向左转
void rotateLeft(int speedOfRotate, int length){
  analogWrite(motorPin1, speedOfRotate); //用pwm设置电流大小
  digitalWrite(motorPin2, LOW);    // 设置 Pin motorPin2为LOW
  delay(length); //waits
  digitalWrite(motorPin1, LOW);    // 设置 Pin motorPin1为LOW
}
//向右转
void rotateRight(int speedOfRotate, int length){
  analogWrite(motorPin2, speedOfRotate); //用pwm设置电流大小
  digitalWrite(motorPin1, LOW);    // 设置 Pin motorPin1为LOW
  delay(length); //waits
  digitalWrite(motorPin2, LOW);    // 设置 Pin motorPin2为LOW
}

//全力向左转
void rotateLeftFull(int length){
  digitalWrite(motorPin1, HIGH); //设置最大电流
  digitalWrite(motorPin2, LOW);    // 设置 Pin motorPin2为LOW
  delay(length); //waits
  digitalWrite(motorPin1, LOW);    // 设置 Pin motorPin1为LOW
}
//全力向右转
void rotateRightFull(int length){
  digitalWrite(motorPin2, HIGH); //设置最大电流
  digitalWrite(motorPin1, LOW);    // 设置 Pin motorPin1为LOW
  delay(length); //waits
  digitalWrite(motorPin2, LOW);    // 设置 Pin motorPin2为LOW
}

