#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h>
#include <SPI.h>
#include <EEPROM.h>
#include <I2Cdev.h>
#include <MPU6050.h>
#include <Wire.h>//添加必须的库文件
#include <Joypad.h>  // Try to use hardware but if not (buttons) use Joypad library function

//====一下三个定义了陀螺仪的偏差===========
#define Gx_offset -3.06 
#define Gy_offset 1.01
#define Gz_offset -0.88

//====================
MPU6050 accelgyro;

int16_t ax,ay,az;
int16_t gx,gy,gz;//存储原始数据
float Ax,Ay,Az;//单位 g(9.8m/s^2)

float Angel_accX,Angel_accY,Angel_accZ;//存储加速度计算出的角度

//SCREEN
#define s_width 128
#define s_height 149

#define cs 5  // Joypad uses display chip select on D7
#define dc 4  // Joypad uses LCD DC on D0
#define rst 1 // Joypad uses display reset on D1

Adafruit_ST7735 tft = Adafruit_ST7735(cs, dc, rst);  // define tft display (use Adafruit library) hw (if you add osi and sclk it will slow too much)

long lastMillis;
boolean monitor = false;

unsigned int background = 0x0000;   // ST7735_BLACK

//SOUND
#define sound_pin 6 // Direct sound on Joypad

//BUTTONS (not used on Joypad as buttons must be read using library function)
// #define Rbtn_pin 1
// #define Lbtn_pin 0

//PLAYER
float p_X = s_width/2;
float p_Y = s_height/2;
float p_lastX, p_vX;
float p_lastY, p_vY;
//#define p_vmaxY 6
//#define p_vmaxX 6
#define gravity_default 0.2
float gravity = 0.2;
#define p_bounceSpeed_default 4
byte p_bounceSpeed = p_bounceSpeed_default;
#define p_width_default 4
byte p_width = p_width_default;
#define p_height_default 4
byte p_height = p_height_default;
#define p_color_default 0xFFFF     // ST7735_WHITE
#define p_color_dead w_color
int p_color = p_color_default;

//SCORE & BONUS
unsigned int score = 0;
unsigned int highscore = 0;
byte nextBonus;
boolean b_pendingPlatform = false;
boolean b_used;
byte b_platformID;
byte b_pendingID;
byte b_onUseID;
byte b_remaining = 0;
byte b_max;
#define b_number 4
int b_colors[b_number] = {
  0xFD00, 0x0F00, 0xF000, 0x00FF};
byte b_lives = 0;


//WORLD
#define w_size 8
byte world[w_size][6]; //contain platforms data
int w_color = ST7735_YELLOW;  // was 0x41E6 which was faint on my display
byte w_speed = 1;
byte w_widthMin = 4;
byte w_widthMax = 8;

/*
world[a][b] ->
 a ->  platform id
 b ->  0 = x
 1 = y
 2 = last x
 3 = last y
 4 = width
 */

//ACCELEROMETER (mod for Joypad)
int acc_avgX, acc_avgY, acc_avgZ;
#define acc_pinX 1  // A5
#define acc_pinY 2  // A7
#define acc_pinZ 3  // A6

int val;

void setup() {
  Wire.begin();

  accelgyro.initialize();

  // initialize a ST7735R TFT
  tft.initR();
  tft.fillScreen(background);
  tft.setCursor(4, 40);
  tft.setTextSize(2);
  tft.println("Microduino");
  tft.println("  Joypad  ");
  delay(1000);
  tft.setTextSize(1);
  tft.fillScreen(background);
  randomSeed(127);  // can't use unused analog pin as there are none
  //Serial.begin(9600);

  FillWorld();
  ScoreSetup();//最高分
}

void loop() {
  if(millis()-lastMillis > 20){
    lastMillis = millis();
    CheckButtons();
    MovePlayer();
    ScrollWorld();
    CollideBorders();
    CollideWorld();
    DrawWorld();
    DrawPlayer();
    if(monitor){
      ShowMonitor();
    }
  }
}

void ShowMonitor(){
  unsigned long time = millis() - lastMillis; //should be inferior to 20, the time between frames
  tft.fillRect(0, s_height+1, 100, 1, 0x0A00);
  tft.fillRect(100, s_height+1, 28, 1, 0xA000);
  tft.fillRect(time*5, s_height+1, 3, 1, 0xFFFF);
}

int acc_readX() {
  if(Joypad.readButton(CH_SWITCH_L)&&Joypad.readButton(CH_SWITCH_R))
  {
    mpu6050();
    val= constrain(Angel_accY, -64, 64); // use Y for orientation
  }
  else
    val= -1 * constrain(Joypad.readJoystickX()/8, -64, 64); // use Y for orientation
  return(val);
  // Serial.println(val);
}

void CheckBonus(byte thisPlatform){
  if(b_pendingPlatform && (b_platformID == thisPlatform)){
    b_pendingPlatform = false;
  }
  if(nextBonus ){
    nextBonus--;
  } 
  else {
    nextBonus = random(30, 60);
    b_pendingID = random(0, b_number);
    b_pendingPlatform = true;
    b_platformID = thisPlatform;
  }
}

void BonusReset(){
  nextBonus = random(30, 60);
  b_pendingPlatform = false;
  b_remaining = 0;
  tft.fillRect(0, s_height+2, 96, 10, background);
  tft.fillRect(0, s_height+1, 96, 1, w_color);
  EndUseBonus();
}

void ClearBonus(){
  EndUseBonus();
  b_remaining = 0;
  p_color = p_color_default;
  tft.fillRect(0, s_height+2, 96, 10, background);
  tft.fillRect(0, s_height+1, 96, 1, w_color);
  b_used = false;
}

void GetBonus(){
  ClearBonus();
  b_onUseID = b_pendingID;
  b_pendingPlatform = false;
  tone(sound_pin, 3500, 50);
  p_color = b_colors[b_onUseID];
  tft.fillRect(0, s_height+2, 96, 10, background);

  switch (b_onUseID){
  case 0:
    drawString(1, s_height+3, "Jet pack", b_colors[b_onUseID], 1);
    //   tft.drawString(1, s_height+3, "Jet pack", b_colors[b_onUseID], 1);
    b_max = 100;
    b_remaining = b_max;
    break;
  case 1:
    drawString(1, s_height+3, "Frog legs", b_colors[b_onUseID], 1);
    //    tft.drawString(1, s_height+3, "Frog legs", b_colors[b_onUseID], 1);
    b_max = 100;
    b_remaining = b_max;
    break;
  case 2:
    drawString(1, s_height+3, "Testosterone", b_colors[b_onUseID], 1);
    //    tft.drawString(1, s_height+3, "Testosterone", b_colors[b_onUseID], 1);
    b_max = 1;
    b_remaining = b_max;
    break;
  case 3:
    drawString(1, s_height+3, "Low gravity", b_colors[b_onUseID], 1);
    //    tft.drawString(1, s_height+3, "Low gravity", b_colors[b_onUseID], 1);
    b_max = 255;
    b_remaining = b_max;
    break;
  }

  if(b_remaining > 1) {
    tft.fillRect(95, s_height+2, 1, 10, b_colors[b_onUseID]);
    tft.fillRect(0, s_height+1, 95, 1, b_colors[b_onUseID]);
  }

}

void UseBonus(){
  if(b_remaining){
    b_used = true;
    tft.fillRect(b_remaining*95/b_max, s_height+2, 1, 10, background);
    tft.drawPixel(b_remaining*95/b_max, s_height+1, w_color);
    b_remaining --;
    tft.fillRect(b_remaining*95/b_max, s_height+2, 1, 10, b_colors[b_onUseID]);
    tft.drawPixel(b_remaining*95/b_max, s_height+1, b_colors[b_onUseID]);

    switch (b_onUseID){
    case 0:
      if(p_vY > -1){
        p_vY -= 0.3;
      }
      tone(sound_pin, 100, 3 + abs(10 * p_vY));
      break;
    case 1:
      p_bounceSpeed = p_bounceSpeed_default * 1.5;
      break;
    case 2:
      p_width = min(p_width+1, p_width_default+12);
      p_height = min(p_height+1, p_height_default+12);
      b_lives = min(b_lives+1, 12);
      tone(sound_pin, 50+150*b_lives, 100);
      break;
    case 3:
      gravity = gravity_default * 0.3;
      break;
    }

    if(!b_remaining){
      ClearBonus();
    }
  }
}

void EndUseBonus(){
  b_used = false;

  switch (b_onUseID){
  case 0:
    break;
  case 1:
    p_bounceSpeed = p_bounceSpeed_default;
    break;
  case 2:
    break;
  case 3:
    gravity = gravity_default;
    break;
  }
}

void CheckButtons(){
  int Lbutton = !Joypad.readButton(CH_SWITCH_1);  // Joypad Switch 1
  int Rbutton = !Joypad.readButton(CH_SWITCH_2);    // Joypad Switch 3

  if(Lbutton) {
    if(monitor)
      monitor=false;
    else
      monitor = true;
  }
  if(Rbutton) {
    UseBonus();
  } 
  else {
    if(b_used = true) {
      EndUseBonus();
    }
  }
}

void DrawPlayer(){
  tft.fillRect(p_lastX, p_lastY, p_width, p_height, background); //erase previous pos
  if(p_lastX > (s_width-1 - p_width)) //if accross the edge of the screen
    tft.fillRect(0, p_lastY, p_lastX + p_width - s_width, p_height, background);

  tft.fillRect(p_X, p_Y, p_width, p_height, p_color); //draw new pos
  if(p_X > (s_width-1 - p_width)) //if accross the edge of the screen
    tft.fillRect(0, p_Y, p_X + p_width - s_width, p_height, p_color);

}

void MovePlayer(){
  p_lastX = p_X;
  p_lastY = p_Y;

  p_vX = (p_vX + acc_readX()) / 20;
  p_vY = (p_vY + gravity);

  p_X = p_X + p_vX;
  if(p_X > s_width){
    p_X = 0;
  }
  if(p_X < 0){
    p_X = s_width - p_X;
  }
  p_Y = p_Y + p_vY;

  if((p_vY < -4) && (p_color != p_color_dead)){
    tone(sound_pin, 600+p_vY*100, 25);
  }
  if(b_used && (b_onUseID == 3)) { //if low gravity is on use
    tone(sound_pin, 300-abs(p_vY*100), 25);
  }
}

void CollideBorders() {
  if(p_Y > s_height - p_height){ //touch the floor
    p_vY = -p_bounceSpeed;
    p_Y = s_height - p_height;
    if(!b_lives){
      if(p_color != p_color_dead){
        BonusReset();
        ScoreReset();
      }
    } 
    else {
      tft.fillRect(p_lastX, p_lastY, p_width, p_height, background);
      p_width--;
      p_height--;
      b_lives--;
      tone(sound_pin, 100+150*b_lives, 100);
    }
  }
}

void ScoreSetup(){
  //tft.fillRect(0, s_height+2, 128, 10, background);
  tft.fillRect(0, s_height+1, 128, 1, w_color);
  highscore = EEPROM.read(0);
  highscore += (EEPROM.read(1)<<8);
  if(highscore > 64000){
    highscore = 0;
  }
  drawString(1, s_height+3, "Highscore:", w_color, 1);
  drawInt(highscore, 66, s_height+3, w_color, background);
}

void ScoreReset(){

  if(p_color != p_color_dead) {
    p_color = p_color_dead;
    tone(sound_pin, 50, 500);

    if(score > highscore){
      if(score < 64000) {
        EEPROM.write(0, score);
        EEPROM.write(1, score>>8);
      }
      else { // bad value, zero out
        EEPROM.write(0,0);
        EEPROM.write(1,0);
      }        
      //drawString(1, s_height+3, "HIGHSCORE!", p_color_default, 1);
      // tft.drawString(1, s_height+3, "HIGHSCORE!", p_color_default, 1);
      drawInt(score, 120, s_height+3, p_color_default, background);
      highscore = score;
    } 
    gameover();
    score = 0;
    drawInt(score, 97, s_height+3, w_color, background);
  }

}

void ScoreAdd(){ // add to the current score lower left
  score++;
  tone(sound_pin, 3000, 50);
  if(score > highscore) {
    drawInt(score, 97, s_height+3, p_color_default, background);
  } 
  else {
    drawInt(score, 97, s_height+3, w_color, background);
  }
  w_widthMin = max(1, 4 - score/50);
  w_widthMax = max(2, 8 - score/50);
  if(score == 1) {
    p_color = p_color_default;
  }
}

//transform from int to string and display it
void drawInt(unsigned int num, byte nx, byte ny, unsigned int color, unsigned int color2) {
  tft.fillRect(nx, ny, 29, 7, color2);
  drawChar(nx+24, ny, 48+(num%10), color);
  //    tft.drawChar(nx+24, ny, 48+(num%10), color);
  if(num>9) {
    drawChar(nx+18, ny, 48+(num%100)/10, color);
    ///     tft.drawChar(nx+18, ny, 48+(num%100)/10, color);
    if(num>99) {
      drawChar(nx+12, ny, 48+(num%1000)/100, color);
      //      tft.drawChar(nx+12, ny, 48+(num%1000)/100, color);
      if(num>999) {
        drawChar(nx+6, ny, 48+(num%10000)/1000, color);
        //        tft.drawChar(nx+6, ny, 48+(num%10000)/1000, color);
        if(num>9999) {
          drawChar(nx, ny, 48+(num%100000)/10000, color);
          //          tft.drawChar(nx, ny, 48+(num%100000)/10000, color);
        }
      }
    }
  }
}

/*
world[a][b] ->
 a ->  platform id
 b ->  0 = x
 1 = y
 2 = last x
 3 = last y
 4 = width
 4 = last width
 */

void FillWorld(){
  for(byte i=0; i<w_size; i++) {
    world[i][0] = (s_width * (w_size-i-1) / w_size)/2;
    world[i][1] = 1 + i * (s_height / w_size);
    world[i][2] = world[i][0];
    world[i][3] = world[i][1];
    world[i][4] = s_width * (i+1)/w_size;
    world[i][5] = world[i][4];
  }
}

void ScrollWorld(){
  byte fastScrolling = 0;
  if(p_Y < s_height/4) {//if the player reachs the top of the screen
    fastScrolling ++;
    if(p_Y < s_height/8) {//if the player reachs the top of the screen
      fastScrolling ++;
    }
  }
  p_Y += fastScrolling;
  for(byte i=0; i<w_size; i++) { //for each platform
    world[i][3] = world[i][1]; //update last y
    world[i][2] = world[i][0]; //update last x
    world[i][5] = world[i][4]; //update last width
    world[i][1] += w_speed ; //move the platform downward
    world[i][1] += fastScrolling;
    if(world[i][1] > s_height){ //if the platform reaches the bottom of the screen
      //world[i][2] = world[i][0]; //update last x
      world[i][1] = world[i][1] - s_height + 1; //put the platform on the top of the screen
      world[i][4] = min(random(w_widthMin, w_widthMax)*p_width_default, s_width-world[i][0]); //pick a random width
      world[i][0] = random(0, s_width-world[i][4]); //pick a random x
      CheckBonus(i); //check if the new platform will have a bonus
    }
  }
}

void CollideWorld() {
  for(byte i=0; i<w_size; i++) {
    if(p_vY > 0){ //if player falling
      if((p_Y+p_width <= world[i][1]) && (world[i][1] < (p_Y+p_vY+p_height)) ) { //and at the height of the platform
        if(((p_X+p_width) >= world[i][0]) && (world[i][0]+world[i][4] > (p_X)) ) {     //and does not miss it
          p_vY = -p_bounceSpeed ;
          p_Y = world[i][1] - p_height;
          ScoreAdd();
          if(b_pendingPlatform && (b_platformID == i)) {
            GetBonus();
          }
        }
      }
    }
  }
}

void DrawWorld() {
  for(byte i=0; i<w_size; i++) {
    tft.fillRect(world[i][2], world[i][3], world[i][5], 1, background);
    tft.fillRect(world[i][0], world[i][1], world[i][4], 1, w_color);
  }
  if(b_pendingPlatform){
    tft.fillRect(world[b_platformID][0], world[b_platformID][1], world[b_platformID][4], 1, b_colors[b_pendingID]);
  }
}


void drawString(byte x, byte y, char *text, uint16_t color, bool wrap) { // replicate tft.drawString
  tft.setCursor(x,y);
  tft.setTextColor(color);
  tft.setTextWrap(wrap);
  tft.print(text);
}

void drawChar(byte x, byte y, char text, uint16_t color) { // replicate tft.drawChar
  tft.setCursor(x,y);
  tft.setTextColor(color);
  tft.print(text);
}

void mpu6050()
{
  accelgyro.getMotion6(&ax,&ay,&az,&gx,&gy,&gz);//获取三个轴的加速度和角速度

  //======一下三行是对加速度进行量化，得出单位为g的加速度值
  Ax=ax/16384.00;
  Ay=ay/16384.00;
  Az=az/16384.00;
  //==========以下三行是用加速度计算三个轴和水平面坐标系之间的夹角
  // Angel_accX=atan(Ax/sqrt(Az*Az+Ay*Ay))*180/3.14;
  Angel_accY=atan(Ay/sqrt(Ax*Ax+Az*Az))*180/3.14;
  //Angel_accZ=atan(Az/sqrt(Ax*Ax+Ay*Ay))*180/3.14;
}

void gameover()
{
  tft.fillScreen(background);
    tft.setTextColor(ST7735_WHITE);
  tft.setCursor(4, 4);
  tft.setTextSize(2);
  tft.println("Microduino");
  tft.setTextColor(ST7735_BLUE);
  tft.setCursor(4, 40);
  tft.setTextSize(2);
  tft.println("GAME OVER");
  tft.println("");
  tft.setTextSize(1);
  tft.print(" High score:");
  tft.setTextSize(2);
  tft.setTextColor(ST7735_WHITE);
  tft.println(highscore);
  tft.println(" ");
  tft.setTextSize(1);
  tft.print(" Your score:");
  tft.setTextSize(2);
  tft.setTextColor(ST7735_GREEN);
  tft.println(score);
  delay(2000);
  tft.fillScreen(background);
  tft.setTextSize(1);
  FillWorld();
  //tft.fillRect(0, s_height+2, 128, 10, background);
  tft.fillRect(0, s_height+1, 128, 1, w_color);
  tft.setCursor(1, s_height+3);
  tft.setTextColor(ST7735_RED);
  tft.print("Microduino Joypad");
}
















