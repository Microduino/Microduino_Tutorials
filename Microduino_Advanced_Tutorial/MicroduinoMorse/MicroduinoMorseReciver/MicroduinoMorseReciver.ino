/* Receiving morse code - v0.1 */

int photocellPin = 0;     // LDR connected to pin 0
int photocellReading;     // the analog reading from LDR
long onmillis = 0;        // reset onmillis
long timeon = 0;          // reset timeon
int offset=30;
int dotTime=100;
int dashTime=300;
boolean isPrinted=false;



void setup(void) {
  Serial.begin(9600);
}
String morse;
void loop(void) {
  photocellReading = analogRead(photocellPin);  
  //Serial.println(photocellReading);
  //delay(1000);

  if(photocellReading<offset) {
    if(!isPrinted) {
      timeon = millis() - onmillis;

      if(timeon<=10) {

      } else if (timeon>=dotTime && timeon <= 130) {
        Serial.print(timeon);
        Serial.print(":  ");
        Serial.print(". ");
        morse+=".";
      } else if (timeon > dashTime && timeon <= 330) {
        Serial.print(timeon);
        Serial.print(":  ");
        Serial.print("_ ");
        morse+="_";
      } else { // larger than 300: space detected, which means the end of this letter
        Serial.print(timeon);
        Serial.print(":  ");
        Serial.println();
      }
      onmillis = 0;
      isPrinted=true;

      if(morse==".__..._._._..") {
        Serial.println(morse);
        morse="";
      }

    }
  } else {
    // on (receiving signal)
    if (onmillis == 0) {
      // we just started receiving signal
      onmillis = millis();
      isPrinted=false;
    }
  }
}

