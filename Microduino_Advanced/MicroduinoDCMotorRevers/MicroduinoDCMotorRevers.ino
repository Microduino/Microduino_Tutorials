int motorPin1 =  5;    // one wire connect to digital pin 5
int motorPin2 =  6;    // another connect to digital pin 6


void setup()   {                
  // inital, set to pins is OUTPUT
  pinMode(motorPin1, OUTPUT); 
  pinMode(motorPin2, OUTPUT);  
}


void loop()                     
{
  rotateLeft(150, 500);//Turn left rates 150,delay 500ms
  rotateRight(50, 1000);//Turn right rates 50，delay 1000ms
  rotateRight(150, 1000);//Turn right rates 150, delay 1000ms
  rotateRight(200, 1000);//Turn right rates 200, delay 1000ms
  rotateLeft(255, 500);//Turn left rates 255,delay 500ms
  rotateRight(10, 1500);//Turn right rates 10，delay 1500ms
}

//rotate left
void rotateLeft(int speedOfRotate, int length){
  analogWrite(motorPin1, speedOfRotate); //use pwm set current
  digitalWrite(motorPin2, LOW);    // set Pin motorPin2 is LOW
  delay(length); //waits
  digitalWrite(motorPin1, LOW);    // set Pin motorPin1 is LOW
}
//rotate right
void rotateRight(int speedOfRotate, int length){
  analogWrite(motorPin2, speedOfRotate); //use pwm set current
  digitalWrite(motorPin1, LOW);    // set Pin motorPin1 is LOW
  delay(length); //waits
  digitalWrite(motorPin2, LOW);    // set Pin motorPin2 is LOW
}

//full rotate left
void rotateLeftFull(int length){
  digitalWrite(motorPin1, HIGH); //set max current
  digitalWrite(motorPin2, LOW);    // set Pin motorPin2 is LOW
  delay(length); //waits
  digitalWrite(motorPin1, LOW);    // set Pin motorPin1 is LOW
}
//full rotate right
void rotateRightFull(int length){
  digitalWrite(motorPin2, HIGH); //set max current
  digitalWrite(motorPin1, LOW);    // set Pin motorPin1 is LOW
  delay(length); //waits
  digitalWrite(motorPin2, LOW);    // set Pin motorPin2 is LOW
}

