
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/settings/settings.h>

#include "utils_settings.h"
#include "mqtt_settings.h"

LOG_MODULE_REGISTER(mqtt_settings, LOG_LEVEL_INF);


static settings_info_t mqtt_settings_list[eMQTT_SETTING_ID_LAST] = {
    [eMQTT_SETTING_ID_BROKER_ADDRESS] = {
        .nvm_name = "broker_address",
        .storage_type = eSETTING_STORAGE_TYPE_RW,
        .dtype = eSETTING_DTYPE_UINT32,
        .value.uint32 = 0,
        .dvalue.uint32 = 0
    },
    [eMQTT_SETTING_ID_BROKER_PORT] = {
        .nvm_name = "broker_port",
        .storage_type = eSETTING_STORAGE_TYPE_RW,
        .dtype = eSETTING_DTYPE_UINT16,
        .value.uint16 = 1883,
        .dvalue.uint16 = 1883
    },
};

#define MQTT_SUBSYSTEM_KEY      "mqtt"
settings_handler_t mqtt_settings_handler = {
    .subsys_key = MQTT_SUBSYSTEM_KEY,
    .list = mqtt_settings_list,
    .len = eMQTT_SETTING_ID_LAST
};

static int load_cb(const char *name, size_t len, settings_read_cb read_cb, void *cb_arg) {
    return setting_set_handler(name, len, read_cb, cb_arg, &mqtt_settings_handler);
}

SETTINGS_STATIC_HANDLER_DEFINE(
    mqtt_settings_handler,
    MQTT_SUBSYSTEM_KEY,
    NULL,
    load_cb,
    NULL,
    NULL
);

static int init_early(void) {
    int rc = settings_load_subtree(MQTT_SUBSYSTEM_KEY);
    if (0 != rc) {
        LOG_ERR("Error while loading settings subsystem.");
        return rc;
    }
    return 0;
}

SYS_INIT_NAMED(system_early, init_early, APPLICATION, 12);