/*
 AnalogPong
 Reads in 2 sensor value as paddles to send to a processing sketch
 Code by Ben Leduc-Mills, edited by Linz Craig
 */

void setup() {
  Serial.begin(9600); //Establish rate of Serial communication
  establishContact(); //See function below
}

void loop() {
  if (Serial.available() > 0) { //If we've heard from Processing do the following

    int inByte = Serial.read();    // read the incoming byte from processing
    int leftPaddle = analogRead(A0); //save the value from analog sensor as variable leftPaddle
    Serial.print(leftPaddle, DEC); //print out on Serial the value of the first sensor
    Serial.print(",");             //print out on Serial a delimiter
    int rightPaddle = analogRead(A1); //save the value from analog sensor as variable leftPaddle
    Serial.println(rightPaddle, DEC); //print out on Serial the value of the first sensor
    //print out on Serial the value of the second sensor (but make sure you use Serial.println instead of Serial.print)
    //If you add additional sensors/buttons make sure you add delimiters between the values and use Serial.println only for the last value
  }
}

void establishContact() {
  while (Serial.available() <= 0) { //when Arduino receives a Serial message from Processing
    Serial.println("hello");   // send a starting message
    delay(300); //Wait 300 milliseconds
  }
}


