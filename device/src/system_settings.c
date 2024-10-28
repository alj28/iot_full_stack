
#include <stdio.h>

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/zbus/zbus.h>
#include <zephyr/settings/settings.h>

#include "system.h"
#include "system_private.h"
#include "utils_settings.h"

LOG_MODULE_REGISTER(system_settings, LOG_LEVEL_INF);

#define SETTINGS_SUBSYSTEM_KEY          "system"


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

settings_handler_t system_settings_handler = {
    .subsys_key = SETTINGS_SUBSYSTEM_KEY,
    .list = settings_list,
    .len = eSYSTEM_SETTING_ID_LAST
};

static int load_cb(const char *name, size_t len, settings_read_cb read_cb, void *cb_arg) {
    return setting_set_handler(name, len, read_cb, cb_arg, &system_settings_handler);
}


SETTINGS_STATIC_HANDLER_DEFINE(
    system_settings_handler,
    SETTINGS_SUBSYSTEM_KEY,
    NULL,
    load_cb,
    NULL,
    NULL
);


static int init_early(void) {
    int rc = settings_load_subtree(SETTINGS_SUBSYSTEM_KEY);
    if (0 != rc) {
        LOG_ERR("Error while loading settings subsystem.");
        return rc;
    }
    return 0;
}

SYS_INIT_NAMED(system_early, init_early, APPLICATION, 11);