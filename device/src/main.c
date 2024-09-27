
#include <stdio.h>
#include <zboss_api.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zigbee/zigbee_app_utils.h>
#include <zephyr/linker/linker-defs.h>
#include <zigbee/zigbee_error_handler.h>

#include "buttons.h"

#define SLEEP_TIME_MS	100UL

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

void zboss_signal_handler(zb_bufid_t bufid)
{
	zigbee_default_signal_handler(bufid);
}

int main(void)
{
	LOG_INF("Application image boots up.");
	LOG_INF("Address of sample %p\n", (void *)__rom_region_start);



	while (1) 
	{
		//printf("Test\r\n");

		k_msleep(SLEEP_TIME_MS);
	}
	return 0;
}
