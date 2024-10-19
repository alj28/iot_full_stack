
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/zbus/zbus.h>

#include "communication_hub.h"
#include "sensors_hub.h"

LOG_MODULE_REGISTER(communication_hub, LOG_LEVEL_INF);


//void communication_hub_sensor_hub_observer_cb(const struct zbus_channel *chan)
//{
//    // Access the data from the channel
//    const sensor_hub_msg_t *msg = zbus_chan_const_msg(chan);
//
//    // Process the message (for example, print the message type)
//    if (msg != NULL) {
//        printk("Received message with type: %d\n", msg->msg_type);
//    }
//}

ZBUS_MSG_SUBSCRIBER_DEFINE(communication_hub_server_hub_subscriber);
static void subscriber_task(void *ptr1, void *ptr2, void *ptr3)
{
    ARG_UNUSED(ptr1);
    ARG_UNUSED(ptr2);
    ARG_UNUSED(ptr3);
    const struct zbus_channel *chan;
    sensor_hub_msg_t msg;

    while (1) {
        if (!zbus_sub_wait_msg(&communication_hub_server_hub_subscriber, &chan, &msg, K_FOREVER)) {
            LOG_INF("Message arrived. Voltage: %d", msg.payload.battery_level.voltage_mv);
        }
        
    }
}
K_THREAD_DEFINE(communication_hub_server_hub_subscriber_task_id, 1024, subscriber_task, NULL, NULL, NULL, 3, 0, 0);

