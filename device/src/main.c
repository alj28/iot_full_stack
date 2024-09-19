
#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/linker/linker-defs.h>

#define SLEEP_TIME_MS	1000UL

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

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
