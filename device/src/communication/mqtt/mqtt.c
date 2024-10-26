
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

typedef struct
{
    const char* server_addr;
    uint16_t broker_port;
    uint8_t rx_buffer[DEFAULT_RX_BUFFER_LEN];
    uint8_t tx_buffer[DEFAULT_TX_BUFFER_LEN];
    struct mqtt_client client_ctx;
    struct sockaddr_storage broker;
    struct sockaddr broker_addr;
    struct pollfd fds;
} mqtt_data_t;

static mqtt_data_t data = {
    .broker_port = DEFAULT_BROKER_PORT,
    .server_addr = DEFAULT_SERVER_ADDR,
};

static int subscribe(void)
{
    const char* topic = "heartbeat";
	struct mqtt_topic subscribe_topic = {
		.topic = {
			.utf8 = topic,
			.size = strlen(topic)
		},
		.qos = MQTT_QOS_1_AT_LEAST_ONCE
	};

	const struct mqtt_subscription_list subscription_list = {
		.list = &subscribe_topic,
		.list_count = 1,
		.message_id = 1234
	};

	LOG_INF("Subscribing to: %s len %u", topic,
		(unsigned int)strlen(topic));

	return mqtt_subscribe(&data.client_ctx, &subscription_list);
}

static void mqtt_evt_handler(struct mqtt_client *client, const struct mqtt_evt *evt) {
    
    switch(evt->type) {
        /** Acknowledgment of connection request. Event result accompanying
         *  the event indicates whether the connection failed or succeeded.
         */
        case MQTT_EVT_CONNACK:
            LOG_INF("MQTT_EVT_CONNACK");
            subscribe();
            break;

        /** Disconnection Event. MQTT Client Reference is no longer valid once
         *  this event is received for the client.
         */
        case MQTT_EVT_DISCONNECT:
            LOG_INF("MQTT_EVT_DISCONNECT");
            break;

        /** Publish event received when message is published on a topic client
         *  is subscribed to.
         *
         * @note PUBLISH event structure only contains payload size, the payload
         *       data parameter should be ignored. Payload content has to be
         *       read manually with @ref mqtt_read_publish_payload function.
         */
        case MQTT_EVT_PUBLISH:
        {
            LOG_INF("MQTT_EVT_PUBLISH");

            // read all data
            uint8_t buffer[50];
            int data_read;
            LOG_INF("Received message: \n\t");
            do
            {
                data_read = mqtt_read_publish_payload_blocking(
                    &data.client_ctx,
                    buffer,
                    50
                );

                if (data_read > 0) {
                    for (size_t i = 0; i < data_read; i++) {
                        LOG_INF("0x%02x, ", buffer[i]);
                    }
                }
                LOG_INF("\n");
            } while (0 != data_read);
            
        }
            break;

        /** Acknowledgment for published message with QoS 1. */
        case MQTT_EVT_PUBACK:
            LOG_INF("MQTT_EVT_PUBACK");
            break;

        /** Reception confirmation for published message with QoS 2. */
        case MQTT_EVT_PUBREC:
            LOG_INF("MQTT_EVT_PUBREC");
            break;

        /** Release of published message with QoS 2. */
        case MQTT_EVT_PUBREL:
            LOG_INF("MQTT_EVT_PUBREL");
            break;

        /** Confirmation to a publish release message with QoS 2. */
        case MQTT_EVT_PUBCOMP:
            LOG_INF("MQTT_EVT_PUBCOMP");
            break;

        /** Acknowledgment to a subscribe request. */
        case MQTT_EVT_SUBACK:
            LOG_INF("MQTT_EVT_SUBACK");
            break;

        /** Acknowledgment to a unsubscribe request. */
        case MQTT_EVT_UNSUBACK:
            LOG_INF("MQTT_EVT_UNSUBACK");
            break;

        /** Ping Response from server. */
        case MQTT_EVT_PINGRESP:
            LOG_INF("MQTT_EVT_PINGRESP");
            break;

        default:
            LOG_WRN("Unknown MQTT event type: %d", evt->type);
            break;
    }

}

static int broker_init(void) {

    int err;
    struct addrinfo *result;
    struct addrinfo *addr;
    struct addrinfo hints = {
        .ai_family = AF_INET,       // IPv4
        .ai_socktype = SOCK_DGRAM
    };
    

    char port_str_buffer[10];
    snprintf(port_str_buffer, 10, "%d", data.broker_port);
    err = getaddrinfo(data.server_addr, port_str_buffer, &hints, &result);
    if (err) {
        LOG_ERR("getaddrinfo failed: %d", err);
        return -ECHILD;
    }

    addr = result;
    err = -EINVAL;
    while (NULL != addr) {
        if (sizeof(struct sockaddr_in) == addr->ai_addrlen) {

            struct sockaddr_in *broker4 = (struct sockaddr_in *)&data.broker;
            char ipv4_addr[NET_IPV4_ADDR_LEN];

			broker4->sin_addr.s_addr = ((struct sockaddr_in *)addr->ai_addr)->sin_addr.s_addr;
            broker4->sin_family = AF_INET;
            broker4->sin_port = htons(data.broker_port);

            inet_ntop(AF_INET, &broker4->sin_addr.s_addr, ipv4_addr, sizeof(ipv4_addr));

            LOG_INF("IPv4 Address found %s", (char *)(ipv4_addr));

            err = 0;
            break;
        }
        addr = addr->ai_next;
    }

    freeaddrinfo(addr);

    return err;
}

void mqtt_thread(void *arg1, void *arg2, void *arg3) {
    int err;
	while (1) {
		err = poll(&data.fds, 1, mqtt_keepalive_time_left(&data.client_ctx));
		if (err < 0) {
			LOG_ERR("Error in poll(): %d", errno);
			break;
		}

		err = mqtt_live(&data.client_ctx);
		if ((err != 0) && (err != -EAGAIN)) {
			LOG_ERR("Error in mqtt_live: %d", err);
			break;
		}

		if ((data.fds.revents & POLLIN) == POLLIN) {
			err = mqtt_input(&data.client_ctx);
			if (err != 0) {
				LOG_ERR("Error in mqtt_input: %d", err);
				break;
			}
		}

		if ((data.fds.revents & POLLERR) == POLLERR) {
			LOG_ERR("POLLERR");
			break;
		}

		if ((data.fds.revents & POLLNVAL) == POLLNVAL) {
			LOG_ERR("POLLNVAL");
			break;
		}
	}
}
struct k_thread mqtt_thread_data;
K_THREAD_DEFINE(mqtt_thread_id, 2048,
                mqtt_thread, NULL, NULL, NULL,
                7, 0, 0);

static int init(void) {

    mqtt_client_init(&data.client_ctx);
    broker_init();

    data.client_ctx.broker = &data.broker;
    data.client_ctx.evt_cb = mqtt_evt_handler;
    data.client_ctx.client_id.utf8 = (uint8_t *)DEFAULT_CLIENT_ID;
    data.client_ctx.client_id.size = sizeof(DEFAULT_CLIENT_ID) - 1;
    data.client_ctx.password = NULL;
    data.client_ctx.user_name = NULL;
    data.client_ctx.protocol_version = MQTT_VERSION_3_1_1;
    data.client_ctx.transport.type = MQTT_TRANSPORT_NON_SECURE;

    data.client_ctx.rx_buf = data.rx_buffer;
    data.client_ctx.rx_buf_size = DEFAULT_RX_BUFFER_LEN;
    data.client_ctx.tx_buf = data.tx_buffer;
    data.client_ctx.tx_buf_size = DEFAULT_TX_BUFFER_LEN;

    int rc = mqtt_connect(&data.client_ctx);
    if (0 != rc) {
        LOG_ERR("MQTT connect failed... (error %d)", rc);
    } else {
        LOG_INF("MQTT connected.");
	    data.fds.fd = data.client_ctx.transport.tcp.sock;
        data.fds.events = ZSOCK_POLLIN;
        k_thread_start(mqtt_thread_id);
    }
    return rc;
}

SYS_INIT_NAMED(mqtt, init, APPLICATION, 96);