#include <Adafruit_NeoPixel.h>
#define PIN 6
Adafruit_NeoPixel strip = Adafruit_NeoPixel(6, PIN, NEO_GRB + NEO_KHZ800);

uint32_t color_m[9][3] =
{
  {0, 255, 255},
  {255, 0, 0},
  {248, 141, 30},
  {255, 255, 0},
  {0, 255, 0},
  {0, 127, 255},
  {0, 0, 255},
  {139, 0, 255},
  {255, 255, 255}
};
uint32_t color[9] =
{
  strip.Color(0, 0, 0), strip.Color(255, 0, 0), strip.Color(248, 141, 30), strip.Color(255, 255, 0), strip.Color(0, 255, 0), strip.Color(0, 127, 255), strip.Color(0, 0, 255), strip.Color(139, 0, 255), strip.Color(255, 255, 255)
};

#define JoystickX_PIN A1
#define JoystickY_PIN A0

int sensorValueX, sensorValueY;

int num, color_l;

#define val_max 255
#define val_min 0

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'

  pinMode(JoystickX_PIN, INPUT);
  pinMode(JoystickY_PIN, INPUT);

  for (int i = 0; i < 9; i++)
  {
    colorWipe(color[i]);
    delay(300);
  }

  colorWipe(color[0]);
}

void loop() {
  // put your main code here, to run repeatedly:
  sensorValueX = analogRead(JoystickX_PIN);
  sensorValueY = analogRead(JoystickY_PIN);

  if (sensorValueY <= 10)
  {
    delay(500);
    num++;
    if (num > 8)
      num = 0;
  }
  else if (sensorValueY > 800)
  {
    delay(500);
    num--;
    if (num < 0)
      num = 8;
  }

  if (sensorValueX <= 10)
  {
    delay(10);
    color_l++;
    if (color_l > 255)
      color_l = 255;
  }
  else if (sensorValueX > 800)
  {
    delay(10);
    color_l--;
    if (color_l < 0)
      color_l = 0;
  }

  colorWipe(strip.Color(map(color_l, val_min, val_max, 0, color_m[num][0]), map(color_l, val_min, val_max, 0, color_m[num][1]), map(color_l, val_min, val_max, 0, color_m[num][2])));
  /*
    Serial.print(num);
    Serial.print("\t");
    Serial.println(color_l);
    */
}

void colorWipe(uint32_t c) {
  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
  }
}
