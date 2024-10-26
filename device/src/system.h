#ifndef _SYSTEM_H_
#define _SYSTEM_H_

#include <stdint.h>

typedef enum
{
    eSM_STATE_INIT = 0,
    eSM_STATE_RUNNING = 1,
    eSM_STATE_TEST = 2,
    eSM_STATE_ERROR = 3,
    eSM_STATE_SHUTDOWN = 4,
    eSM_STATE_LAST = UINT16_MAX
} eSYSTEM_STATE_t;

typedef struct
{
    uint32_t sw_id;
    uint32_t sw_version;
    uint32_t hw_id;
    uint32_t hw_version;
    uint32_t serial_no;
} system_identification_msg_t;

typedef struct
{
    uint64_t up_time_ms;
    uint32_t error_reg;
    uint16_t system_state;
    uint16_t battery_level_mV;
} system_status_msg_t;

typedef enum
{
    eSYSTEM_MSG_STATUS = 0,
    eSYSTEM_MSG_IDENTIFICATION,
    eSYSTEM_MSG_LAST,
    eSYSTEM_MSG_ERROR = eSYSTEM_MSG_LAST
} eSYSTEM_MSG_t;

typedef struct
{
    eSYSTEM_MSG_t msg;
    union {
        system_status_msg_t status;
        system_identification_msg_t identification;
    } payload;
} system_msg_t;

const char* get_system_msg_name(eSYSTEM_MSG_t msg);


#endif /* _SYSTEM_H_ */