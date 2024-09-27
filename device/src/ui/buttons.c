
#include <zephyr/init.h>
#include <zephyr/device.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/drivers/gpio.h>

#include "buttons.h"

LOG_MODULE_REGISTER(ui_button, LOG_LEVEL_INF);

#define BUTTONS_READ_PERIOD         K_MSEC(250)

typedef enum
{
    BUTTON_1 = 0,
    BUTTON_2,
    BUTTON_3,
    BUTTON_4,
    BUTTON_LAST
} buttons_t;

typedef struct
{
    const struct gpio_dt_spec dt;
    void (*event_handler)(void);
    bool is_pressed;
} button_handler_t;


__weak void button_1_pressed(void) { LOG_INF("%s", __FUNCTION__); }
__weak void button_2_pressed(void) { LOG_INF("%s", __FUNCTION__); }
__weak void button_3_pressed(void) { LOG_INF("%s", __FUNCTION__); }
__weak void button_4_pressed(void) { LOG_INF("%s", __FUNCTION__); }


static button_handler_t buttons[BUTTON_LAST] = {
    [BUTTON_1] = {.dt = GPIO_DT_SPEC_GET_OR(DT_ALIAS(sw0), gpios, {0}), .event_handler = button_1_pressed, .is_pressed = false},
    [BUTTON_2] = {.dt = GPIO_DT_SPEC_GET_OR(DT_ALIAS(sw1), gpios, {0}), .event_handler = button_2_pressed, .is_pressed = false},
    [BUTTON_3] = {.dt = GPIO_DT_SPEC_GET_OR(DT_ALIAS(sw2), gpios, {0}), .event_handler = button_3_pressed, .is_pressed = false},
    [BUTTON_4] = {.dt = GPIO_DT_SPEC_GET_OR(DT_ALIAS(sw3), gpios, {0}), .event_handler = button_4_pressed, .is_pressed = false},
};
static struct k_timer timer;
static bool is_initialized = false;

static void run(struct k_timer *timer) {
    ARG_UNUSED(timer);
    for (int i = 0; i < ARRAY_SIZE(buttons); i++) {
        button_handler_t *p_hnd = &buttons[i];
        bool is_now_pressed = (1 == gpio_pin_get_dt(&p_hnd->dt));
        if (is_now_pressed && !p_hnd->is_pressed && (NULL != p_hnd->event_handler)) {
            p_hnd->event_handler();
        }
        p_hnd->is_pressed = is_now_pressed;
    }
}

void buttons_start(void) {
    if (is_initialized) {
        k_timer_start(&timer, BUTTONS_READ_PERIOD, BUTTONS_READ_PERIOD);
    }
}

void buttons_stop(void) {
    if (is_initialized) {
        k_timer_stop(&timer);
    }
}

static int init(void) {
    for (int i = 0; i < ARRAY_SIZE(buttons); i++) {
        if (!gpio_is_ready_dt(&buttons[i].dt)) {
            LOG_ERR("Button %d not ready.", (i+1));
            return -1;
        }

        if (0 != gpio_pin_configure_dt(&buttons[i].dt, (GPIO_INPUT | GPIO_PULL_UP | GPIO_ACTIVE_LOW))) {
            LOG_ERR("Configuration of %d button failed.", (i+1));
            return -1;
        }
    }
    k_timer_init(&timer, run, NULL);
    is_initialized = true;
    buttons_start();
    return 0;
}

SYS_INIT(init, APPLICATION, 16);