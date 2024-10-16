
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/zbus/zbus.h>

#include "sensors_hub.h"

LOG_MODULE_REGISTER(sensors_hub, LOG_LEVEL_INF);

ZBUS_CHAN_DEFINE(
    sensors_hub_chan,       /**< channel name */
    sensor_hub_msg_t,       /**< channel type */
    NULL,                   /**< validator */
    NULL,                   /**< user data */
    ZBUS_OBSERVERS_EMPTY,   /**< observers list */
    ZBUS_MSG_INIT(.msg_type = eSENSOR_HUB_MSG_TYPE_ERROR)      /**< initial value */
);


static const char* msg_type_names[] = {
    [eSENSOR_HUB_MSG_TYPE_ERROR] = "Error",
#if defined(CONFIG_SENSOR_BATTERY_LEVEL_EN)
    [eSENSOR_HUB_MSG_TYPE_BATTERY_LEVEL] = "Battery level",
#endif 
};

static const char* get_msg_type_name(sensor_hub_msg_type_t msg_type) {
    return msg_type_names[msg_type];
}

