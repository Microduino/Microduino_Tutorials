    #include <Servo.h>
    Servo myservo;
    int pos = 90; // initial position
    int sens1 = A0; // LRD 1 pin
    int sens2 = A1; //LDR 2 pin
    int tolerance = 2;
    void setup()
    {
    // initialize serial communication at 9600 bits per second:
    //Serial.begin(9600);
    myservo.attach(9); // attaches the servo on pin 9 to the servo object
    pinMode(sens1, INPUT);
    pinMode(sens2, INPUT);
    myservo.write(pos);
    delay(200); // a 2 seconds delay while we position the solar panel
    }
    void loop()
    {
    int val1 = analogRead(sens1); // read the value of sensor 1
    int val2 = analogRead(sens2); // read the value of sensor 2
    //Serial.print(val1);
    //Serial.print("\t");
    //Serial.println(val2);
    if((abs(val1 - val2) <= tolerance) || (abs(val2 - val1) <= tolerance)) {
    //do nothing if the difference between values is within the tolerance limit
    } else {
    if(val2 > val1)
    {
    pos = --pos;
    }
    if(val2 < val1)
    {
    pos = ++pos;
    }
    }
     
    if(pos > 180) { pos = 180; } // reset to 180 if it goes higher
    if(pos < 0) { pos = 0; } // reset to 0 if it goes lower
    myservo.write(pos); // write the position to servo
    delay(10);
    }
