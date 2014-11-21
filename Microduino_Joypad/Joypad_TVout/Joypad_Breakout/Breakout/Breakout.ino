/*
  Breakout
 Copyright (C) 2011 Sebastian Goscik
 All rights reserved.
 
 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.
 */

#include <TVout.h>
#include <fontALL.h>
#include <Controllers.h>
#include <EEPROM.h>
#include "breakout_bitmaps.h"

#include <Joypad.h>
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library
#include <SPI.h>

#define cs   5
#define dc   4
#define rst  8  // you can also connect this to the Arduino reset

#if defined(__SAM3X8E__)
#undef __FlashStringHelper::F(string_literal)
#define F(string_literal) string_literal
#endif

Adafruit_ST7735 tft = Adafruit_ST7735(cs, dc, rst);

TVout TV;
const byte width = 120;   //Width of screen
const byte height = 96;   //Hight of screen
int dx = -1;              //Initial movement of ball
int dy = -1;              //Initial movement of ball
int xb;                   //Balls starting possition
int yb;                   //Balls starting possition
boolean released;         //If the ball has been released by the player
boolean paused = false;   //If the game has been paused
byte xPaddle;             //X position of paddle
boolean isHit[5][12];     //Array of if bricks are hit or not
boolean bounced=false;    //Used to fix double bounce glitch
byte lives = 3;           //Amount of lives
byte level = 1;           //Current level
unsigned int score=0;     //Score for the game
unsigned int brickCount;  //Amount of bricks hit
byte pad,pad2,pad3;       //Button press buffer used to stop pause repeating
byte oldpad,oldpad2,oldpad3;
char text[16];            //General string buffer
boolean start=false;      //If in menu or in game
boolean initialDraw=false;//If the inital draw has happened
char initials[3];         //Initials used in high score

//Ball Bounds used in collision detection
byte leftBall;
byte rightBall;
byte topBall;
byte bottomBall;

//Brick Bounds used in collision detection
byte leftBrick;
byte rightBrick;
byte topBrick;
byte bottomBrick;

void setup()
{

  TV.begin(NTSC,width,height);

  tft.initR(INITR_BLACKTAB); 
  tft.fillScreen(ST7735_BLACK);

  tft.setCursor(4, 0);
  tft.setTextSize(2);
  tft.println("Microduino");
  tft.setTextColor(ST7735_WHITE);
}

void movePaddle()
{
  //Move right
  if(xPaddle < width-12)
  {  
    if(Joypad.readJoystickX()<-100||Joypad.readJoystick1X()<-100)
    {  
      xPaddle+=1;
    }
  }

  //Move left
  if(xPaddle > 0)
  {
   if(Joypad.readJoystickX()>100||Joypad.readJoystick1X()>100)
    {  
      xPaddle-=1;
    }
  } 
}

void moveBall()
{
  if(released)
  {
    //Move ball
    xb=xb + dx;
    yb=yb + dy;
    delay(10);
    //Set bounds
    leftBall = xb;
    rightBall = xb+2;
    topBall = yb;
    bottomBall = yb+2;

    //Bounce off top edge
    if (yb<=0)
    {
      yb=2;
      dy=-dy;
      TV.tone(523, 20);
    }

    //Lose a life if bottom edge hit
    if (yb>=89)
    {
      TV.draw_rect(xPaddle,87,11,1,0);
      xPaddle = 54;
      yb=84; 
      released = false;
      lives--;
      drawLives();
      TV.tone(175, 20);
      if (random(0, 2) == 0) {
        dx = 1;
      } 
      else {
        dx = -1;
      }
    }

    //Bounce off left side
    if (xb<=0)
    {
      xb=2;
      dx=-dx;
      TV.tone(523, 20);
    }

    //Bounce off right side
    if (xb>=width-2)
    {
      xb=width-4;
      dx=-dx;
      TV.tone(523, 20);
    }
    //Bounce off paddle
    if (xb+1>=xPaddle && xb<=xPaddle+12 && yb+2>=87 && yb<=92){
      dy=-dy;
      dx=((xb-(xPaddle+6))/3); //Applies spin on the ball
      TV.tone(200, 20);
    }

    //Bounce off Bricks
    for (byte row = 0; row < 5; row++) {
      for (byte column = 0; column < 12; column++) {
        if (!isHit[row][column]){

          //Sets Brick bounds
          leftBrick = 10*column;
          rightBrick = 10*column+10;
          topBrick = 6*row+1;
          bottomBrick = 6*row+7;

          //If A collison has occured
          if (topBall <= bottomBrick && bottomBall>=topBrick && leftBall<=rightBrick && rightBall >=leftBrick){
            Score();
            brickCount++;
            isHit[row][column] = true;
            TV.draw_rect(10*column ,2+6*row,8,4,0);

            //Vertical collision
            if (bottomBall>bottomBrick || topBall<topBrick){
              //Only bounce once each ball move
              if(!bounced){
                dy=-dy;
                yb+=dy;
                bounced=true;
                TV.tone(261, 20);
              } 
            }

            //Hoizontal collision
            if (leftBall<leftBrick || rightBall>rightBrick){

              //Only bounce once brick each ball move
              if(!bounced){
                dx=-dx;
                xb+=dx;
                bounced=true;
                TV.tone(261, 20);
              } 
            }
          }
        }
      }
    }
    //Reset Bounce
    bounced=false;
  }
  else{
    //Ball follows paddle
    xb=xPaddle+5;

    //Release ball if FIRE pressed
    // pad3 = Controller.firePressed();
    pad3 = Joypad.readButton(CH_JOYSTICK1_SW);

    if(pad3 ==0 && oldpad3==0){  
      released=true;

      //Apply random direction to ball on release
      if (random(0, 2) == 0) {
        dx = 1;
      } 
      else {
        dx = -1;
      }
      //Makes sure the ball heads upwards 
      dy = -1;
    }
    oldpad3=pad3; 
  }
}

void drawBall()
{
  TV.set_pixel(xb,yb,0);
  TV.set_pixel(xb+1,yb,0);
  TV.set_pixel(xb,yb+1,0);
  TV.set_pixel(xb+1,yb+1,0);

  moveBall();

  TV.set_pixel(xb,yb,1);
  TV.set_pixel(xb+1,yb,1);
  TV.set_pixel(xb,yb+1,1);
  TV.set_pixel(xb+1,yb+1,1);
}

void drawPaddle()
{
  TV.draw_rect(xPaddle,87,11,1,0);
  movePaddle();
  TV.draw_rect(xPaddle,87,11,1,1);
}

void drawLives()
{
  sprintf(text, "LIVES:%u", lives);
  TV.print(0, 90, text);
}

void drawGameOver()
{
  TV.set_pixel(xb,yb,0);
  TV.set_pixel(xb+1,yb,0);
  TV.set_pixel(xb,yb+1,0);
  TV.set_pixel(xb+1,yb+1,0);
  TV.print(52, 42, "Game");
  TV.print(52, 54, "Over");
  delay(4000);
}

void pause()
{
  paused = true;
  //Draw pause to the screen
  TV.print(52, 45, "PAUSE");
  while(paused){
    delay(15);
    //Unpause if FIRE is pressed
    pad2 = Joypad.readButton(CH_JOYSTICK1_SW);
    if(pad2 ==0 && oldpad2==0 && released){
      TV.print(52, 45, "     ");
      paused=false;
    }
    oldpad2=pad2; 
  }
}

void Score()
{
  score += (level*10);
  sprintf(text, "SCORE:%u", score);
  TV.print(80, 90, text); 
}

void newLevel(){
  //Undraw paddle
  TV.draw_rect(xPaddle,87,11,1,0);

  //Undraw ball
  TV.set_pixel(xb,yb,0);
  TV.set_pixel(xb+1,yb,0);
  TV.set_pixel(xb,yb+1,0);
  TV.set_pixel(xb+1,yb+1,0);

  //Alter various variables to reset the game
  xPaddle = 54;
  yb=84;
  brickCount=0;
  released=false;

  //Draws new bricks and resets their values
  for (byte row = 0; row < 5; row++) {
    for (byte column = 0; column < 12; column++) {
      isHit[row][column] = false;
      TV.draw_rect(10*column ,2+6*row,8,4,1);
    }
  }

  //Draws the initial lives
  drawLives();

  //Draws the initial score
  sprintf(text, "SCORE:%u", score);
  TV.print(80, 90, text); 
}

//Used to delay images while reading button input
boolean pollFireButton(int n) {
  for(int i=0;i<n;i++) {
    TV.delay(15);
    pad = Joypad.readButton(CH_JOYSTICK_SW);
    if(pad ==0 && oldpad==0){
      oldpad3=1; //Forces pad loop 3 to run once
      return true;
    }
    oldpad=pad;
  }
  return false;
}

//Function by nootropic design to display highscores
boolean displayHighScores(byte file) {
  byte y = 10;
  byte x = 24;
  // Each block of EEPROM has 10 high scores, and each high score entry
  // is 5 bytes long:  3 bytes for initials and two bytes for score.
  int address = file*10*5;
  byte hi, lo;
  TV.fill(0);
  TV.select_font(font6x8);
  TV.print(32, 0, "HIGH SCORES");
  for(int i=0;i<10;i++) {
    sprintf(text, "%2d", i+1);
    TV.print(x, y+(i*8), text);

    hi = EEPROM.read(address + (5*i));
    lo = EEPROM.read(address + (5*i) + 1);
    if ((hi == 0xFF) && (lo == 0xFF)) {
      score = 0;
    } 
    else {
      score = (hi << 8) | lo;
    }
    initials[0] = (char)EEPROM.read(address + (5*i) + 2);
    initials[1] = (char)EEPROM.read(address + (5*i) + 3);
    initials[2] = (char)EEPROM.read(address + (5*i) + 4);

    if (score > 0) {
      sprintf(text, "%c%c%c %u", initials[0], initials[1], initials[2], score);
      TV.print(x+24, y+(i*8), text);
    }
  }
  if (pollFireButton(300)) {
    return true;
  }
  return false;
}

boolean titleScreen() {
  //Clears the screen
  TV.fill(0);

  //Draws the main title
  TV.bitmap(0, 0, title);
  if (pollFireButton(25)) {
    return true;
  }

  //Flash "Press FIRE" 5 times
  for(byte i=0;i<5;i++){
    //Draws "Press FIRE"
    TV.bitmap(31, 53, fire);
    if (pollFireButton(50)) {
      return true;
    }
    //Removes "Press FIRE"
    TV.draw_rect(31,53,57,8,0,0);
    if (pollFireButton(25)) {
      return true;
    }  
  }
  return false;
}

//Function by nootropic design to add high scores
void enterInitials() {
  char index = 0;

  TV.fill(0);
  TV.print(16, 0, "HIGH SCORE");
  sprintf(text, "%u", score);
  TV.print(88, 0, text);
  initials[0] = ' ';
  initials[1] = ' ';
  initials[2] = ' ';

  while (true) {
    TV.print_char(56, 20, initials[0]);
    TV.print_char(64, 20, initials[1]);
    TV.print_char(72, 20, initials[2]);
    for(byte i=0;i<3;i++) {
      TV.draw_line(56 + (i*8), 27, 56 + (i*8) + 6, 27, 1);
    }
    TV.draw_line(56, 28, 88, 28, 0);
    TV.draw_line(56 + (index*8), 28, 56 + (index*8) + 6, 28, 1);
    TV.delay(150);
    if (Joypad.readButton(CH_SWITCH_2)) {
      index--;
      if (index < 0) {
        index = 0;
      } 
      else {
        TV.tone(1046, 20);
      }
    }
    if (Joypad.readButton(CH_SWITCH_4)) {
      index++;
      if (index > 2) {
        index = 2;
      } 
      else {
        TV.tone(1046, 20);
      }
    }
    if (Joypad.readButton(CH_JOYSTICK1_SW)) {
      initials[index]++;
      TV.tone(523, 20);
      // A-Z 0-9 :-? !-/ ' '
      if (initials[index] == '0') {
        initials[index] = ' ';
      }
      if (initials[index] == '!') {
        initials[index] = 'A';
      }
      if (initials[index] == '[') {
        initials[index] = '0';
      }
      if (initials[index] == '@') {
        initials[index] = '!';
      }
    }
    if (Joypad.readButton(CH_JOYSTICK_SW)) {
      initials[index]--;
      TV.tone(523, 20);
      if (initials[index] == ' ') {
        initials[index] = '?';
      }
      if (initials[index] == '/') {
        initials[index] = 'Z';
      }
      if (initials[index] == 31) {
        initials[index] = '/';
      }
      if (initials[index] == '@') {
        initials[index] = ' ';
      }
    }
    if (!Joypad.readButton(CH_SWITCH_1)) {
      if (index < 2) {
        index++;
        TV.tone(1046, 20);
      } 
      else {
        TV.tone(1046, 20);
        return;
      }
    }
  }

}

void enterHighScore(byte file) {
  // Each block of EEPROM has 10 high scores, and each high score entry
  // is 5 bytes long:  3 bytes for initials and two bytes for score.
  int address = file*10*5;
  byte hi, lo;
  char tmpInitials[3];
  unsigned int tmpScore = 0;

  // High score processing
  for(byte i=0;i<10;i++) {
    hi = EEPROM.read(address + (5*i));
    lo = EEPROM.read(address + (5*i) + 1);
    if ((hi == 0xFF) && (lo == 0xFF)) {
      // The values are uninitialized, so treat this entry 
      // as a score of 0.
      tmpScore = 0;
    } 
    else {
      tmpScore = (hi << 8) | lo;
    }
    if (score > tmpScore) {
      enterInitials();
      for(byte j=i;j<10;j++) {
        hi = EEPROM.read(address + (5*j));
        lo = EEPROM.read(address + (5*j) + 1);
        if ((hi == 0xFF) && (lo == 0xFF)) {
          tmpScore = 0;
        } 
        else {
          tmpScore = (hi << 8) | lo;
        }
        tmpInitials[0] = (char)EEPROM.read(address + (5*j) + 2);
        tmpInitials[1] = (char)EEPROM.read(address + (5*j) + 3);
        tmpInitials[2] = (char)EEPROM.read(address + (5*j) + 4);

        // tmpScore and tmpInitials now hold what we want to write in the next slot.

        // write score and initials to current slot
        EEPROM.write(address + (5*j), ((score >> 8) & 0xFF));
        EEPROM.write(address + (5*j) + 1, (score & 0xFF));
        EEPROM.write(address + (5*j) + 2, initials[0]);
        EEPROM.write(address + (5*j) + 3, initials[1]);
        EEPROM.write(address + (5*j) + 4, initials[2]);

        score = tmpScore;
        initials[0] = tmpInitials[0];
        initials[1] = tmpInitials[1];
        initials[2] = tmpInitials[2];
      }
      score = 0;
      initials[0] = ' ';
      initials[1] = ' ';
      initials[2] = ' ';
      return;
    }
  }
}



void loop() 
{
  //Title screen loop switches from title screen and high scores utill FIRE is pressed
  while (!start) {
    start = titleScreen();
    if (!start) {
      start = displayHighScores(2);
    }
  }

  //Inital level draw
  if (!initialDraw){

    //Clears the screen 
    TV.fill(0);

    //Selects Font
    TV.select_font(font4x6);

    //Draws the new level
    newLevel();
    initialDraw=true;
  }

  if (lives>0){
    drawPaddle();

    //Pause game if FIRE pressed
    pad = Joypad.readButton(CH_SWITCH_2);
    if(pad ==0 && oldpad==0 && released){
      oldpad2=1; //Forces pad loop 2 to run once
      pause();
    }
    oldpad=pad;

    drawBall();

    if(brickCount==60){
      level++;
      newLevel(); 
    }
  }
  else{
    drawGameOver();
    if (score > 0) {
      TV.select_font(font6x8);
      enterHighScore(2);
    }
    TV.select_font(font4x6);
    drawGameOver();
    initialDraw=false;
    start=false;
    lives=3;
    score=0;
    newLevel();
  }
  TV.delay_frame(1);
}


