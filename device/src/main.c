
#include <stdio.h>
#include <zephyr/kernel.h>

#define SLEEP_TIME_MS	1000UL

int main(void)
{
	while (1) 
	{
		printf("Test\r\n");
		k_msleep(SLEEP_TIME_MS);
	}
	return 0;
}
