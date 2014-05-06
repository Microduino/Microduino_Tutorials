#ifndef PAL_CONFIG_H
#define PAL_CONFIG_H

#include "pal_boardtypes.h"

#if (BOARD_TYPE == ZIGDUINO)
#include "return_val.h"

typedef enum led_id_tag
{
    LED_0,
    LED_1,
    LED_2
} led_id_t;

#define NO_OF_LEDS                      (3)

typedef enum button_id_tag
{
    BUTTON_0
} button_id_t;

#define NO_OF_BUTTONS                   (1)

#ifndef F_CPU
#define F_CPU                           (16000000UL)
#endif

#define PAL_WAIT_65_NS()                // empty
#define LED_PORT                        (PORTD)
#define LED_PORT_DIR                    (DDRD)
#define LED_PIN_0                       (PB1)
#define LED_PIN_1                       (PD5)
#define LED_PIN_2                       (PD6)
#define BUTTON_PORT                     (PORTE)
#define BUTTON_PORT_DIR                 (DDRE)
#define BUTTON_INPUT_PINS               (PINE)
#define BUTTON_PIN_0                    (PE0)
#define UART_0_INIT_NON_GENERIC()       // Empty macro
#define UART_1_INIT_NON_GENERIC()       // Empty macro
#ifndef EXTERN_EEPROM_AVAILABLE
#define EXTERN_EEPROM_AVAILABLE            (0)
#endif
#define ALERT_INIT()                    do {    \
        LED_PORT = 0;                           \
        LED_PORT_DIR = 0xFF;                    \
} while (0)
#define ALERT_INDICATE()                do {    \
        LED_PORT ^= 0xFF;                       \
} while (0)
#define TST_PIN_0_HIGH()
#define TST_PIN_0_LOW()
#define TST_PIN_1_HIGH()
#define TST_PIN_1_LOW()
#define TST_PIN_2_HIGH()
#define TST_PIN_2_LOW()
#define TST_PIN_3_HIGH()
#define TST_PIN_3_LOW()
#define TST_PIN_4_HIGH()
#define TST_PIN_4_LOW()
#define TST_PIN_5_HIGH()
#define TST_PIN_5_LOW()
#define TST_PIN_6_HIGH()
#define TST_PIN_6_LOW()
#define TST_PIN_7_HIGH()
#define TST_PIN_7_LOW()
#define TST_PIN_8_HIGH()
#define TST_PIN_8_LOW()

#endif

#endif
