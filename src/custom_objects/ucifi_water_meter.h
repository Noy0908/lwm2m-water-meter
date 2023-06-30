/**
 * @file ucifi_water_meter.h
 * @brief
 *
 * Copyright (c) 2021 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef __UCIFI_WATER_METER__
#define __UCIFI_WATER_METER__

#include <zephyr/net/lwm2m.h>



#define UCIFI_OBJECT_WATER_METER_ID 3424

// #define MAX_INSTANCE_COUNT CONFIG_LWM2M_UCIFI_WATER_METER_INSTANCE_COUNT
#define MAX_INSTANCE_COUNT 			1

#define WATER_METER_VERSION_MAJOR 	1
#define WATER_METER_VERSION_MINOR 	0
#define NUMBER_OF_OBJECT_FIELDS 	13

#define RESOURCE_INSTANCE_COUNT 	NUMBER_OF_OBJECT_FIELDS

// #define UNIT_STR_MAX_SIZE 8
// #define COLOUR_STR_MAX_SIZE 16
#define APP_TYPE_STR_MAX_SIZE 		32


/* Resource IDs for water meter */
/* clang-format off */
#define WATER_METER_CUMULATED_WATER_VOLUME_RID                  1
#define WATER_METER_CUMULATED_WATER_METER_VALUE_RESET_RID       2
#define WATER_METER_TYPE_RID                                    3
#define WATER_METER_CUMULATED_PULSE_VALUE_RID                   4
#define WATER_METER_CUMULATED_PULSE_VALUE_RESET_RID             5
#define WATER_METER_PULSE_RATIO_RID                             6
#define WATER_METER_MINIMUM_FLOW_RATE_RID                       7
#define WATER_METER_MAXIMUM_FLOW_RATE_RID                       8
#define WATER_METER_LEAK_SUSPECTED_RID                          9
#define WATER_METER_LEAK_DETECTE_RID                            10
#define WATER_METER_BACK_FLOW_DETECTED_RID                      11
#define WATER_METER_BLOCKED_METER_RID                           12
#define WATER_METER_FRAUD_DETECTED_RID                          13
/* clang-format on */


typedef void (*clean_meter_data_cb_t)(uint16_t res_id);


void regitster_clean_meter_data_cb(clean_meter_data_cb_t cb);

#endif /* __UCIFI_WATER_METER__ */
