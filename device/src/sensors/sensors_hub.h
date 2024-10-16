#ifndef _SENSORS_HUB_H_
#define _SENSORS_HUB_H_

#include <stdint.h>

#define CONFIG_SENSOR_BATTERY_LEVEL_EN

#if defined(CONFIG_SENSOR_BATTERY_LEVEL_EN)
    #include "battery_level.h"
#endif

typedef enum
{
    eSENSOR_HUB_MSG_TYPE_ERROR = 0,
#if defined(CONFIG_SENSOR_BATTERY_LEVEL_EN)
    eSENSOR_HUB_MSG_TYPE_BATTERY_LEVEL,
#endif 
} sensor_hub_msg_type_t;

typedef struct
{
    sensor_hub_msg_type_t msg_type;
    union {
#if defined(CONFIG_SENSOR_BATTERY_LEVEL_EN)
        battery_level_msg_t battery_level;
#endif 
    } payload;
} sensor_hub_msg_t;

#if defined(CONFIG_SENSOR_BATTERY_LEVEL_EN)
#define DEFINE_BATTERY_LEVEL_MSG(_name, _voltage_mv, _percent)      \
    sensor_hub_msg_t _name = {                                      \
        .msg_type = eSENSOR_HUB_MSG_TYPE_BATTERY_LEVEL,             \
        .payload.battery_level.voltage_mv = _voltage_mv,            \
        .payload.battery_level.percent = _percent,                  \
    }
#endif


#endif /* _SENSORS_HUB_H_ */