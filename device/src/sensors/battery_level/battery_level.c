
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/zbus/zbus.h>

#include "sensors_hub.h"
#include "battery_level_private.h"

LOG_MODULE_REGISTER(battery_level, LOG_LEVEL_INF);

ZBUS_CHAN_DECLARE(sensors_hub_chan);

#define DEFAULT_MAX_UPDATE_RATE                 (1000UL)
#define DEFAULT_NOMINAL_VOLTAGE                 (3000L)
#define DEFAULT_ALARM_THRESHOLD_ACTIVATE        (2000L)
#define DEFAULT_ALARM_THRESHOLD_DEACTIVATE      (2200L)

typedef struct
{
    struct k_timer timer;
    uint32_t last_update_timestamp; 
    uint16_t max_update_rate_ms;
    int16_t nominal_voltage_mv;
    int16_t measured_voltage_mv;
    int16_t alarm_threshold_activate_mv;
    int16_t alarm_threshold_deactivate_mv;
    uint8_t is_enabled;
} battery_level_data_t;

static battery_level_data_t data = {
    .last_update_timestamp = 0LL,
    .max_update_rate_ms = DEFAULT_MAX_UPDATE_RATE,
    .nominal_voltage_mv = DEFAULT_NOMINAL_VOLTAGE,
    .measured_voltage_mv = 0L,
    .alarm_threshold_activate_mv = DEFAULT_ALARM_THRESHOLD_ACTIVATE,
    .alarm_threshold_deactivate_mv = DEFAULT_ALARM_THRESHOLD_DEACTIVATE
};

static void timer_expired_cb(struct k_timer *timer_id) {
    publish_voltage();
}

static void control_timer(void) {
    if (0 != data.is_enabled) {
        k_timer_stop(&data.timer);

        /*  duration = 0 --> lets update battery_level immediately
         */
        k_timer_start(&data.timer, K_MSEC(0), K_MSEC(data.max_update_rate_ms));
    } else {
        k_timer_stop(&data.timer);
    }
}

int init(void) {
    k_timer_init(&data.timer, timer_expired_cb, NULL);
    control_timer();
    return 0;
}

int set_voltage(int16_t voltage_mv) {
    if (voltage_mv < 0L) {
        voltage_mv = 0L;
    }
    data.measured_voltage_mv = voltage_mv;
    return 0;
}

int16_t get_voltage(void) {
    return data.measured_voltage_mv;
}

static int publish_data_to_channel(int16_t voltage_mv, uint8_t percent) {
    DEFINE_BATTERY_LEVEL_MSG(msg, voltage_mv, percent);
    return zbus_chan_pub(&sensors_hub_chan, &msg, K_SECONDS(1));
}

int publish_voltage(void) {
    uint8_t percent = ((data.measured_voltage_mv * 100L) / data.nominal_voltage_mv);
    return publish_data_to_channel(data.measured_voltage_mv, percent);
}

int set_update_rate(uint16_t update_rate_ms) {
    data.max_update_rate_ms = update_rate_ms;
    control_timer();
    return 0;
}

int16_t get_update_rate(void) {
    return data.max_update_rate_ms;
}

int set_nominal_voltage(int16_t nominal_voltage_mv) {
    if (nominal_voltage_mv < 0L) {
        return -EINVAL;
    }   
    data.nominal_voltage_mv = nominal_voltage_mv;
    return 0;
}

int16_t get_nominal_voltage(void) {
    return data.nominal_voltage_mv;
}

int set_alarm_thresholds(int16_t activate_mv, int16_t deactivate_mv) {
    if ((activate_mv < 0L) || (deactivate_mv < 0L) || (deactivate_mv < activate_mv)) {
        return -EINVAL;
    }
    data.alarm_threshold_activate_mv = activate_mv;
    data.alarm_threshold_deactivate_mv = deactivate_mv;
    return 0;
}

void set_enable(uint8_t is_enabled) {
    if (data.is_enabled != is_enabled) {
        data.is_enabled = is_enabled;
        control_timer();
    }
}

uint8_t is_enabled(void) {
    return data.is_enabled;
}

SYS_INIT_NAMED(battery_level, init, APPLICATION, 16);

