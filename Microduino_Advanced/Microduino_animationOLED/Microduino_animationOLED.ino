#include <avr/sleep.h>
#include <avr/power.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "bitmaps.h" // Toaster graphics data is in this header file

int pin2 = 2;


#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif
 

#define OLED_RESET 4 // because the Pro Trinket has no pin 2 or 7.
// Hardware SPI for Data & Clk -- pins 11 & 13 on Uno or Pro Trinket.
Adafruit_SSD1306 display(OLED_RESET);
 
#define N_FLYERS 5 // Number of flying things
 
struct Flyer { // Array of flying things
int16_t x, y; // Top-left position * 16 (for subpixel pos updates)
int8_t depth; // Stacking order is also speed, 12-24 subpixels/frame
uint8_t frame; // Animation frame; Toasters cycle 0-3, Toast=255
} flyer[N_FLYERS];
 
//uint32_t startTime;


/***************************************************
 *  Name:        pin2Interrupt
 *
 *  Returns:     Nothing.
 *
 *  Parameters:  None.
 *
 *  Description: Service routine for pin2 interrupt  
 *
 ***************************************************/
void pin2Interrupt(void)
{
  /* This will bring us back from sleep. */
  
  /* We detach the interrupt to stop it from 
   * continuously firing while the interrupt pin
   * is low.
   */
  detachInterrupt(0);
}


/***************************************************
 *  Name:        enterSleep
 *
 *  Returns:     Nothing.
 *
 *  Parameters:  None.
 *
 *  Description: Enters the arduino into sleep mode.
 *
 ***************************************************/
void enterSleep(void)
{
  
  /* Setup pin2 as an interrupt and attach handler. */
  attachInterrupt(0, pin2Interrupt, LOW);
  //attachInterrupt(0, pin2Interrupt, CHANGE);
  delay(100);
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  
  display.ssd1306_command(SSD1306_DISPLAYOFF); // Screen off
  power_spi_disable(); // Disable remaining periphs
  power_timer0_disable();
  sleep_enable();
  sleep_mode();

  /* The program will continue from here. */
  
  /* First thing to do is disable sleep. */
  sleep_disable(); 


  // Execution resumes here on wake.
  power_spi_enable(); // Re-enable SPI
  power_timer0_enable(); // and millis(), etc.
  display.ssd1306_command(SSD1306_DISPLAYON); // Main screen turn on
}


/***************************************************
 *  Name:        setup
 *
 *  Returns:     Nothing.
 *
 *  Parameters:  None.
 *
 *  Description: Setup for the Arduino.           
 *
 ***************************************************/
void setup()
{
  Serial.begin(9600);
  
  /* Setup the pin direction. */
  pinMode(pin2, INPUT);
  
  //Serial.println("Initialisation complete.");


  display.begin(SSD1306_SWITCHCAPVCC); // Init screen
  display.clearDisplay();
  for(uint8_t i=0; i<N_FLYERS; i++) { // Randomize initial flyer states
  flyer[i].x = (-32 + random(160)) * 16;
  flyer[i].y = (-32 + random(96)) * 16;
  flyer[i].frame = random(3) ? random(4) : 255; // 66% toaster, else toast
  flyer[i].depth = 10 + random(16); // Speed / stacking order
  }
  qsort(flyer, N_FLYERS, sizeof(struct Flyer), compare); // Sort depths


}



/***************************************************
 *  Name:        loop
 *
 *  Returns:     Nothing.
 *
 *  Parameters:  None.
 *
 *  Description: Main application loop.
 *
 ***************************************************/
int seconds=0;
void loop()
{

  uint8_t i, f;
  int16_t x, y;
  boolean resort = false; // By default, don't re-sort depths
   
  display.display(); // Update screen to show current positions
  display.clearDisplay(); // Start drawing next frame
   
  for(i=0; i<N_FLYERS; i++) { // For each flyer...
   
  // First draw each item...
  f = (flyer[i].frame == 255) ? 4 : (flyer[i].frame++ & 3); // Frame #
  x = flyer[i].x / 16;
  y = flyer[i].y / 16;
  display.drawBitmap(x, y, (const uint8_t *)pgm_read_word(&mask[f]), 32, 32, BLACK);
  display.drawBitmap(x, y, (const uint8_t *)pgm_read_word(&img[ f]), 32, 32, WHITE);
   
  // Then update position, checking if item moved off screen...
  flyer[i].x -= flyer[i].depth * 2; // Update position based on depth,
  flyer[i].y += flyer[i].depth; // for a sort of pseudo-parallax effect.
  if((flyer[i].y >= (64*16)) || (flyer[i].x <= (-32*16))) { // Off screen?
  if(random(7) < 5) { // Pick random edge; 0-4 = top
  flyer[i].x = random(160) * 16;
  flyer[i].y = -32 * 16;
  } else { // 5-6 = right
  flyer[i].x = 128 * 16;
  flyer[i].y = random(64) * 16;
  }
  flyer[i].frame = random(3) ? random(4) : 255; // 66% toaster, else toast
  flyer[i].depth = 10 + random(16);
  resort = true;
  }
  }
  // If any items were 'rebooted' to new position, re-sort all depths
  if(resort) qsort(flyer, N_FLYERS, sizeof(struct Flyer), compare);
     


  delay(100);
  seconds++;
  
  //Serial.print("Awake for ");
  //Serial.print(seconds, DEC);
  //Serial.println(" second");
  
  if(seconds == 30)
  {
    //Serial.println("Entering sleep");
    delay(200);
    seconds = 0;
    enterSleep();
  }
  
}


// Flyer depth comparison function for qsort()
static int compare(const void *a, const void *b) {
return ((struct Flyer *)a)->depth - ((struct Flyer *)b)->depth;
}
