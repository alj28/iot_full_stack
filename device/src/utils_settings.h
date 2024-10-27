#ifndef _UTILS_SETTINGS_H_
#define _UTILS_SETTINGS_H_

#include <stdio.h>
#include <stdint.h>

#include <zephyr/settings/settings.h>

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

typedef union 
{
    uint64_t uint64;
    int64_t int64;
    double float64;
    uint32_t uint32;
    int32_t int32;
    float float32;
    uint16_t uint16;
    int16_t int16;
    uint8_t uint8;
    int8_t int8;
    char char_val;
} setting_value_t;

typedef struct
{
    const char* const nvm_name;
    eSETTING_STORAGE_TYPE_t const storage_type;
    eSETTING_DTYPE_t const dtype;
    setting_value_t value;
    setting_value_t const dvalue;
} settings_info_t;

typedef struct
{
    const char* const subsys_key;
    settings_info_t* const list;
    size_t len;
} settings_handler_t;

int setting_set_handler(const char *name, size_t len, settings_read_cb read_cb, void *cb_arg, settings_handler_t* const handler);
int setting_set(settings_handler_t* const handler, size_t id, setting_value_t value);
setting_value_t setting_get_value(settings_handler_t* const handler, size_t id);
setting_value_t setting_get_default_value(settings_handler_t* const handler, size_t id);

#endif  /* _UTILS_SETTINGS_H_ */