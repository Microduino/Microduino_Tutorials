
#define humanHotSensor 10

const int buzzerPin =  5;      // the number of the buzzer pin

bool humanHotState=false;

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  // make the humanHotSensor's pin an input:
  pinMode(humanHotSensor, INPUT);
  
  pinMode(buzzerPin, OUTPUT);
}

// the loop routine runs over and over again forever:
void loop() {
  // read the input pin:
  humanHotState = digitalRead(humanHotSensor);
  // print out the state of the button:
  Serial.println(humanHotState);
  delay(1);        // delay in between reads for stability

  if(humanHotState) {
    tone(buzzerPin,255);
  }else {
    tone(buzzerPin,0);
  }

}

