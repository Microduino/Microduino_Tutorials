/*
  Joypad.h - Arduino Joypad board library
  Written by Enrico Gueli
  Copyright (c) 2012 Arduino(TM)  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef Joypad_H_
#define Joypad_H_

#include <Arduino.h>

/*
 * The following constants are used internally by the Joypad
 * library code.
 */

const byte JOYSTICK_BASE  = 16; // it's a "virtual" channel: its ID won't conflict with real ones
const byte JOYSTICK1_BASE  = 20; // it's a "virtual" channel: its ID won't conflict with real ones

const byte MAX_CHANNELS   = 16;

const byte CH_SWITCH_1    = 3;
const byte CH_SWITCH_2    = 2;
const byte CH_SWITCH_3    = 1;
const byte CH_SWITCH_4    = 0;
const byte CH_LIGHT       = 15;
const byte CH_MIC         = 14;
const byte CH_JOYSTICK_SW = 10;
const byte CH_JOYSTICK_X  = 9;
const byte CH_JOYSTICK_Y  = 8;

const byte CH_JOYSTICK1_SW = 11;
const byte CH_JOYSTICK1_X  = 12;
const byte CH_JOYSTICK1_Y  = 13;

const byte CH_SWITCH_L       = 7;
const byte CH_SWITCH_R       = 6;

/*
 * The following constants can be used with the readButton()
 * method.
 */

const byte SWITCH_L       = 7;
const byte SWITCH_R       = 6;

const byte SWITCH_1       = 3;
const byte SWITCH_2       = 2;
const byte SWITCH_3       = 1;
const byte SWITCH_4       = 0;

const byte SWITCH_A  = SWITCH_1;
const byte SWITCH_B  = SWITCH_2;
const byte SWITCH_C  = SWITCH_3;
const byte SWITCH_D  = SWITCH_4;

const byte JOYSTICK_DOWN  = JOYSTICK_BASE;
const byte JOYSTICK_LEFT  = JOYSTICK_BASE+1;
const byte JOYSTICK_UP    = JOYSTICK_BASE+2;
const byte JOYSTICK_RIGHT = JOYSTICK_BASE+3;

const byte JOYSTICK1_DOWN  = JOYSTICK1_BASE;
const byte JOYSTICK1_LEFT  = JOYSTICK1_BASE+1;
const byte JOYSTICK1_UP    = JOYSTICK1_BASE+2;
const byte JOYSTICK1_RIGHT = JOYSTICK1_BASE+3;

const byte JOYSTICK_SW = 10;
const byte JOYSTICK1_SW = 11;

/*
 * These constants can be use for comparison with the value returned
 * by the readButton() method.
 */
const boolean PRESSED   = LOW;
const boolean RELEASED  = HIGH;

/*
 * The following constants can be used with the readTemperature()
 * method to specify the desired scale.
 */
const byte DEGREES_C = 0;
const byte DEGREES_F = 1;



class _Joypad {
private:

  unsigned int readChannel(byte channel);    
  
  boolean joyLowHalf(byte joyCh);
  boolean joyHighHalf(byte joyCh);
    
public:
  _Joypad();
  
  /*
   * Returns a number corresponding to the position of the
   * linear potentiometer. 0 means full right, 1023 means
   * full left.
   */

  /*
   * Returns a number corresponding to the amount of ambient
   * light sensed by the light sensor.
   */
  inline unsigned int readLightSensor() { return readChannel(CH_LIGHT); }


  /*
   * Returns a number corresponding to the amount of ambient noise.
   */
  inline unsigned int readMicrophone() { return readChannel(CH_MIC); }
  
  inline unsigned int readJoystickSwitch() { return readChannel(CH_JOYSTICK_SW); }
  inline unsigned int readJoystick1Switch() { return readChannel(CH_JOYSTICK1_SW); }

  inline int readJoystickX() { 
    return readChannel(CH_JOYSTICK_X) - 512;
  }
  inline int readJoystickY() {
    return readChannel(CH_JOYSTICK_Y) - 512;
  }

  inline int readJoystick1X() { 
    return readChannel(CH_JOYSTICK1_X) - 512;
  }
  inline int readJoystick1Y() {
    return readChannel(CH_JOYSTICK1_Y) - 512;
  }


  /*
   * Reads the current state of a button. It will return
   * LOW if the button is pressed, and HIGH otherwise.
   */
  boolean readButton(byte channel);

  boolean readJoystickButton();
  boolean readJoystick1Button();
  
  
  void tone(unsigned int freq);
  void tone(unsigned int freq, unsigned long duration);
  void noTone();
  
  void motor(unsigned int motor_vol);
  
};



extern _Joypad Joypad;

#endif // Joypad_H_
