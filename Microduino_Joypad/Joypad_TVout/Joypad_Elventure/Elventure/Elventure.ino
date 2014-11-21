/*                                   __    
                                   /___/
                     ________      / /
   /-------------/  /  ____  \  __/_/__
     |  |-------/  /__/ //===//__<*>_ //===//
     |  .----/        _ / _/   / / /
     |  .---/      /  /____   / / /
     |  |------/  /________/ / / /
   /----------/ L + V + E + N + T + U + R + E *=-
   An Adventure Game for the Arduino/Hackvision Video Game Platform
   v2.0

   2011 - trodoss
   
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License along
   with this program; if not, write to the Free Software Foundation, Inc.,
   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.   
   
 */
 
#include <stdlib.h> 
#include <TVout.h>
#include <Controllers.h>
#include <font4x6.h>

#include <Joypad.h>

#include "map.h"
#include "room.h"
#include "elf.h"
#include "sound.h"
#include "display.h"
#include "logo.h"

#define SCREENWIDTH     128
#define SCREENHEIGHT    96
boolean palJumper;
#define PALpin 12

#define PAUSE_BETWEEN_ACTIONS 100

#define GAME_TITLE   0
#define GAME_PLAYING 1
#define GAME_WON     3
#define GAME_OVER    4
TVout TV;
char game_state;

void setup()
{
  palJumper = digitalRead( PALpin );
  TV.begin( palJumper ? NTSC : PAL, SCREENWIDTH, SCREENHEIGHT );
  TV.select_font(font4x6);
  
  randomSeed(analogRead(0));
  
  start_title();
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
		   
		   //check to see what we are doing when we start the game
		   if (game_state > GAME_TITLE)
		   {
		     if (game_state == GAME_WON)
			 {
			   //if the game has been won, then reset the inventory of the elf
			   resetElf(true);
			 } else {
			   //if the game was just over (died) then do not reset the inventory
		       resetElf(false);
			 }
		   }
		   
		   game_state = GAME_PLAYING;
		   start_game();
         }
		         
         next_action = TV.millis() + PAUSE_BETWEEN_ACTIONS; 
       }	
	   update_sound();
	   break;
	   
	case GAME_PLAYING:
       if (TV.millis() >= next_action)
       {      
         if (Joypad.readJoystick1Y()<-100||Joypad.readJoystickY()<-100) moveElf(FACING_UP);
         if (Joypad.readJoystick1Y()>100||Joypad.readJoystickY()>100) moveElf(FACING_DOWN);
         if (Joypad.readJoystick1X()<-100||Joypad.readJoystickX()<-100) moveElf(FACING_RIGHT);
         if (Joypad.readJoystickX()>100||Joypad.readJoystick1X()>100) moveElf(FACING_LEFT);
         if (!Joypad.readButton(CH_JOYSTICK1_SW)||!Joypad.readButton(CH_JOYSTICK_SW)) throwSword();
    
         next_action = TV.millis() + PAUSE_BETWEEN_ACTIONS; 
	
         handleRoomElements();
		 
		 //check the elf's state
		 if (getElfState() != ELFSTATE_PLAYING)
		 {
		   if (getElfState() == ELFSTATE_DEAD)
		   {
		     start_game_over();
		   } else {
		     start_game_won();
		   }
		 }
       }
       update_sound();
       break;
  }
}

void start_game()
{
  //clear the contents of the screen
  TV.clear_screen();
  
  //start the game
  setMapRoom(0);
  showElf();
  updateDisplay(getElf());
  play_song(0);
  game_state = GAME_PLAYING;  
}

void start_title()
{
  //clear the contents of the screen
  TV.clear_screen();
  
  game_state = GAME_TITLE;
  drawLogo(0);
  play_song(4);
}

void start_game_over()
{
  //clear the contents of the screen
  TV.clear_screen();
  
  game_state = GAME_OVER;
  drawLogo(4);
  play_song_once(2);
}

void start_game_won()
{
  //clear the contents of the screen
  TV.clear_screen();
  
  game_state = GAME_WON;
  drawLogo(8);
  play_song(3);
}
