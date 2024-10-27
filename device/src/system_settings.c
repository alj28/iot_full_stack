
#include <stdio.h>

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/zbus/zbus.h>
#include <zephyr/settings/settings.h>

#include "system.h"
#include "system_private.h"

LOG_MODULE_REGISTER(system_settings, LOG_LEVEL_INF);

#define SETTINGS_SUBSYSTEM_KEY          "system"

typedef enum 
{
    eSETTING_DTYPE_UINT64 = 0,
    eSETTING_DTYPE_INT64,
    eSETTING_DTYPE_FLOAT64,
    eSETTING_DTYPE_UINT32,
    eSETTING_DTYPE_INT32,
    eSETTING_DTYPE_FLOAT32,
    eSETTING_DTYPE_UINT16,
    eSETTING_DTYPE_INT16,
    eSETTING_DTYPE_UINT8,
    eSETTING_DTYPE_INT8,
    eSETTING_DTYPE_CHAR,
    eSETTING_DTYPE_LAST,
    eSETTING_DTYPE_ERROR = eSETTING_DTYPE_LAST
} eSETTING_DTYPE_t;

typedef enum
{
    eSETTING_STORAGE_TYPE_RW = 0,
    eSETTING_STORAGE_TYPE_RO,

    eSETTING_STORAGE_TYPE_LAST,
    eSETTING_STORAGE_TYPE_ERROR = eSETTING_STORAGE_TYPE_LAST
} eSETTING_STORAGE_TYPE_t;

typedef struct
{
    const char* const nvm_name;
    eSETTING_STORAGE_TYPE_t const storage_type;
    eSETTING_DTYPE_t const dtype;
    setting_value_t value;
    setting_value_t const dvalue;
} settings_info_t;

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

#define SETTING_DEFAULT_VALUE_SERIAL_NUMBER         (0x00000000UL)
#define SETTING_DEFAULT_VALUE_SW_ID                 (0x00000001UL)
#define SETTING_DEFAULT_VALUE_SW_VERSION            (0x00000001UL)
#define SETTING_DEFAULT_VALUE_HW_ID                 (0x00000001UL)
#define SETTING_DEFAULT_VALUE_HW_VERSION            (0x00000001UL)


static settings_info_t settings_list[eSYSTEM_SETTING_ID_LAST] = {
    [eSYSTEM_SETTING_ID_SERIAL_NUMBER] = {
        .nvm_name = "serial_number",
        .storage_type = eSETTING_STORAGE_TYPE_RW,
        .dtype = eSETTING_DTYPE_UINT32,
        .value.uint32 = SETTING_DEFAULT_VALUE_SERIAL_NUMBER,
        .dvalue.uint32 = SETTING_DEFAULT_VALUE_SERIAL_NUMBER,
    },
    [eSYSTEM_SETTING_ID_SW_ID] = {
        .nvm_name = "sw_id",
        .storage_type = eSETTING_STORAGE_TYPE_RO,
        .dtype = eSETTING_DTYPE_UINT32,
        .value.uint32 = SETTING_DEFAULT_VALUE_SW_ID,
        .dvalue.uint32 = SETTING_DEFAULT_VALUE_SW_ID,
    },
    [eSYSTEM_SETTING_ID_SW_VERSION] = {
        .nvm_name = "sw_version",
        .storage_type = eSETTING_STORAGE_TYPE_RO,
        .dtype = eSETTING_DTYPE_UINT32,
        .value.uint32 = SETTING_DEFAULT_VALUE_SW_VERSION,
        .dvalue.uint32 = SETTING_DEFAULT_VALUE_SW_VERSION,
    },
    [eSYSTEM_SETTING_ID_HW_ID] = {
        .nvm_name = "hw_id",
        .storage_type = eSETTING_STORAGE_TYPE_RW,
        .dtype = eSETTING_DTYPE_UINT32,
        .value.uint32 = SETTING_DEFAULT_VALUE_HW_ID,
        .dvalue.uint32 = SETTING_DEFAULT_VALUE_HW_ID,
    },
    [eSYSTEM_SETTING_ID_HW_VERSION] = {
        .nvm_name = "hw_version",
        .storage_type = eSETTING_STORAGE_TYPE_RW,
        .dtype = eSETTING_DTYPE_UINT32,
        .value.uint32 = SETTING_DEFAULT_VALUE_HW_VERSION,
        .dvalue.uint32 = SETTING_DEFAULT_VALUE_HW_VERSION,
    },
};

static int load_cb(const char *name, size_t len, settings_read_cb read_cb, void *cb_arg) {

    for (size_t i = 0; i < ARRAY_SIZE(settings_list); i++) {
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

int set_setting(eSYSTEM_SETTING_ID_t id, setting_value_t value) {
    settings_info_t* setting = &settings_list[id];
    if (eSETTING_STORAGE_TYPE_RO == setting->storage_type) {
        return -EINVAL;
    }
    const size_t key_buffer_len = 50;
    char key_buffer[key_buffer_len];
    size_t remaining_bytes = snprintf(key_buffer, key_buffer_len, "%s/%s", SETTINGS_SUBSYSTEM_KEY, setting->nvm_name);
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

setting_value_t get_setting(eSYSTEM_SETTING_ID_t id) {
    return settings_list[id].value;
}

setting_value_t get_setting_default(eSYSTEM_SETTING_ID_t id) {
    return settings_list[id].dvalue;
}

SETTINGS_STATIC_HANDLER_DEFINE(
    settings_handler,
    "system",
    NULL,
    load_cb,
    NULL,
    NULL
);


static int init_early(void) {
    int rc = settings_subsys_init();
    if (0 != rc) {
        LOG_ERR("Settings init error %d", rc);
        return rc;
    }
    rc = settings_load_subtree(SETTINGS_SUBSYSTEM_KEY);
    if (0 != rc) {
        LOG_ERR("Error while loading settings subsystem.");
        return rc;
    }
    return 0;
}

SYS_INIT_NAMED(system_early, init_early, APPLICATION, 10);