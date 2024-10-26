
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/zbus/zbus.h>
#include <cJSON.h>

#include "communication_hub.h"
#include "sensors_hub.h"

#include "mqtt.h"
#include "system.h"

LOG_MODULE_REGISTER(communication_hub, LOG_LEVEL_INF);

typedef void (*field_setter_t)(cJSON * const object, const char * const name, const uint8_t* payload, size_t offset);
typedef struct
{
    const uint8_t* const name;
    size_t offset;
    field_setter_t field_setter;
} json_field_t;

static void cJSON_AddNumberToObject_uint64_hex(cJSON * const object, const char * const name, const uint8_t* payload, size_t offset) {
    uint64_t const value = *(uint64_t *)((payload + offset));
    char value_str[22];
    snprintf(value_str, 22, "0x%016llx", value);
    cJSON_AddStringToObject(object, name, value_str);
}

static void cJSON_AddNumberToObject_uint64(cJSON * const object, const char * const name, const uint8_t* payload, size_t offset) {
    uint64_t const value = *(uint64_t *)((payload + offset));
    cJSON_AddNumberToObject(object, name, value);
}

static void cJSON_AddNumberToObject_uint32_hex(cJSON * const object, const char * const name, const uint8_t* payload, size_t offset) {
    uint32_t const value = *(uint32_t *)((payload + offset));
    char value_str[20];
    snprintf(value_str, 20, "0x%08x", value);
    cJSON_AddStringToObject(object, name, value_str);
}

static void cJSON_AddNumberToObject_uint32(cJSON * const object, const char * const name, const uint8_t* payload, size_t offset) {
    uint32_t const value = *(uint32_t *)((payload + offset));
    cJSON_AddNumberToObject(object, name, value);
}

static void cJSON_AddNumberToObject_uint16_hex(cJSON * const object, const char * const name, const uint8_t* payload, size_t offset) {
    uint16_t const value = *(uint16_t *)((payload + offset));
    char value_str[20];
    snprintf(value_str, 20, "0x%04x", value);
    cJSON_AddStringToObject(object, name, value_str);
}

static void cJSON_AddNumberToObject_uint16(cJSON * const object, const char * const name, const uint8_t* payload, size_t offset) {
    uint16_t const value = *(uint16_t *)((payload + offset));
    cJSON_AddNumberToObject(object, name, value);
}

static void prepare_json_payload(cJSON* const object, const json_field_t* const fields, size_t len, const uint8_t* const payload) {
    for (size_t i = 0; i < len; i++) {
        fields[i].field_setter(object, fields[i].name, payload, fields[i].offset);
    }
}

static const json_field_t system_identification_fields[] = {
    {.name = "sw_id", .offset = offsetof(system_identification_msg_t, sw_id), cJSON_AddNumberToObject_uint32_hex},
    {.name = "sw_version", .offset = offsetof(system_identification_msg_t, sw_version), cJSON_AddNumberToObject_uint32_hex},
    {.name = "hw_id", .offset = offsetof(system_identification_msg_t, hw_id), cJSON_AddNumberToObject_uint32_hex},
    {.name = "hw_version", .offset = offsetof(system_identification_msg_t, hw_version), cJSON_AddNumberToObject_uint32_hex},
    {.name = "serial_no", .offset = offsetof(system_identification_msg_t, serial_no), cJSON_AddNumberToObject_uint32_hex},
};

static const json_field_t system_status_fields[] = {
    {.name = "up_time_ms", .offset = offsetof(system_status_msg_t, up_time_ms), cJSON_AddNumberToObject_uint64},
    {.name = "error_reg", .offset = offsetof(system_status_msg_t, error_reg), cJSON_AddNumberToObject_uint32_hex},
    {.name = "system_state", .offset = offsetof(system_status_msg_t, system_state), cJSON_AddNumberToObject_uint16},
    {.name = "battery_level_mV", .offset = offsetof(system_status_msg_t, battery_level_mV), cJSON_AddNumberToObject_uint16},
};

static void publish(eMQTT_TOPIC_t topic, const json_field_t* const fields, size_t len, const uint8_t* const payload) {
    cJSON *root = cJSON_CreateObject();
    prepare_json_payload(root, fields, len, (void*)payload);
    char* json_string = cJSON_Print(root);
    publish_data(topic, json_string, strlen(json_string));
    cJSON_Delete(root);
    cJSON_free(json_string);
}

static void publish_system_msg(const system_msg_t* const msg) {
    const json_field_t* fields = NULL;
    size_t len;
    eMQTT_TOPIC_t topic;
    const uint8_t* payload = (uint8_t*)&msg->payload;

    switch (msg->msg) {
        case eSYSTEM_MSG_STATUS:
            fields = system_status_fields;
            len = ARRAY_SIZE(system_status_fields);
            topic = eMQTT_TOPIC_IDENTIFICATION;
            break;
        case eSYSTEM_MSG_IDENTIFICATION:
            fields = system_identification_fields;
            len = ARRAY_SIZE(system_identification_fields);
            topic = eMQTT_TOPIC_IDENTIFICATION;
            break;
        default:
            LOG_ERR("Unknown system message.");
            return;
    }

    publish(topic, fields, len, payload);
}


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


void system_listener(const struct zbus_channel *chan)
{
    
    const system_msg_t* const msg = zbus_chan_const_msg(chan);
    if (NULL != msg) {
        publish_system_msg(msg);
    }
}
ZBUS_LISTENER_DEFINE(communication_hub_system_subscriber, system_listener);
