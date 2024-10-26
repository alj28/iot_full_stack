
#include <stdio.h>

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/net/net_ip.h>
#include <zephyr/net/socket.h>


// https://docs.zephyrproject.org/latest/connectivity/networking/api/mqtt.html

LOG_MODULE_REGISTER(mqtt, LOG_LEVEL_INF);

#define DEFAULT_SERVER_ADDR     ("192.168.0.105")
#define DEFAULT_BROKER_PORT     (1883UL)
#define DEFAULT_RX_BUFFER_LEN   (256)
#define DEFAULT_TX_BUFFER_LEN   (256)
#define DEFAULT_CLIENT_ID       ("zephyr_mqtt_client")




static int init(void) {
    LOG_INF("Init");
}

SYS_INIT_NAMED(mqtt, init, APPLICATION, 96);