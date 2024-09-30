
#include <zboss_api.h>
#include <zb_mem_config_max.h>
#include <zb_nrf_platform.h>
#include <zboss_api_addons.h>
#include <zephyr/logging/log.h>
#include <zigbee/zigbee_app_utils.h>
#include <zigbee/zigbee_error_handler.h>
#include <zcl/zb_zcl_temp_measurement_addons.h>
#include <zb_ha_device_config.h>
#include <zephyr/kernel.h>

#include "zb_temperature_sensor.h"
#include "zigbee.h"
#include "leds.h"

LOG_MODULE_REGISTER(zigbee, LOG_LEVEL_INF);

#define TEMPERATURE_MEASUREMENT_ENDPOINT        1U
#define APP_ZIGBEE_OBSERVE_TEMP_INTERVAL        (ZB_TIME_ONE_SECOND * 30)

/*  Main application's context.
 */
typedef struct {
    zb_zcl_basic_attrs_ext_t basic_attr;
    zb_zcl_identify_attrs_t identify_attr;
    zb_zcl_on_off_attrs_t on_off_attr;
    zb_zcl_temp_measurement_attrs_t temperature_measurement_attr;
} temperature_sensor_ctx_t;
static temperature_sensor_ctx_t dev_ctx;

ZB_ZCL_DECLARE_IDENTIFY_ATTRIB_LIST(
    identify_attr_list,
    &dev_ctx.identify_attr.identify_time
);

ZB_ZCL_DECLARE_BASIC_ATTRIB_LIST_EXT(
	basic_attr_list,
	&dev_ctx.basic_attr.zcl_version,
	&dev_ctx.basic_attr.app_version,
	&dev_ctx.basic_attr.stack_version,
	&dev_ctx.basic_attr.hw_version,
	dev_ctx.basic_attr.mf_name,
	dev_ctx.basic_attr.model_id,
	dev_ctx.basic_attr.date_code,
	&dev_ctx.basic_attr.power_source,
	dev_ctx.basic_attr.location_id,
	&dev_ctx.basic_attr.ph_env,
	dev_ctx.basic_attr.sw_ver
);

ZB_ZCL_DECLARE_ON_OFF_ATTRIB_LIST(
	on_off_attr_list,
	&dev_ctx.on_off_attr.on_off
);

ZB_ZCL_DECLARE_TEMP_MEASUREMENT_ATTRIB_LIST(
    temperature_measurement_attr_list,
    &dev_ctx.temperature_measurement_attr.measure_value,
    &dev_ctx.temperature_measurement_attr.min_measure_value,
    &dev_ctx.temperature_measurement_attr.max_measure_value,
    &dev_ctx.temperature_measurement_attr.tolerance
);

ZB_DECLARE_TEMPERATURE_SENSOR_CLUSTER_LIST(
    temperature_sensor_clusters,
    basic_attr_list,
    identify_attr_list,
    temperature_measurement_attr_list
);

ZB_DECLARE_TEMPERATURE_SENSOR_EP(
    temperature_sensor_ep,
    TEMPERATURE_MEASUREMENT_ENDPOINT,
    temperature_sensor_clusters
);

ZBOSS_DECLARE_DEVICE_CTX_1_EP(
    temperature_sensor_ctx,
    temperature_sensor_ep
);


static void clusters_attr_init(void) {
    dev_ctx.basic_attr.zcl_version = ZB_ZCL_VERSION;
    dev_ctx.basic_attr.app_version = 1;
    dev_ctx.basic_attr.stack_version = 10;
    dev_ctx.basic_attr.hw_version = 11;

    ZB_ZCL_SET_STRING_VAL(
        dev_ctx.basic_attr.mf_name,
        "Demo",
        ZB_ZCL_STRING_CONST_SIZE("Demo")
    );

    ZB_ZCL_SET_STRING_VAL(
        dev_ctx.basic_attr.model_id,
        "DemoTempSensor",
        ZB_ZCL_STRING_CONST_SIZE("DemoTempSensor")
    );

    ZB_ZCL_SET_STRING_VAL(
        dev_ctx.basic_attr.date_code,
        "20240930",
        ZB_ZCL_STRING_CONST_SIZE("20240930")
    );

    dev_ctx.basic_attr.power_source = ZB_ZCL_BASIC_POWER_SOURCE_BATTERY;

    ZB_ZCL_SET_STRING_VAL(
        dev_ctx.basic_attr.location_id,
        "DemoLocation",
        ZB_ZCL_STRING_CONST_SIZE("DemoLocation")
    );

    dev_ctx.basic_attr.ph_env = ZB_ZCL_BASIC_ENV_UNSPECIFIED;

    ZB_ZCL_SET_STRING_VAL(
        dev_ctx.basic_attr.sw_ver,
        "v1.0.0",
        ZB_ZCL_STRING_CONST_SIZE("v1.0.0")
    );

    dev_ctx.identify_attr.identify_time = ZB_ZCL_IDENTIFY_IDENTIFY_TIME_DEFAULT_VALUE;

    dev_ctx.on_off_attr.on_off = (zb_bool_t)ZB_ZCL_ON_OFF_IS_ON;

    dev_ctx.temperature_measurement_attr.max_measure_value = INT16_MAX;
    dev_ctx.temperature_measurement_attr.measure_value = 0L;
    dev_ctx.temperature_measurement_attr.min_measure_value = -INT16_MAX;
    dev_ctx.temperature_measurement_attr.tolerance = 10L;


    // initialization of the attribute value in Zigbee stack
    ZB_ZCL_SET_ATTRIBUTE(
        TEMPERATURE_MEASUREMENT_ENDPOINT,
        ZB_ZCL_CLUSTER_ID_ON_OFF,
        ZB_ZCL_CLUSTER_SERVER_ROLE,
        ZB_ZCL_ATTR_ON_OFF_ON_OFF_ID,
        (zb_uint8_t*)&dev_ctx.on_off_attr.on_off,
        ZB_FALSE
    );

    ZB_ZCL_SET_ATTRIBUTE(
        TEMPERATURE_MEASUREMENT_ENDPOINT,
        ZB_ZCL_CLUSTER_ID_TEMP_MEASUREMENT,
        ZB_ZCL_CLUSTER_SERVER_ROLE,
        ZB_ZCL_ATTR_TEMP_MEASUREMENT_VALUE_ID,
        (zb_uint8_t*)&dev_ctx.temperature_measurement_attr.measure_value,
        ZB_FALSE
    );
}


static void identify_cb(zb_bufid_t bufid) {
    LOG_INF("Identify CB");
}



void zboss_signal_handler(zb_bufid_t bufid) {
	zigbee_default_signal_handler(bufid);
    if (bufid) { zb_buf_free(bufid); }
}

static void temperature_measurement_set_value(zb_int16_t measurement) {
    ZB_ZCL_SET_ATTRIBUTE(
        TEMPERATURE_MEASUREMENT_ENDPOINT,
        ZB_ZCL_CLUSTER_ID_TEMP_MEASUREMENT,
        ZB_ZCL_CLUSTER_SERVER_ROLE,
        ZB_ZCL_ATTR_TEMP_MEASUREMENT_VALUE_ID,
        (zb_uint8_t*)&measurement,
        ZB_FALSE
    );
}

static void temperature_update_task(struct k_timer *timer) {
    static zb_int16_t temp = 0;
    temp = (temp + 1L) % 80;
    temperature_measurement_set_value((temp - 40) * 100);
} 


struct k_timer timer;
static int init(void) {
    LOG_INF("Starting Zigbee thread.");
    clusters_attr_init();
    ZB_AF_REGISTER_DEVICE_CTX(&temperature_sensor_ctx);
    ZB_AF_SET_IDENTIFY_NOTIFICATION_HANDLER(TEMPERATURE_MEASUREMENT_ENDPOINT, identify_cb);
    zigbee_enable();

    k_timer_init(&timer, temperature_update_task, NULL);
    k_timer_start(&timer, K_MSEC(500), K_MSEC(500));

    return 0;
}

SYS_INIT_NAMED(zigbee, init, APPLICATION, 15);