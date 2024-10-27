
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/zbus/zbus.h>
#include <zephyr/settings/settings.h>

#include "system.h"
#include "system_private.h"

LOG_MODULE_REGISTER(system_settings, LOG_LEVEL_INF);

typedef enum
{
    eSETTINGS_KEY_SERIAL_NUMBER = 0,
    eSETTINGS_KEY_LAST,
    eSETTINGS_KEY_ERROR = eSETTINGS_KEY_LAST
}eSETTINGS_KEY_t;

typedef struct
{
    const char* name;
    size_t size;
} settings_info_t;

static const settings_info_t settings_info[eSETTINGS_KEY_LAST] = {
    [eSETTINGS_KEY_SERIAL_NUMBER] = {.name = "serial_number", .size = sizeof(uint32_t)}
};

uint32_t serial_number_value = 0UL;
static int serial_number_set_handle(const char *name, size_t len, settings_read_cb read_cb, void *cb_arg) {
    return read_cb(cb_arg, &serial_number_value, sizeof(serial_number_value));
}
static struct settings_handler serial_number = {
    .name = settings_info[eSETTINGS_KEY_SERIAL_NUMBER].name,
    .h_set = serial_number_set_handle
};
int set_serial_number(uint32_t value) {
    int rc = settings_save_one(settings_info[eSETTINGS_KEY_SERIAL_NUMBER].name, (void*)value, settings_info[eSETTINGS_KEY_SERIAL_NUMBER].size);
    if (0 == rc) {
        serial_number_value = value;
    }
    return rc;
}

uint32_t get_serial_number(void) {
    return serial_number_value;
}


static int init_early(void) {
    int rc = settings_subsys_init();
    if (0 != rc) {
        LOG_ERR("Settings init error %d", rc);
        return rc;
    }
    rc = settings_register(&serial_number);
    if (0 != rc) {
        LOG_ERR("Settings register error %d", rc);
        return rc;
    }
    rc = settings_load();
    if (0 != rc) {
        LOG_ERR("Settings load error %d", rc);
        return rc;
    }
    return 0;
}

SYS_INIT_NAMED(system_early, init_early, APPLICATION, 10);