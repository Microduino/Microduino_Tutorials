#include <Adafruit_ST7735.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Joypad.h>  // use Joypad library function

#define cs    5  // Joypad uses display chip select on D7
#define dc    4  // Joypad uses LCD DC on D0
#define rst   1 // Joypad uses display reset on D1

Adafruit_ST7735 tft = Adafruit_ST7735(cs, dc, rst);  // define tft display (use Adafruit library) hw (if you add osi and sclk it will slow too much)

float BPX;  // Ball Position X
float BPY;  // Ball Position Y
int byx;
int bx;
int A;      // Ball Direction (1 = left to right, -1 = right to left)
int by=1;
int playerScore;    // These variables hold the player & computer score.
int computerScore;
#define WINSCORE 5  // When you reach this value you win - suggest you keep <=9 for display alignment, increase > 9 if you tweek

int R, L, D, U;     // These values adjust the paddles L & R from player buttons, U & D from computer algorithm
int playerPaddle;   // Player Paddle Position
int computerPaddle; // Computer's Paddle Position
int Random;

void setup(){

  tft.initR(INITR_REDTAB);       // Initialize Adafruit display - use GREENTAB if you have such display
  tft.fillScreen(ST7735_BLACK);  // clear display
  tft.setRotation(1);            // landscape (as on Joypad)

  // Draw splash screen text
  tft.setTextColor(ST7735_YELLOW, ST7735_BLACK);
  tft.setTextSize(5);
  tft.setCursor(22, 15);
  tft.print("PONG");
  tft.setTextSize(1);
  tft.println("");
  tft.println(" Microduino Joypad");
  tft.println("");
  tft.println(" Joypad port by:");
  tft.println("           PKJ");
  tft.setTextColor(ST7735_WHITE, ST7735_BLACK);
  tft.println(" ");
  tft.println("Open the top switch To Start");
  while(Joypad.readButton(SWITCH_R)==LOW||Joypad.readButton(SWITCH_L)==LOW) 
    ;                                    // New game when swtich 4 is pressed

  tft.fillScreen(ST7735_BLACK);  // clear screen again
  tft.setRotation(0);            // coordinates for game actually use portrait mode
  DrawCourt(0);   // Draw court lines

  playerScore=0;
  computerScore=0;
  DisplayScore(playerScore, computerScore);
  BPX = 15;
  BPY = 15; 
  byx=15;
  bx=1;
  A=1;  
  playerPaddle=48;    // set paddles in the center of the display
  computerPaddle=48;

  //long seed = Joypad.readLightSensor() * Joypad.readMicrophone();
  // randomSeed(seed); // create seed because on Joypad there are no unused analog pins for random noise
}

void loop() {
  /*
  if ((BPY==80)||(BPY==20)){    //This is how i made the computer/the probability of the computer making an error
   Random = random(1, 10);
   }
   if (Random<=8){
   
   if (( A == 1)||((BPY > 100) && ( A == -1))) {
   if ((bx == -1) && (BPX < (computerPaddle+16))) {
   U = 1; 
   D = 0;
   }    //Computer simulation
   if ((bx == 1) && (BPX > (computerPaddle+16))) {
   D = 1; 
   U = 0;
   }
   }
   else {
   D = 0; 
   U = 0;
   }
   }
   
   if ((Random>8) && (Random<=9)){ 
   if (( A == 1)||((BPY > 100) && ( A == -1))) {
   if ((bx == -1) && (BPX < (computerPaddle+16))) {
   U = 0; 
   D = 1;
   }    //Computer simulation
   if ((bx == 1) && (BPX > (computerPaddle+16))) {
   D = 0; 
   U = 1;
   }
   }
   else {
   D = 0; 
   U = 0;
   }
   }
   
   if (Random>9){
   
   if (( A == 1)||((BPY > 100) && ( A == -1))) {
   if ((bx == -1) && (BPX < (computerPaddle-16))) {
   U = 1; 
   D = 0;
   }    //Computer simulation
   if ((bx == 1) && (BPX > (computerPaddle-16))) {
   D = 1; 
   U = 0;
   }
   }
   else {
   D = 0; 
   U = 0;
   } 
   }
   */
  DrawCourt(0);  // Draw court line(s)

  // see if player is using up & down buttons to signal they wish t move the player paddle
  R = Joypad.readButton(SWITCH_1);  // Esplora Switch 1 moves player paddle down
  L = Joypad.readButton(SWITCH_2);    // Esplora Switch 3 moves player paddle up
  //if(Joypad.readJoystickX()<-100) R=1; // joystick not working
  // if(Joypad.readJoystickX()>100) L=1; // joystick not working well

  D = Joypad.readButton(SWITCH_3);
  U = Joypad.readButton(SWITCH_4);

  playerPaddle=playerPaddle+R; //These equations are for the movement of the paddle, R, L, D, and U are all boolean.  paddles initially equal 48. This is so
  //at startup the paddles are at center.  
  playerPaddle=playerPaddle-L;

  computerPaddle=computerPaddle+D;  //I used D and U because i use the buttons for other applications but they can be defined as player2 R and L
  computerPaddle=computerPaddle-U;

  tft.fillRect(playerPaddle-1, 3, 2, 3, ST7735_BLACK);  //These two parts are so the paddle erase themselves as they move, they could have been black pixels but i just used 
  tft.fillRect(playerPaddle+33, 3, 2, 3, ST7735_BLACK);
  tft.fillRect(playerPaddle, 3, 32, 3, ST7735_GREEN);   //This is the paddle for one player

  if (playerPaddle==1)  // This is so the paddle does not move off the screen on the bottom side of the screen
    playerPaddle=2;
  if (playerPaddle==95) // This is so the paddle does not move off the screen on the top side of the screen
    playerPaddle=94;

  tft.fillRect(computerPaddle, 154, 32, 3, ST7735_GREEN);
  tft.fillRect(computerPaddle-1, 154, 2, 3, ST7735_BLACK);
  tft.fillRect(computerPaddle+33, 154, 2, 3, ST7735_BLACK);

  if (computerPaddle==1)
    computerPaddle=2;
  if (computerPaddle==95)
    (computerPaddle=94);

  byx=(A+byx);
  BPY=((byx));
  BPX=(bx+BPX);

  if ((BPX== 127)||(BPX==2)){
    (bx=(-1*bx));
  }
  else {
  };
  if ((BPX<=(computerPaddle+38)) && (BPX>=(computerPaddle-6))&&(BPY==149)){
    (A=(-1*A));
  }
  else{
  };
  if ((BPX<=(playerPaddle+38)&&(BPX>=(playerPaddle-6))&&(BPY==11))){
    (A=(-1*A));
  }
  else{
  };

  if (BPY>=160 || BPY <=0){    // someone scored!
    if (BPY >= 160) 
      playerScore=playerScore+1; 
    else 
      computerScore=computerScore+1;
    // Reset:                                   // reset court after score
    DisplayScore(playerScore, computerScore);
    DrawCourt(0);
    tft.fillCircle(BPX, BPY, 7, ST7735_BLACK);  // erase ball in last known location
    BPX=64;                                     // set ball to center of screen
    BPY=80;
    tft.fillCircle(BPX, BPY, 4, ST7735_GREEN);  // draw ball  in center
    delay(3000);                                // delay 3 seconds
    tft.fillCircle(BPX, BPY, 7, ST7735_BLACK);
    byx=80; 
  }

  DisplayScore(playerScore, computerScore);

  if(playerScore==WINSCORE || computerScore==WINSCORE) {  // if someone hit the winning score then game over - print who one and reset game
    tft.setRotation(1);
    tft.setTextColor(ST7735_WHITE, ST7735_BLACK);
    tft.setCursor(8, 50);
    tft.setTextSize(2);

    if (playerScore==WINSCORE){  // player wins
      tft.fillScreen(ST7735_BLACK);
      tft.print("   YOU WIN!");
      delay(1000);
    }
    else {
      tft.fillScreen(ST7735_BLACK);
      tft.print("Joypad WINS");   // computer wins
      delay(1000);
    }
    //NEWGAME:           //Resets the screen for a new game

    tft.setTextSize(1);
    tft.setTextColor(ST7735_YELLOW, ST7735_BLACK);
    tft.setCursor(8,90);
    tft.print("Press Top Switch 2 To Restart");
    while(Joypad.readButton(SWITCH_R)==LOW||Joypad.readButton(SWITCH_L)==LOW) 
      ;                                            // New game when swtich 4 is pressed
    tft.setRotation(0);
    tft.fillScreen(ST7735_BLACK);
    tft.fillCircle(BPX, BPY, 7, ST7735_BLACK);
    BPX = 15;
    BPY = 15; 
    byx=15;
    bx=1;
    A=1; 
    tft.fillCircle(BPX, BPY, 4, ST7735_GREEN);
    tft.fillCircle(BPX, BPY, 7, ST7735_BLACK);
    tft.fillCircle(BPX, BPY, 4, ST7735_GREEN);
    tft.fillCircle(BPX, BPY, 7, ST7735_BLACK);
    computerScore=0;
    playerScore=0;
    DrawCourt(0);
    DisplayScore(playerScore, computerScore);

    delay(2000);     // wait 4 seconds to start new game
  }
  //If you want, uncomment to use the new scoring method/output using bars
  ////////////////////////////////////////////// 
  // DisplayScoreTicks(playerScore, computerScore);
  ////////////////////////////////////////////

  tft.fillCircle(BPX, BPY, 7, ST7735_BLACK);  //Black sudo ball to errase itself 
  // if little green pixels start to light up due to the ball, change the 6 to a 7
  tft.fillCircle(BPX, BPY, 4, ST7735_GREEN);  //This is the actual ball

}

void DrawCourt(boolean onlycenter) // Draw the play court lines (pass 1 to onlu have the center line drawn for speed)
{
  if(!onlycenter) {
    tft.drawFastVLine(0,0,160,ST7735_GREEN);
    tft.drawFastVLine(127,0,160,ST7735_GREEN);
  }
  tft.drawFastHLine(0, 80, 127, ST7735_GREEN);  // Center Line 
}

void DisplayScore(int playerScore, int computerScore)  // display score with numbers.  Tested for WINSCORE <= 9 (ot greater # digits than 1)
{
  tft.setRotation(1);
  tft.setTextColor(ST7735_GREEN, ST7735_BLACK);
  tft.setCursor(65, 5);
  tft.setTextSize(2);
  tft.print(playerScore);
  tft.setCursor(85, 5);
  tft.print(computerScore);

  tft.setRotation(3);
  tft.setTextColor(ST7735_GREEN, ST7735_BLACK);
  tft.setCursor(65, 10);
  tft.setTextSize(2);
  tft.print(computerScore);
  tft.setCursor(85, 10);
  tft.print(playerScore); 
  tft.setRotation(0);
}







