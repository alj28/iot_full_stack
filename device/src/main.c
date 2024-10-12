
#include <stdio.h>

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#include "board_utils.h"


#define SLEEP_TIME_MS	100UL

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);


int main(void)
{
	LOG_INF("Application image boots up.");
	//LOG_INF("Address of sample %p\n", (void *)__rom_region_start);



	while (1) 
	{
		//printf("Test\r\n");

		k_msleep(SLEEP_TIME_MS);
		POSIX_SPECIFIC_SECTION({
			k_cpu_idle();
		})
	}
	return 0;
}
