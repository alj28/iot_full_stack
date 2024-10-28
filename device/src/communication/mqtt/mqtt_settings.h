#ifndef _MQTT_SETTINGS_H_
#define _MQTT_SETTINGS_H_

#include "utils_settings.h"

typedef enum
{
    eMQTT_SETTING_ID_BROKER_ADDRESS = 0,
    eMQTT_SETTING_ID_BROKER_PORT,

    eMQTT_SETTING_ID_LAST,
    eMQTT_SETTING_ID_ERROR = eMQTT_SETTING_ID_LAST
} eMQTT_SETTING_ID_t;

extern settings_handler_t mqtt_settings_handler;

#endif /* _MQTT_SETTINGS_H_ */