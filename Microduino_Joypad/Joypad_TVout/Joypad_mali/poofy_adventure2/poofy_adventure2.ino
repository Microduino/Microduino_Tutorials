/*  ________                    ______
 /    __  \  ______  ______  /   __/---//--/\
 |   /__/  |/ __   \/ __   \/   __/|  /  /  /
 /     ____   /__/    /__/      /\ \|   /  /
 /_____/\   |_______/\______/\__/   |__/  /
 \     \    \       \ \     \ \  \  \  \ / 
 An Adventure Game for the Arduino/Hackvision
 
 2012 - trodoss
 
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 */

#include <stdlib.h> 
#include <TVout.h>
#include <Controllers.h>
#include <font4x6.h>

#define SCREENWIDTH     128
#define SCREENHEIGHT    96

#define PAUSE_BETWEEN_ACTIONS 50

#define GAME_TITLE   0
#define GAME_PLAYING 1
#define GAME_PAUSED  2
#define GAME_WON     3
#define GAME_OVER    4

#include "sound.h"
#include "logo.h"
#include "level.h"
#include "poofy.h"
#include "display.h"

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
char game_state;

void setup()
{
  TV.begin(_NTSC, SCREENWIDTH, SCREENHEIGHT);
  TV.select_font(font4x6);

  randomSeed(analogRead(0));

  start_title();

  tft.initR(INITR_BLACKTAB); 
  tft.fillScreen(ST7735_BLACK);

  tft.setCursor(4, 0);
  tft.setTextSize(2);
  tft.println("Microduino");
  tft.setTextColor(ST7735_WHITE);
}

void loop()
{  
  static long next_action = 0;
  switch (game_state)
  {
  case GAME_TITLE:
  case GAME_OVER:
  case GAME_WON:
    if (TV.millis() >= next_action)
    {
      //wait for a button to be pressed to continue
      if (!Joypad.readButton(CH_JOYSTICK1_SW)||!Joypad.readButton(CH_JOYSTICK_SW))
      {	   
        game_state = GAME_PLAYING;
        start_game();
      }

      next_action = TV.millis() + PAUSE_BETWEEN_ACTIONS; 
    }	
    sound_update();
    break;

  case GAME_PLAYING:
    if (TV.millis() >= next_action)
    {      
      if (Joypad.readJoystick1X()<-100||Joypad.readJoystickX()<-100) poofy_move(FACING_RIGHT);
      if (Joypad.readJoystickX()>100||Joypad.readJoystick1X()>100) poofy_move(FACING_LEFT);
      if (Joypad.readJoystick1Y()<-100||Joypad.readJoystickY()<-100) poofy_jump();//up
      //(Joypad.readJoystick1Y()>100||Joypad.readJoystickY()>100)   down
      next_action = TV.millis() + PAUSE_BETWEEN_ACTIONS; 

      level_update();

      //check the poofy's state
      if (poofy_getState() > POOFY_FALLING)
      {
        if (poofy_getState() == POOFY_DEAD)
        {
          start_game_over();
        } 
        else {
          start_game_won();
        }
      }
    }
    sound_update();
    break;

  }
}

void start_game()
{
  game_state = GAME_PLAYING;
  poofy_init();
  level_draw();
  sound_play_song(1);
}

void start_title()
{
  TV.clear_screen();
  game_state = GAME_TITLE;
  logo_draw();
  sound_play_song(0);
}

void start_game_over()
{
  TV.clear_screen();
  TV.printPGM(48, 40, PSTR("Game Over"));
  game_state = GAME_OVER;
  display_update(0);
  sound_play_song_once(2);
}  

void start_game_won()
{
  TV.clear_screen();
  TV.printPGM(48, 40, PSTR("Victory!!"));
  game_state = GAME_WON;
  display_update(0);
  sound_play_song_once(3);
}

