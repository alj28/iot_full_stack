#ifndef _MQTT_H_
#define _MQTT_H_

#include <stdio.h>
#include <stdint.h>

typedef enum
{
    eMQTT_TOPIC_IDENTIFICATION = 0,
    eMQTT_TOPIC_STATUS,
    eMQTT_TOPIC_LAST,
    eMQTT_TOPIC_ERROR = eMQTT_TOPIC_LAST
} eMQTT_TOPIC_t;

int publish_data(eMQTT_TOPIC_t topic, uint8_t* payload, size_t size);

#endif /* _MQTT_H_ */