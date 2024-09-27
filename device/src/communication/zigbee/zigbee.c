
#include <zboss_api.h>
#include <zb_nrf_platform.h>
#include <zboss_api_addons.h>
#include <zephyr/logging/log.h>
#include <zigbee/zigbee_app_utils.h>
#include <zigbee/zigbee_error_handler.h>

#include "zigbee.h"
#include "leds.h"


LOG_MODULE_REGISTER(zigbee, LOG_LEVEL_INF);

void zboss_signal_handler(zb_bufid_t bufid)
{
	zigbee_default_signal_handler(bufid);
}


static int init(void) {
    LOG_INF("Starting Zigbee thread.");

    /*  Enabling zigbee will automatically start network steering.
     */
    zigbee_enable();

    led_set_state_blink(LED_1, 500);
    return 0;
}

SYS_INIT_NAMED(zigbee, init, APPLICATION, 15);