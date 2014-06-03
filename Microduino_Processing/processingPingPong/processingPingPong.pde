/* Analog Pong
 Reads in 2 sensor values from Arduino for the paddle controls
 Original Code by Ben Leduc-Mills 
 Additional Code by Linz Craig
 */

import processing.serial.*; //import serial library

Serial myPort;
PFont myFont; //init font for text
int lf = 10; //linefeed in ASCII
Paddle leftPaddle, rightPaddle; //paddle objects
Ball pong; //ball object
int padWidth = 15;
int padHeight = 60;
int leftPaddlePos = 0;
int rightPaddlePos = 0;
int distWall = 15; //paddle distance from wall
float lpp; //scaled left paddle position
float rpp; //scaled right paddle position


int playerOne = 0; //player 1 score (left paddle)
int playerTwo = 0; //player 2 score (right paddle)
boolean oneWins = false;
boolean twoWins = false;

boolean firstContact = false; // Whether we've heard from the microcontroller

PImage pStart; //PImage object for Start Screen
PImage pEnd; //PImage object for End Screen

boolean started = false; //boolean to keep track of whether we have started game

int timePlayed = 0; //added to create timer

void setup() {
  pStart = loadImage("Pong_Start.png"); //Assigning the data or file
  pEnd = loadImage("Pong_End.png"); //Assigning the data or file

    size(1000, 600); //Size of Game Window
  rectMode(CENTER);
  ellipseMode(CENTER);
  myFont = createFont("FFScala", 16);
  textFont(myFont);
  println(Serial.list()); //list serial ports
  myPort = new Serial(this, Serial.list()[0], 9600); //init serial object (picks 1st port available)
  myPort.bufferUntil('\n');
  leftPaddle = new Paddle(padWidth, padHeight, distWall, leftPaddlePos); //init right paddle (width, height, x, y)
  rightPaddle = new Paddle(padWidth, padHeight, width - distWall, rightPaddlePos); //init left paddle
  pong = new Ball (15, width/2, height/2, 8, 2, 1, 1); //add new instance of Ball object called pong here
}

void draw() {
  background(50);
  showGUI(); //shows scores, etc. (see GUI tab)
  pong.display(); 
  pong.update();
  leftPaddle.display(lpp); //show left paddle
  rightPaddle.display(rpp); //show right paddle
  if (started == false) {
    image(pStart, 0, 0, width, height); //start
    println("Start");
  }
  timePlayed ++; //added to increment game timer
  pong.bRadius = pong.bRadius + (timePlayed * .001); //added to make ball go faster
}

void serialEvent(Serial myPort) {



  String myString = myPort.readStringUntil('\n');
  // if you got any bytes other than the linefeed:
  if (myString != null) {

    myString = trim(myString);

    // if you haven't heard from the microncontroller yet, listen:
    if (firstContact == false) {
      if (myString.equals("hello")) { 
        myPort.clear();          // clear the serial port buffer
        firstContact = true;     // you've had first contact from the microcontroller
        myPort.write('A');       // ask for more
        println("contact");
      }
    } 
    // if you have heard from the microcontroller, proceed:
    else {
      // split the string at the commas
      // and convert the sections into integers:
      int sensors[] = int(split(myString, ','));
      //add parsing sensor into array code here

      // print out the values you got:
      for (int sensorNum = 0; sensorNum < sensors.length; sensorNum++) {
        println("Sensor " + sensorNum + ": " + sensors[sensorNum]);
      }

      if (sensors.length > 1) {

        leftPaddlePos = sensors[0];

        rightPaddlePos = sensors[1]; //assign second sensor array value to rightPaddlePos here

        lpp = (float)leftPaddlePos;

        rpp = (float)rightPaddlePos; //reassign rightPaddlePos to the variable rpp of type float
        //println(rpp);

        //adjust these mapping functions to fit the sensors values you're receiving 
        lpp = map(lpp, 0, 1032, 1, height);
        rpp = map(rpp, 500, 900, 1, height);  //photocell
        //rpp = map(rpp, 200, 1024, 1, height); //soft pot
        //rpp =  map(rpp, 150, 275, 1, height); //flex sensor
      }
      // when you've parsed the data you have, ask for more:
      myPort.write("A");
    }
  }
}

void keyPressed() {
  if (started == true) {   //added for Start Screen
    pong.keyPressed();
  }                        //added for Start Screen
  if (started == false) {  //added for Start Screen
    started = true;        //added for Start Screen
  }                        //added for Start Screen
}

