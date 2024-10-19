#ifndef _BATTERY_LEVEL_PRIVATE_H_
#define _BATTERY_LEVEL_PRIVATE_H_

#include <stdint.h>

int set_voltage(int16_t voltage_mv);
int16_t get_voltage(void);
int publish_voltage(void);
int set_update_rate(uint16_t update_rate_ms);
int16_t get_update_rate(void);
int set_nominal_voltage(int16_t nominal_voltage_mv);
int16_t get_nominal_voltage(void);
int set_alarm_thresholds(int16_t activate_mv, int16_t deactivate_mv);
void set_enable(uint8_t is_enabled);
uint8_t is_enabled(void);

#endif /* _BATTERY_LEVEL_PRIVATE_H_ */