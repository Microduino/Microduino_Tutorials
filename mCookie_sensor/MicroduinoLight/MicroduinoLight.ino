
#define colorSensorPin A2

#define buzzer_pin 6
//#define colorSensorPin A3

int sensorIn;
int value;

void setup() {
  Serial.begin(9600);

   pinMode(colorSensorPin, INPUT);
   pinMode(buzzer_pin, OUTPUT);
}

// the loop routine runs over and over again forever:
void loop() {
  // read the input pin:
  sensorIn = analogRead(colorSensorPin);

  // print out the state of the button:
  Serial.print("sensor value: ");
  Serial.println(sensorIn);

  delay(500);


  value = map(sensorIn, 0, 1023, 0, 255);

  tone(buzzer_pin,value);

}

