
#include "utils_settings.h"

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/zbus/zbus.h>
#include <zephyr/settings/settings.h>

LOG_MODULE_REGISTER(utils_settings, LOG_LEVEL_INF);

const static size_t dtype_len[eSETTING_DTYPE_LAST] = {
    [eSETTING_DTYPE_UINT64] = sizeof(uint64_t),
    [eSETTING_DTYPE_INT64] = sizeof(int64_t),
    [eSETTING_DTYPE_FLOAT64] = sizeof(double),
    [eSETTING_DTYPE_UINT32] = sizeof(uint32_t),
    [eSETTING_DTYPE_INT32] = sizeof(int32_t),
    [eSETTING_DTYPE_FLOAT32] = sizeof(float),
    [eSETTING_DTYPE_UINT16] = sizeof(uint16_t),
    [eSETTING_DTYPE_INT16] = sizeof(int16_t),
    [eSETTING_DTYPE_UINT8] = sizeof(uint8_t),
    [eSETTING_DTYPE_INT8] = sizeof(int8_t),
    [eSETTING_DTYPE_CHAR] = sizeof(char),
};

/*  This function needs to be encapsulated in the CB for a particular subsystem.
 */
int setting_set_handler(const char *name, size_t len, settings_read_cb read_cb, void *cb_arg, settings_handler_t* const handler) {
    if (NULL == handler) { return -EINVAL; }
    settings_info_t* const settings_list = handler->list;
    size_t list_len = handler->len;
    for (size_t i = 0; i < list_len; i++) {
        settings_info_t* setting = &settings_list[i];
        if (eSETTING_STORAGE_TYPE_RW != setting->storage_type) {
            continue;
        }
        const char *next;
        if (settings_name_steq(name, setting->nvm_name, &next) && !next) {
            if (len != dtype_len[setting->dtype]) {
                return -EINVAL;
            }
            int rc = read_cb(cb_arg, &setting->value, dtype_len[setting->dtype]);
            if (rc > 0) {
                rc = 0;
            }
            return rc;
        }
    }
    return -ENOENT;   
}

int setting_set(settings_handler_t* const handler, size_t id, setting_value_t value) {
    if (NULL == handler) {return -EINVAL;}
    if (id >= handler->len) {return -EINVAL;}
    settings_info_t* setting = &handler->list[id];
    if (eSETTING_STORAGE_TYPE_RO == setting->storage_type) {
        return -EINVAL;
    }
    const size_t key_buffer_len = 50;
    char key_buffer[key_buffer_len];
    size_t remaining_bytes = snprintf(key_buffer, key_buffer_len, "%s/%s", handler->subsys_key, setting->nvm_name);
    if (0 == remaining_bytes) {
        LOG_ERR("Buffer reserved for setting key too small.");
        return -EINVAL;
    }
    int rc = settings_save_one(key_buffer, &value, dtype_len[setting->dtype]);
    if (0 == rc) {
        setting->value = value;
    }
    return rc;
}

setting_value_t setting_get_value(settings_handler_t* const handler, size_t id) {
    if (NULL == handler) { return (setting_value_t){.uint8 = 0}; }
    if (id >= handler->len) { return (setting_value_t){.uint8 = 0}; }
    return handler->list[id].value;
}

setting_value_t setting_get_default_value(settings_handler_t* const handler, size_t id) {
    if (NULL == handler) { return (setting_value_t){.uint8 = 0}; }
    if (id >= handler->len) { return (setting_value_t){.uint8 = 0}; }
    return handler->list[id].dvalue;
}

static int init(void) {
    int rc = settings_subsys_init();
    if (0 != rc) {
        LOG_ERR("Settings init error %d", rc);
    }
    return rc;
}

SYS_INIT_NAMED(utils_settings, init, APPLICATION, 10);
