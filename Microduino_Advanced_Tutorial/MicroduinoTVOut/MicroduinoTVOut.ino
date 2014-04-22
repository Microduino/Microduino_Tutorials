#include <TVout.h>
#include <fontALL.h>
#include <stdint.h>
const uint8_t WIDTH = 128;
const uint8_t HEIGHT = 96;
TVout tv;
void setup() {
  tv.begin(PAL, WIDTH, HEIGHT);
  tv.select_font(font4x6);
}
void loop() {
  tv.clear_screen();
  tv.print(30, 40, " Hello Microduino");
  delay(3000);
  tv.clear_screen();
  tv.print(0, 0, " Let's draw a line:");
  tv.draw_line(0, 10, WIDTH - 1, HEIGHT - 1, WHITE);
  delay(3000);
  tv.clear_screen();
  tv.print(0, 0, " Now let's draw a rectangle:");
  tv.draw_rect(0, 10, WIDTH - 11, HEIGHT - 11, WHITE);
  delay(3000);
  tv.clear_screen();
  tv.print(0, 0, " And here we have a circle:");
  tv.draw_circle(WIDTH / 2, HEIGHT / 2, 20, WHITE);
  delay(3000);
}

