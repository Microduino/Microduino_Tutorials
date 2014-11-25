#include <Joypad.h> 
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library
#include <SPI.h>

#define cs   5
#define dc   4
#define rst  0  // you can also connect this to the Arduino reset

Adafruit_ST7735 tft = Adafruit_ST7735(cs, dc, rst);

#define speak   6
///////////////////////////////////////////////////////////////

int joyx,joyy;
int block;

unsigned int background = 0x0000;   // ST7735_BLACK

struct Vec {
  int x, y;
};

typedef Vec BlockDef[4];

BlockDef BlockDefs[] = {
	{{0,1}, {0,0},  {0,-1}, {0,-2}}, // |
	{{0,0}, {0,-1}, {1,0},  {1,1} }, // Z
	{{0,0}, {0,-1}, {-1,0}, {-1,1}}, // S
	{{0,0}, {0,1},  {1,0},  {1,1} }, // X
	{{0,0}, {-1,0}, {1,0},  {0,1} }, // T
	{{0,0}, {-1,0}, {1,0},  {1,1} }, // L
	{{0,0}, {-1,0}, {1,0},  {-1,1}}, // Sq
};
 
int8_t BlockColors[][3] = {
	{255, 255, 255},
	{255, 0, 0},
	{0, 255, 0},
	{0, 0, 255},
	{255, 255, 0},
	{255, 0, 255},
	{0, 255, 255},
};

class Block {
public:
  BlockDef Def;
  BlockDef TransformedBlocks;
  int8_t Rotation;
  int32_t Color;
  Vec Position;

  void UpdateTransformed() {
    for (int i = 0; i < 4; ++i) {
      if (Rotation == 0) {
        TransformedBlocks[i].x = Def[i].x + Position.x;
        TransformedBlocks[i].y = Def[i].y + Position.y;
      } 
      else if (Rotation == 1) {
        TransformedBlocks[i].x = -Def[i].y + Position.x;
        TransformedBlocks[i].y =  Def[i].x + Position.y;
      } 
      else if (Rotation == 2) {
        TransformedBlocks[i].x = -Def[i].x + Position.x;
        TransformedBlocks[i].y = -Def[i].y + Position.y;
      } 
      else if (Rotation == 3) {
        TransformedBlocks[i].x =  Def[i].y + Position.x;
        TransformedBlocks[i].y = -Def[i].x + Position.y;
      }
    }
  }
};

class Game {
public:
  void Reset() {
    tft.fillScreen(ST7735_BLACK);
    HasCurrentBlock = false;
    for (int x = 0; x < Width; ++x)
      for (int y = 0; y < Height; ++y)
        Board[x][y] = 0;
    BlockLastMovedAt = millis();
    LastShiftedAt = 0;
    LastRotatedAt = 0;
    BlockDrawDirty = true;
    BoardDrawDirty = true;
    NeedsInitialDraw = true;
    Score = 0;
  }

  void Tick(int16_t tm, int16_t dt) {
    if (HasCurrentBlock) {
      //controls
      
      if(Joypad.readButton(CH_SWITCH_R))
      {
      if(Joypad.readButton(CH_SWITCH_2)==0)
      joyx=100;
      else if(Joypad.readButton(CH_SWITCH_4)==0)
      joyx=-100;
       if(Joypad.readButton(CH_SWITCH_1)==0)
      joyy=-100;
      else if(Joypad.readButton(CH_SWITCH_3)==0)
      joyy=100;
      else if(Joypad.readButton(CH_SWITCH_1)==1&&Joypad.readButton(CH_SWITCH_2)==1&&Joypad.readButton(CH_SWITCH_3)==1&&Joypad.readButton(CH_SWITCH_4)==1)
      {
      joyx=0;
      joyy=0;
      }
      }
      else
      {
      joyx = map(Joypad.readJoystickX(), -280, 300, 100, -100);
      joyy = map(Joypad.readJoystickY(), -300, 230, -100, 100);
      }
      //  Serial.print("joyx:");
      //  Serial.println(joyx);
      //   Serial.print("joyy:");
      //   Serial.println(joyy);
      //delay(100);
      if (abs(joyx) > 20) {
        if ((135-abs(joyx))*4 < (tm-LastShiftedAt)) {
          LastShiftedAt = tm;
          int dir;
          if (joyx > 0)
            dir = 1;
          else
            dir = -1;
          if (!BlockDrawDirty)
            LastBlock = CurrentBlock;
          Move(dir);
          if (IsIntersecting())
            Move(-dir);
          else {
            BlockDrawDirty = true;
          }
        }
      }
      if (abs(joyy) > 0) {
        if (joyy < -10) {
          if ((135+joyy)*11 < (tm-LastRotatedAt)) {
            LastRotatedAt = tm;
            if (!BlockDrawDirty) 
              LastBlock = CurrentBlock;
            Rotate(1);
            if (IsIntersecting()) {
              //try moves
              Move(1);
              if (IsIntersecting()) {
                Move(-1); 
                Move(-1);
                if (IsIntersecting()) {
                  Rotate(-1);
                } 
                else {
                  BlockDrawDirty = true;
                }
              } 
              else {
                BlockDrawDirty = true;
              }
            } 
            else {
              BlockDrawDirty = true;
            }
          }
        }
      }

      //dropping
      int waitTime = (joyy > 20) ? 100 : 500;
      if (tm - BlockLastMovedAt > waitTime) {
        BlockLastMovedAt = millis();
        if (CanDrop()) {
          if (!BlockDrawDirty)
            LastBlock = CurrentBlock;
          Drop();
          BlockDrawDirty = true;
        } 
        else {
          if (CurrentBlock.Position.y == 2) {
            gameover();
            Reset();   
          } 
          else {
            Joypad.motor(0);
            PlaceBlock();
            ClearLines();
          }
        }
      }
    } 
    else {
      NextBlock();
    }
  }

  void Draw() {
    int xstep = 8; //tft.width()/Width - 4;
    int ystep = 8; //tft.height()/Height - 4;
    int ox = 5, oy = 5;

    //
    if (BoardDrawDirty) {
      BoardDrawDirty = false;
      for (int x = 0; x < Width; ++x) {
        for (int y = 0; y < Height; ++y) {
          tft.fillRect(ox + x*xstep, oy + y*ystep, 
          xstep-1, ystep-1, Board[x][y]);
        }
      }
    }

    //tft.fillRect(0, 0, 30, 20, ST7735_BLACK);
    tft.drawRect(0, 0, xstep*Width + 8, ystep*Height + 8, tft.Color565(232,80,215));
    //tft.drawRect(2, 2, xstep*Width + 4, ystep*Height + 4, tft.Color565(255,255,255));

    tft.setTextColor(ST7735_GREEN);
    tft_char("Joypad",90,4,1);

    tft.setTextColor(ST7735_BLUE);
    tft_char("Score:",90,19,1);

    if(Score<100)
    {
      tft.drawChar(90, 30, '0' + (Score/10), ST7735_RED, ST7735_BLACK, 2);   
      tft.drawChar(101, 30, '0' + (Score%10), ST7735_RED, ST7735_BLACK, 2);
    }
    else
    {
      tft.drawChar(90, 30, '0' + (Score/100), ST7735_RED, ST7735_BLACK, 2);   
      tft.drawChar(101, 30, '0' + ((Score-100)/10), ST7735_RED, ST7735_BLACK, 2);
      tft.drawChar(112, 30, '0' + (Score%10), ST7735_RED, ST7735_BLACK, 2);
    }

    //update the block region
    if (BlockDrawDirty) {
      BlockDrawDirty = false;
      //
      int8_t dirtyRegion[6][6] = {
        0                                                };
      int8_t dirtyX = 64, dirtyY = 64;
      for (int i = 0; i < 4; ++i) {
        int8_t x = CurrentBlock.TransformedBlocks[i].x;
        int8_t y = CurrentBlock.TransformedBlocks[i].y;
        if (x < dirtyX) dirtyX = x;
        if (y < dirtyY) dirtyY = y;
        if (!NeedsInitialDraw) {
          int8_t ox = LastBlock.TransformedBlocks[i].x;
          int8_t oy = LastBlock.TransformedBlocks[i].y;
          if (ox < dirtyX) dirtyX = ox;
          if (oy < dirtyY) dirtyY = oy;
        }
      }

      //write the new cells to draw to the dirtyRegion
      for (int i = 0; i < 4; ++i) {
        int x = CurrentBlock.TransformedBlocks[i].x;
        int y = CurrentBlock.TransformedBlocks[i].y;
        dirtyRegion[x-dirtyX][y-dirtyY] = 1; //draw cell
      }

      //write the erase and already drawn cells
      if (!NeedsInitialDraw) {
        for (int i = 0; i < 4; ++i) {
          int x = LastBlock.TransformedBlocks[i].x;
          int y = LastBlock.TransformedBlocks[i].y;
          if (dirtyRegion[x-dirtyX][y-dirtyY] == 1) {
            dirtyRegion[x-dirtyX][y-dirtyY] = 0;
          } 
          else {
            dirtyRegion[x-dirtyX][y-dirtyY] = -1; //erase cell
          }
        }
      }

      //draw out the tiles in question
      for (int x = 0; x < 6; ++x) {
        for (int y = 0; y < 6; ++y) {
          int flag = dirtyRegion[x][y];
          if (flag == -1) {
            tft.fillRect(ox + (x+dirtyX)*xstep,
            oy + (y+dirtyY)*ystep,
            xstep-1, ystep-1, 0);						
          } 
          else if (flag == 1) {
            tft.fillRect(ox + (dirtyX+x)*xstep,
            oy + (dirtyY+y)*ystep,
            xstep-1, ystep-1, CurrentBlock.Color);
          }
        }
      }

      //update needsinitialdraw
      NeedsInitialDraw = false;
    }
  }

private:
  void NextBlock() {
    block = random() % 7;
    for (int i = 0; i < 4; ++i)
      CurrentBlock.Def[i] = BlockDefs[block][i];
    CurrentBlock.Rotation = 0;
    CurrentBlock.Color = tft.Color565(BlockColors[block][0],
    BlockColors[block][1],
    BlockColors[block][2]);
    CurrentBlock.Position.x = 4;
    CurrentBlock.Position.y = 2;
    CurrentBlock.UpdateTransformed();
    HasCurrentBlock = true;
    NeedsInitialDraw = true;
  }

  bool IsIntersecting() {
    for (int i = 0; i < 4; ++i) {
      int x = CurrentBlock.TransformedBlocks[i].x;
      int y = CurrentBlock.TransformedBlocks[i].y;
      if (   y >= Height || y < 0 
        || x >= Width  || x < 0 
        || Board[x][y] != 0) 
      {
        return true;
      }
    }
    return false;
  }

  bool CanDrop() {
    for (int i = 0; i < 4; ++i) {
      int x = CurrentBlock.TransformedBlocks[i].x;
      int y = CurrentBlock.TransformedBlocks[i].y;
      if (y+1 == Height || Board[x][y+1] != 0) {
        return false;
      }
    }
    return true;
  }

  void Rotate(int8_t dir) {
    CurrentBlock.Rotation = CurrentBlock.Rotation + dir;
    if (CurrentBlock.Rotation >= 4) {
      CurrentBlock.Rotation = 0;
    } 
    else if (CurrentBlock.Rotation < 0) {
      CurrentBlock.Rotation = 3;
    }
    CurrentBlock.UpdateTransformed();
  }

  void Move(int8_t dir) {
    CurrentBlock.Position.x += dir;
    CurrentBlock.UpdateTransformed();
  }

  void PlaceBlock() {
    for (int i = 0; i < 4; ++i) {
      int x = CurrentBlock.TransformedBlocks[i].x;
      int y = CurrentBlock.TransformedBlocks[i].y;
      if (InBounds(x, y))
        Board[x][y] = CurrentBlock.Color;
    }
    HasCurrentBlock = false;
  }

  void ClearLines() {
    int totalScore = 0;
    for (int row = Height-1; row >= 0; --row) {
      bool rowFull = true;
      for (int x = 0; x < Width; ++x) {
        if (Board[x][row] == 0)
          rowFull = false;
      }

      if (rowFull) {
        for (int shiftIntoRow = row; shiftIntoRow > 0; --shiftIntoRow) {
          for (int x = 0; x < Width; ++x)
            Board[x][shiftIntoRow] = Board[x][shiftIntoRow-1]; 
        }
        row++;
        totalScore++;
        Score += totalScore;
        BoardDrawDirty = true;
        digitalWrite(speak,HIGH);
        delay(100);
      }
      else
        digitalWrite(speak,LOW);
    }
  }
  
  void gameover()
{
  Joypad.motor(255);
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
  tft.print(" Your Score:");
  tft.setTextSize(2);
  tft.setTextColor(ST7735_GREEN);
  tft.println(Score);
  tft.println("");
  tft.setTextSize(1);
  tft.setTextColor(ST7735_RED);
  tft.println("Microduino Joypad");
  delay(1500);
}

  void Drop() {
    CurrentBlock.Position.y = CurrentBlock.Position.y + 1;
    CurrentBlock.UpdateTransformed();
  }

  bool InBounds(int x, int y) {
    return x >= 0 && x < Width && y >= 0 && y < Height;
  }
  
private:
  static const int32_t Width = 10;
  static const int32_t Height = 18;
  bool HasCurrentBlock;
  Block CurrentBlock;
  Block LastBlock;
  bool BlockDrawDirty;
  bool BoardDrawDirty;
  bool NeedsInitialDraw;
  int16_t LastShiftedAt;
  int16_t LastRotatedAt;
  int16_t BlockLastMovedAt;
  int32_t Board[Width][Height];
  int16_t Score;
};

void tft_char(char* zi,int x,int y,int sz)
{
  tft.setTextSize(sz);
  tft.setCursor(x,y);
  tft.print(zi);
}

void setup() {
  Serial.begin(9600);
  //
  tft.initR(INITR_REDTAB);
  tft.fillScreen(ST7735_BLACK);

  pinMode(speak,OUTPUT);
}

void loop() {
  //
  Game game;
  game.Reset();
  //
  int starttm = millis();
  int lasttm = starttm;
  while (true) {
    int now = millis();
    int dt = lasttm-now;
    //
    game.Tick(now, dt);
    game.Draw();
  }
}
