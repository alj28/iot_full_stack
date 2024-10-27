#ifndef _SYSTEM_PRIVATE_H_
#define _SYSTEM_PRIVATE_H_

#include <stdint.h>

typedef enum
{
    eSYSTEM_SETTING_ID_SERIAL_NUMBER = 0,
    eSYSTEM_SETTING_ID_SW_ID,
    eSYSTEM_SETTING_ID_SW_VERSION,
    eSYSTEM_SETTING_ID_HW_ID,
    eSYSTEM_SETTING_ID_HW_VERSION,

    eSYSTEM_SETTING_ID_LAST,
    eSYSTEM_SETTING_ID_ERROR = eSYSTEM_SETTING_ID_LAST
}eSYSTEM_SETTING_ID_t;

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

int send_identification_msg(void);

int set_setting(eSYSTEM_SETTING_ID_t id, setting_value_t value);
setting_value_t get_setting(eSYSTEM_SETTING_ID_t id);
setting_value_t get_setting_default(eSYSTEM_SETTING_ID_t id);

#endif /* _SYSTEM_PRIVATE_H_ */