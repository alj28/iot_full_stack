#ifndef _LEDS_H_
#define _LEDS_H_

typedef enum
{
    LED_1 = 0,
    LED_2,
    LED_3,
    LED_4,
    LED_LAST
} leds_t;

int led_set_state_on(leds_t led);
int led_set_state_off(leds_t led);
int led_set_state(leds_t led, bool state);
int led_set_state_blink(leds_t led, uint16_t duration_ms);



#endif /* _LEDS_H_ */