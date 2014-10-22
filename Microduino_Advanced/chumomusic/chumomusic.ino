int ledPin = A0;

int capval1,capval2,capval3,capval4,capval5,capval6,capval7,capval8;

void setup()
{
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
  Serial.println("Touch senser");
}

void loop ()
{
  digitalWrite(ledPin,LOW);
  capval1 = readCapacitivePin(3);
  capval2 = readCapacitivePin(4);
  capval3 = readCapacitivePin(5);
  capval4 = readCapacitivePin(6);
  capval5 = readCapacitivePin(7);
  capval6 = readCapacitivePin(8);
  capval7 = readCapacitivePin(9);
  capval8 = readCapacitivePin(10);
if (capval1 > 2)
    {
        tone(ledPin, 262, 10);
    }
    if (capval2 > 2)
    {
        tone(ledPin, 294, 10);
    }
    if (capval3 > 2)
    {
        tone(ledPin, 330, 10);
    }
    if (capval4 > 2)
    {
        tone(ledPin, 350, 10);
    }
    if (capval5 > 2)
    {
        tone(ledPin, 393, 10);
    }
    if (capval6 > 2)
    {
        tone(ledPin, 441, 10);
    }
    if (capval7 > 2)
    {
        tone(ledPin, 495, 10);
    }
    if (capval8 > 2)
    {
        tone(ledPin, 525, 10);
    }

}

uint8_t readCapacitivePin(int pinToMeasure) {
  // Variables used to translate from Arduino to AVR pin naming
  volatile uint8_t* port;
  volatile uint8_t* ddr;
  volatile uint8_t* pin;
  // Here we translate the input pin number from
  // Arduino pin number to the AVR PORT, PIN, DDR,
  // and which bit of those registers we care about.
  byte bitmask;
  port = portOutputRegister(digitalPinToPort(pinToMeasure));
  ddr = portModeRegister(digitalPinToPort(pinToMeasure));
  bitmask = digitalPinToBitMask(pinToMeasure);
  pin = portInputRegister(digitalPinToPort(pinToMeasure));
  // Discharge the pin first by setting it low and output
  *port &= ~(bitmask);
  *ddr |= bitmask;
  delay(1);
  // Make the pin an input with the internal pull-up on
  *ddr &= ~(bitmask);
  *port |= bitmask;

  // Now see how long the pin to get pulled up. This manual unrolling of the loop
  // decreases the number of hardware cycles between each read of the pin,
  // thus increasing sensitivity.
  uint8_t cycles = 17;
  if (*pin & bitmask) { 
    cycles = 0;
  }
  else if (*pin & bitmask) { 
    cycles = 1;
  }
  else if (*pin & bitmask) { 
    cycles = 2;
  }
  else if (*pin & bitmask) { 
    cycles = 3;
  }
  else if (*pin & bitmask) { 
    cycles = 4;
  }
  else if (*pin & bitmask) { 
    cycles = 5;
  }
  else if (*pin & bitmask) { 
    cycles = 6;
  }
  else if (*pin & bitmask) { 
    cycles = 7;
  }
  else if (*pin & bitmask) { 
    cycles = 8;
  }
  else if (*pin & bitmask) { 
    cycles = 9;
  }
  else if (*pin & bitmask) { 
    cycles = 10;
  }
  else if (*pin & bitmask) { 
    cycles = 11;
  }
  else if (*pin & bitmask) { 
    cycles = 12;
  }
  else if (*pin & bitmask) { 
    cycles = 13;
  }
  else if (*pin & bitmask) { 
    cycles = 14;
  }
  else if (*pin & bitmask) { 
    cycles = 15;
  }
  else if (*pin & bitmask) { 
    cycles = 16;
  }

  // Discharge the pin again by setting it low and output
  // It's important to leave the pins low if you want to 
  // be able to touch more than 1 sensor at a time - if
  // the sensor is left pulled high, when you touch
  // two sensors, your body will transfer the charge between
  // sensors.
  *port &= ~(bitmask);
  *ddr |= bitmask;

  return cycles;
}

