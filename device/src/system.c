
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/zbus/zbus.h>

#include "system.h"
#include "system_private.h"

LOG_MODULE_REGISTER(system, LOG_LEVEL_INF);

#define DEFAULT_SW_ID           0x00000001          //  ID 1
#define DEFAULT_SW_VERSION      0x01000101          //  version 1.1.1
#define DEFAULT_HW_ID           0x00000001          //  board ID 1
#define DEFAULT_HW_VERSION      0x01000000          //  version 1.0.0
#define DEFAULT_SERIAL_NUMBER   0x12345678          //  default serial number

#define DEFAULT_ERROR_REG       0x00000000

static const system_identification_msg_t id_msg = {
    .sw_id = DEFAULT_SW_ID,
    .sw_version = DEFAULT_SW_VERSION,
    .hw_id = DEFAULT_HW_ID,
    .hw_version = DEFAULT_HW_VERSION,
    .serial_no = DEFAULT_SERIAL_NUMBER
};

static const char* const msg_names[eSYSTEM_MSG_LAST] = {
    [eSYSTEM_MSG_STATUS] = "status",
    [eSYSTEM_MSG_IDENTIFICATION] = "identification"
};

const char* get_system_msg_name(eSYSTEM_MSG_t msg) {
    if (eSYSTEM_MSG_ERROR != msg) {
        return msg_names[msg];
    }
    return NULL;
}

ZBUS_CHAN_DEFINE(
    system_chan,
    system_msg_t,
    NULL,
    NULL,
    ZBUS_OBSERVERS(communication_hub_system_subscriber),
    ZBUS_MSG_INIT(.msg = eSYSTEM_MSG_LAST)
)

static int publish_to_system_channel(const system_msg_t* msg) {
    return zbus_chan_pub(&system_chan, (void*)msg, K_SECONDS(1));
}

int send_identification_msg(void) {
    system_msg_t msg = {.msg = eSYSTEM_MSG_IDENTIFICATION};
    memcpy((void*)&msg.payload.identification, (void*)&id_msg, sizeof(system_identification_msg_t));
    return publish_to_system_channel(&msg);
}

static int init_early(void) {
    return 0;
}

static int init_very_late(void) {
    return send_identification_msg();
}

SYS_INIT_NAMED(system_early, init_early, APPLICATION, 10);
SYS_INIT_NAMED(system_very_late, init_very_late, APPLICATION, 90);