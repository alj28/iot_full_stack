#ifndef _ZB_HELPER_H_
#define _ZB_HELPER_H_

#include <zcl/zb_zcl_temp_measurement_addons.h>


#define ZB_TEMPERATURE_SENSOR_DEVICE_ID     0x0302

#define ZB_DEVICE_VER_TEMPERATURE_SENSOR    1

#define ZB_TEMPERATURE_SENSOR_IN_CLUSTER_NUM    4
#define ZB_TEMPERATURE_SENSOR_OUT_CLUSTER_NUM   0
#define ZB_TEMPERATURE_SENSOR_CLUSTER_NUM   \
            (ZB_TEMPERATURE_SENSOR_IN_CLUSTER_NUM + ZB_TEMPERATURE_SENSOR_OUT_CLUSTER_NUM)

#define ZB_TEMPERATURE_SENSOR_REPORT_ATTR_COUNT \
            (4)

#define ZB_TEMPERATURE_SENSOR_CVC_ATTR_COUNT    \
            (1)

/**
 * @brief Declare cluster list for Temperature Sensor device
 * @param cluster_list_name - cluster list variable name
 * @param basic_attr_list - attribute list for Basic cluster
 * @param identify_attr_list - attribute list for Identify cluster
 * @param temperature_measurement_attr_list - attribute list for Temperature Measurement cluster
 */
#define ZB_DECLARE_TEMPERATURE_SENSOR_CLUSTER_LIST(					   \
	cluster_list_name,							   \
	basic_attr_list,							   \
	identify_attr_list,							   \
	temperature_measurement_attr_list,				\
	on_off_attr_list)			    \
	zb_zcl_cluster_desc_t cluster_list_name[] =				   \
	{									   \
		ZB_ZCL_CLUSTER_DESC(						   \
			ZB_ZCL_CLUSTER_ID_IDENTIFY,				   \
			ZB_ZCL_ARRAY_SIZE(identify_attr_list, zb_zcl_attr_t),	   \
			(identify_attr_list),					   \
			ZB_ZCL_CLUSTER_SERVER_ROLE,				   \
			ZB_ZCL_MANUF_CODE_INVALID				   \
		),								   \
		ZB_ZCL_CLUSTER_DESC(						   \
			ZB_ZCL_CLUSTER_ID_BASIC,				   \
			ZB_ZCL_ARRAY_SIZE(basic_attr_list, zb_zcl_attr_t),	   \
			(basic_attr_list),					   \
			ZB_ZCL_CLUSTER_SERVER_ROLE,				   \
			ZB_ZCL_MANUF_CODE_INVALID				   \
		),								   \
		ZB_ZCL_CLUSTER_DESC(						   \
			ZB_ZCL_CLUSTER_ID_TEMP_MEASUREMENT,				   \
			ZB_ZCL_ARRAY_SIZE(temperature_measurement_attr_list, zb_zcl_attr_t),	   \
			(temperature_measurement_attr_list),					   \
			ZB_ZCL_CLUSTER_SERVER_ROLE,				   \
			ZB_ZCL_MANUF_CODE_INVALID				   \
		),											\
		ZB_ZCL_CLUSTER_DESC(						   \
			ZB_ZCL_CLUSTER_ID_ON_OFF,				   \
			ZB_ZCL_ARRAY_SIZE(on_off_attr_list, zb_zcl_attr_t),	   \
			(on_off_attr_list),					   \
			ZB_ZCL_CLUSTER_SERVER_ROLE,				   \
			ZB_ZCL_MANUF_CODE_INVALID				   \
		)											\
	}

/**
 * @brief Declare simple descriptor for Temperature Sensor device
 * @param ep_name - endpoint variable name
 * @param ep_id - endpoint ID
 * @param in_clust_num - number of supported input clusters
 * @param out_clust_num - number of supported output clusters
 */
#define ZB_ZCL_DECLARE_HA_TEMPERATURE_SENSOR_SIMPLE_DESC(ep_name, ep_id, in_clust_num, out_clust_num) \
	ZB_DECLARE_SIMPLE_DESC(in_clust_num, out_clust_num);					  \
	ZB_AF_SIMPLE_DESC_TYPE(in_clust_num, out_clust_num) simple_desc_##ep_name =		  \
	{											  \
		ep_id,										  \
		ZB_AF_HA_PROFILE_ID,								  \
		ZB_TEMPERATURE_SENSOR_DEVICE_ID,            \
		ZB_DEVICE_VER_TEMPERATURE_SENSOR,							  \
		0,										  \
		in_clust_num,									  \
		out_clust_num,									  \
		{										  \
			ZB_ZCL_CLUSTER_ID_IDENTIFY,						  \
			ZB_ZCL_CLUSTER_ID_BASIC,						  \
            ZB_ZCL_CLUSTER_ID_TEMP_MEASUREMENT,               \
			ZB_ZCL_CLUSTER_ID_ON_OFF	\
		}										  \
	}

/**
 * @brief Declare endpoint for Temperature Sensor device
 * @param ep_name - endpoint variable name
 * @param ep_id - endpoint ID
 * @param cluster_list - endpoint cluster list
 */
#define ZB_DECLARE_TEMPERATURE_SENSOR_EP(ep_name, ep_id, cluster_list)		      \
	ZB_ZCL_DECLARE_HA_TEMPERATURE_SENSOR_SIMPLE_DESC(ep_name, ep_id,		      \
		ZB_TEMPERATURE_SENSOR_IN_CLUSTER_NUM, ZB_TEMPERATURE_SENSOR_OUT_CLUSTER_NUM); \
	ZBOSS_DEVICE_DECLARE_REPORTING_CTX(reporting_info## ep_name,		      \
		ZB_TEMPERATURE_SENSOR_REPORT_ATTR_COUNT);				      \
	ZBOSS_DEVICE_DECLARE_LEVEL_CONTROL_CTX(cvc_alarm_info## ep_name,	      \
		ZB_TEMPERATURE_SENSOR_CVC_ATTR_COUNT);				      \
	ZB_AF_DECLARE_ENDPOINT_DESC(ep_name, ep_id, ZB_AF_HA_PROFILE_ID,	      \
		0,								      \
		NULL,								      \
		ZB_ZCL_ARRAY_SIZE(cluster_list, zb_zcl_cluster_desc_t), cluster_list, \
			(zb_af_simple_desc_1_1_t *)&simple_desc_## ep_name,	      \
			ZB_TEMPERATURE_SENSOR_REPORT_ATTR_COUNT,			      \
			reporting_info## ep_name,				      \
			ZB_TEMPERATURE_SENSOR_CVC_ATTR_COUNT,			      \
			cvc_alarm_info## ep_name)

#endif /* _ZB_HELPER_H_ */