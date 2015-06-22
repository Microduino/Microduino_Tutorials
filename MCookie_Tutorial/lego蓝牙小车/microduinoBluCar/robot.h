Servo myservo;

//full forward
void fullForward() {
  digitalWrite(motor_pinB, HIGH);
  digitalWrite(motor_pinA, LOW);
  //delay(2000);
}

//stop
void stop() {
  digitalWrite(motor_pinB, LOW);
  digitalWrite(motor_pinA, LOW);
  //delay(2000);
}

//full backward
void fullBackward() {
  digitalWrite(motor_pinB, LOW);
  digitalWrite(motor_pinA, HIGH);
  //delay(2000);
}

void speedControlForward(int speed) {
  digitalWrite(motor_pinB, HIGH);
  digitalWrite(motor_pinA, HIGH);
  analogWrite(motor_pinB, speed);
}

void speedControlBackward(int speed) {
  digitalWrite(motor_pinB, HIGH);
  digitalWrite(motor_pinA, HIGH);
  analogWrite(motor_pinA, speed);
}




void vorobot()
{
    //print currentInfo
  if(currentInfo!=NULL) {

    currentInfo.toCharArray(senderData, sizeof(senderData)); 
    sscanf((char *)strstr((char *)senderData, "D:"), "D:%d,%d", &angle,&speed);

    if(angle>180) {
      return;
    }

    Serial.print("Angle:");
    Serial.print(angle);
    Serial.print("  Speed:");
    Serial.println(speed);

    if(angle!=90) {
      myservo.write(angle);
    }
    
    if(speed>0) {
      //speedControlForward(abs(speed));
      fullForward();
    } else if(speed==0){
      stop();
    } else {
      //speedControlBackward(abs(speed));
      fullBackward();
    }
  }
}