#include <Joypad.h>

#include <SPI.h>
#include <TFT.h>
#include "colour.h"

/* TFT pins */
#define LCD_CS   5
//#define SD_CS 4
#define DC   4
#define RESET  8

#define Shock 8

#define screenWidth 128
#define screenHeight 160
#define tileSize 6

float x,y,x1,y1;
int Joy;
boolean Joy_sw,Joy1_sw;

TFT screen = TFT(LCD_CS, DC, RESET);

long timestamp = 0;
int score = 0;
int collision = 0;
int goX = 0;
int goY = 0;

struct coord_struct{
  int x;
  int y;
  struct coord_struct *next_ptr; 
};
typedef struct coord_struct coord;

coord *new_ptr;
coord *temp;
coord *head = NULL;

struct {
  int x;
  int y;
  int growBy;
} 
food;

/*****************************************************************
 *  FUNCTIONS
 */

void  addFirst(int x, int y){
  new_ptr = (coord *)malloc(sizeof(coord));
  new_ptr->x = x;
  new_ptr->y = y;
  new_ptr->next_ptr = head;
  head = new_ptr;
}

void  remLast(){
  coord *last;  
  temp = head;
  last = temp;
  while (temp != NULL && temp->next_ptr != NULL){
    last = temp;
    temp = temp->next_ptr;
  }
  if (last == temp){
    free(temp);
    head = NULL;
  } 
  else {
    drawRect(temp->x,temp->y,&bgCol); /* draw */
    free(last->next_ptr);
    last->next_ptr = NULL;  
  }
}

void startScreen(){
  reset();

  screen.background(0, 0, 120);
  screen.setTextSize(4);
  screen.stroke(0,255,0);
  screen.setRotation(4);
  screen.text("Snake",4,10);
  screen.setTextSize(1);
  screen.text("Plase move to start",4, 80);

  while(goX==0&&goY==0)
  {
    joypad();
    delay(5);
  }
  screen.stroke(0,0,0);
  screen.fill(bgCol.r,bgCol.g,bgCol.b);
  screen.rect(3,2,screenWidth-6,screenHeight-4);

  for (int i = 29; i<=53; i+=6){
    addFirst(i,52);
    drawRect(head->x, head->y,&snakeCol);
  }

  putFood();
}

void reset(){
  goX = 0;
  goY = 0;
  collision = 0;  
  score = 0;
  while(head){
    remLast();
  }
}

void getInput(){
  if (Joy==2){
    goX = -tileSize; 
    goY = 0;  
  }
  else if (Joy==4){
    goX = 0; 
    goY = tileSize;  
  }
  else if (Joy==1){
    goX = 0; 
    goY = -tileSize;  
  }
  else if (Joy==3){
    goX = tileSize; 
    goY = 0;  
  }
}

int isGrow(){
  if(head->y == food.y && head->x == food.x){
    return 1; 
  } 
  else {
    return 0;
  }
}

int isCollision(){
  temp = head->next_ptr;
  while(temp){
    if (temp->y == head->y && temp->x == head->x){
      return 1;
    }
    temp = temp->next_ptr;  
  }
  if(head->y < 5 || head->y > 149 || head->x < 4 || head->x > 118){
    return 1;
  }
  return 0;
}

void putFood(){
  int validPlace = 0;
  while (!validPlace){
    food.x = 5 + (int)random(0,19)*6;
    food.y = 4 + (int)random(0,24)*6;
    if (food.x != head->y && food.y != head->x){
      validPlace = 1; 
    }
  }
  drawRect(food.x, food.y, &foodCol);
}

void drawRect(int x, int y, colour *c){
  screen.noStroke();
  screen.fill(c->r, c->g, c->b);
  screen.rect(x,y,tileSize,tileSize);
}

void endScreen(){
  digitalWrite(Shock,HIGH); 

  char scoreChars[5];
  screen.background(0, 0, 0);
  screen.stroke(255,255,255);
  screen.setRotation(4);
  String scorestring = String(score);
  scorestring.toCharArray(scoreChars,5);
  screen.setTextSize(2);
  screen.text("Game over", 10, 10);
  screen.text("Score:", 10, 50);
  screen.setTextSize(4);
  screen.text(scoreChars, 80, 40);  
  delay(1000);
  digitalWrite(Shock,LOW); 
}

void setup(){
  screen.begin();
  randomSeed(analogRead(0));
  pinMode(Shock,OUTPUT);
  digitalWrite(Shock,LOW); 
}

void loop(){
  startScreen();
  while (!collision){
    joypad();

    delay(5);
    if (millis()-timestamp > 100){
      addFirst((head->x + goX), (head->y + goY));
      drawRect(head->x,head->y,&snakeCol);
      if (food.growBy){
        food.growBy--;
        score += 1;
        digitalWrite(Shock,HIGH); 
      }
      else {
        remLast();
        digitalWrite(Shock,LOW); 
      }
      if (isGrow()){
        food.growBy++;
        putFood(); 
      }
      if (isCollision()){
        collision = 1;
      }
      timestamp = millis();
    }
  }
  endScreen(); 
}

void joypad()
{
  x=Joypad.readJoystickX();
  y=Joypad.readJoystickY();

  x1=Joypad.readJoystick1X();
  y1=Joypad.readJoystick1Y();

  if(Joypad.readButton(CH_SWITCH_R))
  {
    if(Joypad.readButton(CH_SWITCH_2)==0)
      Joy=3;
    else if(Joypad.readButton(CH_SWITCH_4)==0)
      Joy=2;
    if(Joypad.readButton(CH_SWITCH_1)==0)
      Joy=1;
    else if(Joypad.readButton(CH_SWITCH_3)==0)
      Joy=4;
    else if(Joypad.readButton(CH_SWITCH_1)==1&&Joypad.readButton(CH_SWITCH_2)==1&&Joypad.readButton(CH_SWITCH_3)==1&&Joypad.readButton(CH_SWITCH_4)==1)
      Joy=0;
  }
  else
  {
    if(x>280||x1>280)
      Joy=2;//
    else if(x<-280||x1<-280)
      Joy=3;//
    else if(y<-280||y1<-280)
      Joy=1;
    else if(y>280||y1>280)
      Joy=4;
    else
      Joy=0;
  }
  getInput();
}











