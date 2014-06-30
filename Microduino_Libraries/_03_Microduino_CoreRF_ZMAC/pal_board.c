#include <ZMAC.h>
#include <stdbool.h>
#include <stdlib.h>
#include "pal.h"
#include "pal_boardtypes.h"
#include "pal_config.h"

#if (BOARD_TYPE == ZIGDUINO)
bool pal_calibrate_rc_osc(void)
{
    return true;
}
void pal_led_init(void)
{
    LED_PORT |= (1 << LED_PIN_0);
    LED_PORT_DIR |= (1 << LED_PIN_0);
    LED_PORT |= (1 << LED_PIN_1);
    LED_PORT_DIR |= (1 << LED_PIN_1);
    LED_PORT |= (1 << LED_PIN_2);
    LED_PORT_DIR |= (1 << LED_PIN_2);
}
void pal_led(led_id_t led_no, led_action_t led_setting)
{
    switch (led_no)
    {
    default:
    case 0:
        switch (led_setting)
        {
            case LED_ON:    LED_PORT &= ~(1 << LED_PIN_0); break;
            case LED_OFF:   LED_PORT |= (1 << LED_PIN_0); break;
            case LED_TOGGLE: LED_PORT ^= (1 << LED_PIN_0); break;
        }
        break;

    case 1:
        switch (led_setting)
        {
            case LED_ON:    LED_PORT &= ~(1 << LED_PIN_1); break;
            case LED_OFF:   LED_PORT |= (1 << LED_PIN_1); break;
            case LED_TOGGLE: LED_PORT ^= (1 << LED_PIN_1); break;
        }
        break;

    case 2:
        switch (led_setting)
        {
            case LED_ON:    LED_PORT &= ~(1 << LED_PIN_2); break;
            case LED_OFF:   LED_PORT |= (1 << LED_PIN_2); break;
            case LED_TOGGLE: LED_PORT ^= (1 << LED_PIN_2); break;
        }
        break;

    }
}
void pal_button_init(void)
{
    BUTTON_PORT |= (1 << BUTTON_PIN_0);
    BUTTON_PORT_DIR &= ~(1 << BUTTON_PIN_0);
}
button_state_t pal_button_read(button_id_t button_no)
{
    button_no = button_no;

    if ((BUTTON_INPUT_PINS & (1 << BUTTON_PIN_0)) == 0x00)
    {
        return BUTTON_PRESSED;
    }
    else
    {
        return BUTTON_OFF;
    }
}
#endif
