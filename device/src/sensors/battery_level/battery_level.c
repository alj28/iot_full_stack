
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/zbus/zbus.h>

#include "sensors_hub.h"
#include "battery_level_private.h"

LOG_MODULE_REGISTER(battery_level, LOG_LEVEL_INF);

ZBUS_CHAN_DECLARE(sensors_hub_chan);


int publish_data_to_channel(int16_t voltage_mv, uint8_t percent) {
    DEFINE_BATTERY_LEVEL_MSG(msg, voltage_mv, percent);
    return zbus_chan_pub(&sensors_hub_chan, &msg, K_SECONDS(1));
}

