#ifndef _SYSTEM_PRIVATE_H_
#define _SYSTEM_PRIVATE_H_

#include <stdint.h>

#include "utils_settings.h"
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

extern settings_handler_t system_settings_handler;

int send_identification_msg(void);


#endif /* _SYSTEM_PRIVATE_H_ */