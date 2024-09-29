
#include <zboss_api.h>
#include <zb_mem_config_max.h>
#include <zb_nrf_platform.h>
#include <zboss_api_addons.h>
#include <zephyr/logging/log.h>
#include <zigbee/zigbee_app_utils.h>
#include <zigbee/zigbee_error_handler.h>
#include <zcl/zb_zcl_temp_measurement_addons.h>
#include <zb_ha_device_config.h>

#include "zigbee.h"
#include "leds.h"

LOG_MODULE_REGISTER(zigbee, LOG_LEVEL_INF);

#define ZIGBEE_ENDPOINT                     10
#define APP_ZIGBEE_OBSERVE_TEMP_INTERVAL    (ZB_TIME_ONE_SECOND * 30)

#define FACTORY_RESET_BUTTON       DK_BTN4_MSK

/* Main application customizable context.
 * Stores all settings and static values.
 */
struct zb_device_ctx {
	zb_zcl_basic_attrs_t basic_attr;
	zb_zcl_identify_attrs_t identify_attr;
    zb_zcl_temp_measurement_attrs_t temp_attrs;
};
static struct zb_device_ctx dev_ctx = {
    .basic_attr.power_source = ZB_ZCL_BASIC_POWER_SOURCE_BATTERY,
    .basic_attr.zcl_version = ZB_ZCL_VERSION,
    .identify_attr.identify_time = ZB_ZCL_IDENTIFY_IDENTIFY_TIME_DEFAULT_VALUE,
	.temp_attrs.measure_value = ZB_ZCL_ATTR_TEMP_MEASUREMENT_VALUE_UNKNOWN,
	.temp_attrs.min_measure_value = (-50 * 100),
	.temp_attrs.max_measure_value = (150 * 100),
	.temp_attrs.tolerance = (1)
};

ZB_ZCL_DECLARE_IDENTIFY_ATTRIB_LIST(
	identify_attr_list,
	&dev_ctx.identify_attr.identify_time
);

ZB_ZCL_DECLARE_BASIC_ATTRIB_LIST(
	basic_attr_list,
	&dev_ctx.basic_attr.zcl_version,
	&dev_ctx.basic_attr.power_source
);

ZB_ZCL_DECLARE_TEMP_MEASUREMENT_ATTRIB_LIST(
	temperature_measurement_attr_list,
	&dev_ctx.temp_attrs.measure_value,
	&dev_ctx.temp_attrs.min_measure_value,
	&dev_ctx.temp_attrs.max_measure_value,
	&dev_ctx.temp_attrs.tolerance
);

#define DEVICE_CLUSTER_LIST(            \
    cluster_list_name,                  \
    basic_attr_list,                    \
    indentify_attr_list,                \
    temperature_measurement_attr_list   \
    )                                   \
    zb_zcl_cluster_desc_t cluster_list_name[] =                     \
    {                                                               \
        ZB_ZCL_CLUSTER_DESC(                                        \
            ZB_ZCL_CLUSTER_ID_BASIC,                                \
            ZB_ZCL_ARRAY_SIZE(basic_attr_list, zb_zcl_attr_t),      \
            (basic_attr_list),                                      \
            ZB_ZCL_CLUSTER_SERVER_ROLE,                             \
            ZB_ZCL_MANUF_CODE_INVALID                               \
        ),                                                          \
        ZB_ZCL_CLUSTER_DESC(                                        \
            ZB_ZCL_CLUSTER_ID_IDENTIFY,                             \
            ZB_ZCL_ARRAY_SIZE(indentify_attr_list, zb_zcl_attr_t),  \
            (indentify_attr_list),                                  \
            ZB_ZCL_CLUSTER_SERVER_ROLE,                             \
            ZB_ZCL_MANUF_CODE_INVALID                               \
        ),                                                          \
        ZB_ZCL_CLUSTER_DESC(                                        \
            ZB_ZCL_CLUSTER_ID_TEMP_MEASUREMENT,                                     \
            ZB_ZCL_ARRAY_SIZE(temperature_measurement_attr_list, zb_zcl_attr_t),    \
            (temperature_measurement_attr_list),                                    \
            ZB_ZCL_CLUSTER_SERVER_ROLE,                             \
            ZB_ZCL_MANUF_CODE_INVALID                               \
        )                                                           \
    }

DEVICE_CLUSTER_LIST(
    device_cluster_list,
    basic_attr_list,
    identify_attr_list,
    temperature_measurement_attr_list
);

#define ZB_ZCL_DECLARE_DEVICE_DESC(     \
	ep_name,				            \
	ep_id,					            \
	in_clust_num,			            \
	out_clust_num			            \
)							            \
	ZB_DECLARE_SIMPLE_DESC(in_clust_num, out_clust_num);							\
	ZB_AF_SIMPLE_DESC_TYPE(in_clust_num, out_clust_num) simple_desc_##ep_name = 		\
	{																				\
		ep_id,																		\
		ZB_AF_HA_PROFILE_ID,														\
		0x0302,											\
		0,										\
		0,																			\
		in_clust_num,																\
		out_clust_num,																\
		{																			\
			ZB_ZCL_CLUSTER_ID_BASIC,												\
			ZB_ZCL_CLUSTER_ID_IDENTIFY,												\
			ZB_ZCL_CLUSTER_ID_TEMP_MEASUREMENT										\
		}																			\
	}	

#define ZB_HA_DECLARE_DEVICE_EP(ep_name, ep_id, cluster_list)   \
    ZB_ZCL_DECLARE_DEVICE_DESC(             \
        ep_name,                            \
        ep_id,                              \
        4,                                  \
        0                                   \
    );                                      \
    ZBOSS_DEVICE_DECLARE_REPORTING_CTX(     \
        reporting_info##ep_name,            \
        1                                   \
    );                                      \
    ZB_AF_DECLARE_ENDPOINT_DESC(            \
        ep_name,                            \
        ep_id,                              \
        ZB_AF_HA_PROFILE_ID,                \
        0,                                  \
        NULL,                               \
        ZB_ZCL_ARRAY_SIZE(cluster_list, zb_zcl_cluster_desc_t),     \
        cluster_list,                                               \
        (zb_af_simple_desc_1_1_t *)&simple_desc_##ep_name,           \
        1,                                                          \
        reporting_info##ep_name,                                    \
        0, NULL                                                     \
    )               
/* Endpoint setup (single) */
ZB_HA_DECLARE_DEVICE_EP(
	device_ep,
	10,
	device_cluster_list);

/* Device context */
ZBOSS_DECLARE_DEVICE_CTX_1_EP(
	device_ctx,
	device_ep);

void zboss_signal_handler(zb_bufid_t bufid) {
	zigbee_default_signal_handler(bufid);
    
}

static void identify_callback(zb_bufid_t bufid) {
    LOG_INF("Indentify callback invoked.");
}


static int init(void) {
    LOG_INF("Starting Zigbee thread.");

    ZB_AF_REGISTER_DEVICE_CTX(&device_ctx);

	/* Register callback to identify notifications */
	ZB_AF_SET_IDENTIFY_NOTIFICATION_HANDLER(10, identify_callback);

    zigbee_enable();


    return 0;
}

SYS_INIT_NAMED(zigbee, init, APPLICATION, 15);