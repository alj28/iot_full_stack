#ifndef _BATTERY_LEVEL_H_
#define _BATTERY_LEVEL_H_

#include <stdint.h>

typedef struct 
{
    int16_t voltage_mv;
    uint8_t percent;
} battery_level_msg_t;



#endif /* _BATTERY_LEVEL_H_ */