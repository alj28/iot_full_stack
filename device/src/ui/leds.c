
#include <zephyr/init.h>
#include <zephyr/device.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/drivers/gpio.h>

#include "leds.h"

LOG_MODULE_REGISTER(ui_led, LOG_LEVEL_INF);

typedef struct 
{
    struct gpio_dt_spec dt;
    struct k_timer timer;
} led_handler_t;

static led_handler_t leds[LED_LAST] = {
    [LED_1] = {.dt = GPIO_DT_SPEC_GET_OR(DT_ALIAS(led0), gpios, {0})},
    [LED_2] = {.dt = GPIO_DT_SPEC_GET_OR(DT_ALIAS(led1), gpios, {0})},
    [LED_3] = {.dt = GPIO_DT_SPEC_GET_OR(DT_ALIAS(led2), gpios, {0})},
    [LED_4] = {.dt = GPIO_DT_SPEC_GET_OR(DT_ALIAS(led3), gpios, {0})}
};



int led_set_state_on(leds_t led) {
    led_handler_t *handler = &leds[led];
    k_timer_stop(&handler->timer);
    if (0 != gpio_pin_set_dt(&handler->dt, 1)) {
        LOG_ERR("Cannot change state to \"ON\" of LED %d", ((int)led + 1));
        return -1;
    }
    return 0;
}

int led_set_state_off(leds_t led) {
    led_handler_t *handler = &leds[led];
    k_timer_stop(&handler->timer);
    if (0 != gpio_pin_set_dt(&handler->dt, 0)) {
        LOG_ERR("Cannot change state to \"ON\" of LED %d", ((int)led + 1));
        return -1;
    }
    return 0;
}

int led_set_state(leds_t led, bool state) {
    if (state) {
        return led_set_state_on(led);
    } else {
        return led_set_state_off(led);
    }
}

int led_set_state_blink(leds_t led, uint16_t duration_ms) {
    struct k_timer *timer = &leds[led].timer;
    k_timer_start(timer, K_MSEC(duration_ms), K_MSEC(duration_ms));
    return 0;
}

static void timer_expired(struct k_timer *timer) {
    const led_handler_t* const handler = (const led_handler_t*)k_timer_user_data_get(timer);
    if (NULL == handler) {
        return;
    }

    const struct gpio_dt_spec* const dt = &handler->dt;
    gpio_pin_toggle_dt(dt);
}

static int init(void) {
    for (size_t i = 0; i < ARRAY_SIZE(leds); i++) {
        struct gpio_dt_spec *dt = &leds[i].dt;

        if (!gpio_is_ready_dt(dt)) {
            LOG_ERR("LED %d not ready.", (i+1));
            return -1;
        }

        if (0 != gpio_pin_configure_dt(dt, (GPIO_OUTPUT | GPIO_ACTIVE_LOW))) {
            LOG_ERR("Configuration of %d LED failed.", (i+1));
            return -1;
        }

        k_timer_init(&leds[i].timer, timer_expired, NULL);
        k_timer_user_data_set(&leds[i].timer, (void*)&leds[i]);

        led_set_state_off((leds_t)i);
    }
    return 0;
}

SYS_INIT_NAMED(led, init, APPLICATION, 14);