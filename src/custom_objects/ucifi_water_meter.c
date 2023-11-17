/* Copyright (c) 2010 - 2017, Nordic Semiconductor ASA
*
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without modification,
* are permitted provided that the following conditions are met:
*
* 1. Redistributions of source code must retain the above copyright notice, this
*    list of conditions and the following disclaimer.
*
* 2. Redistributions in binary form, except as embedded into a Nordic
*    Semiconductor ASA integrated circuit in a product or a software update for
*    such product, must reproduce the above copyright notice, this list of
*    conditions and the following disclaimer in the documentation and/or other
*    materials provided with the distribution.
*
* 3. Neither the name of Nordic Semiconductor ASA nor the names of its
*    contributors may be used to endorse or promote products derived from this
*   software without specific prior written permission.
*
* 4. This software, with or without modification, must only be used with a
*    Nordic Semiconductor ASA integrated circuit.
*
* 5. Any software provided in binary form under this license must not be reverse
*    engineered, decompiled, modified and/or disassembled.
*
* THIS SOFTWARE IS PROVIDED BY NORDIC SEMICONDUCTOR ASA "AS IS" AND ANY EXPRESS
* OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
* OF MERCHANTABILITY, NONINFRINGEMENT, AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL NORDIC SEMICONDUCTOR ASA OR CONTRIBUTORS BE
* LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
* GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
* LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
* OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/*
 * Source material for IPSO Colour object (3424) v1.1:
 * https://devtoolkit.openmobilealliance.org/OEditor/LWMOView?url=https%3a%2f%2fraw.githubusercontent.com%2fOpenMobileAlliance%2flwm2m-registry%2fprod%2f3424.xml
 */

#include <zephyr/kernel.h>
#include <zephyr/init.h>
#include <lwm2m_object.h>
#include <lwm2m_engine.h>
#include <lwm2m_resource_ids.h>
#include <zephyr/logging/log.h>

#include "ucifi_water_meter.h"

LOG_MODULE_REGISTER(ucifi_water_meter, CONFIG_APP_LOG_LEVEL);

#define METER_NAME "water_meter"

// static char colour[MAX_INSTANCE_COUNT][COLOUR_STR_MAX_SIZE];
// static char units[MAX_INSTANCE_COUNT][UNIT_STR_MAX_SIZE];
// static char app_type[MAX_INSTANCE_COUNT][APP_TYPE_STR_MAX_SIZE];

/* resource state */
struct ucifi_water_meter_data {
	double volume;
	uint32_t pulse_value;
	uint32_t pulse_ratio;
	double max_flow_rate;
	double min_flow_rate;
	char app_type[APP_TYPE_STR_MAX_SIZE];
	bool leak_suspected;
	bool leak_detected;
	bool back_flow_detected;
	bool blocked_meter;
	bool fraud_detected;
};
static struct ucifi_water_meter_data meter_data[MAX_INSTANCE_COUNT];

static struct lwm2m_engine_obj water_meter;
static struct lwm2m_engine_obj_field fields[] = {
	OBJ_FIELD_DATA(WATER_METER_CUMULATED_WATER_VOLUME_RID, R, FLOAT),
	OBJ_FIELD_EXECUTE_OPT(WATER_METER_CUMULATED_WATER_METER_VALUE_RESET_RID),
	OBJ_FIELD_DATA(WATER_METER_TYPE_RID, RW_OPT, STRING),
	OBJ_FIELD_DATA(WATER_METER_CUMULATED_PULSE_VALUE_RID, R_OPT, U32),
	OBJ_FIELD_EXECUTE_OPT(WATER_METER_CUMULATED_PULSE_VALUE_RESET_RID),
	OBJ_FIELD_DATA(WATER_METER_PULSE_RATIO_RID, RW_OPT, U32),
	OBJ_FIELD_DATA(WATER_METER_MINIMUM_FLOW_RATE_RID, R_OPT, FLOAT),
	OBJ_FIELD_DATA(WATER_METER_MAXIMUM_FLOW_RATE_RID, R_OPT, FLOAT),
	OBJ_FIELD_DATA(WATER_METER_LEAK_SUSPECTED_RID, R_OPT, BOOL),
	OBJ_FIELD_DATA(WATER_METER_LEAK_DETECTE_RID, R_OPT, BOOL),
	OBJ_FIELD_DATA(WATER_METER_BACK_FLOW_DETECTED_RID, R_OPT, BOOL),
	OBJ_FIELD_DATA(WATER_METER_BLOCKED_METER_RID, R_OPT, BOOL),
	OBJ_FIELD_DATA(WATER_METER_FRAUD_DETECTED_RID, R_OPT, BOOL),
};

static struct lwm2m_engine_obj_inst inst[MAX_INSTANCE_COUNT];
static struct lwm2m_engine_res res[MAX_INSTANCE_COUNT][NUMBER_OF_OBJECT_FIELDS];
static struct lwm2m_engine_res_inst res_inst[MAX_INSTANCE_COUNT][RESOURCE_INSTANCE_COUNT];

static clean_meter_data_cb_t clean_meter_data_cb;

void regitster_clean_meter_data_cb(clean_meter_data_cb_t cb)
{
	clean_meter_data_cb = cb;
}


static void clear_cumulated_meter_volume(uint16_t obj_inst_id, int index)
{
	meter_data[index].volume = 0;
	lwm2m_notify_observer(UCIFI_OBJECT_WATER_METER_ID, obj_inst_id,WATER_METER_CUMULATED_WATER_VOLUME_RID);

	if(clean_meter_data_cb)
	{
		clean_meter_data_cb(WATER_METER_CUMULATED_WATER_VOLUME_RID);
	}
}

static void clear_cumulated_meter_pulse(uint16_t obj_inst_id, int index)
{
	meter_data[index].pulse_value = 0;
	lwm2m_notify_observer(UCIFI_OBJECT_WATER_METER_ID, obj_inst_id,WATER_METER_CUMULATED_PULSE_VALUE_RID);

	if(clean_meter_data_cb)
	{
		clean_meter_data_cb(WATER_METER_CUMULATED_PULSE_VALUE_RID);
	}
}


static int reset_meter_measured_values_cb(uint16_t obj_inst_id, uint8_t *args, uint16_t args_len)
{
	uint8_t i = 0;
	LOG_DBG("RESET meter measured value %d\r\n", obj_inst_id);
	for (i = 0; i < MAX_INSTANCE_COUNT; i++) {
		if (inst[i].obj && inst[i].obj_inst_id == obj_inst_id) {
			clear_cumulated_meter_volume(obj_inst_id, i);
			return 0;
		}
	}
	return -ENOENT;
}


static int reset_meter_pulse_values_cb(uint16_t obj_inst_id, uint8_t *args, uint16_t args_len)
{
	uint8_t i = 0;
	LOG_DBG("RESET meter pulse value %d\r\n", obj_inst_id);
	for (i = 0; i < MAX_INSTANCE_COUNT; i++) {
		if (inst[i].obj && inst[i].obj_inst_id == obj_inst_id) {
			clear_cumulated_meter_pulse(obj_inst_id, i);
			return 0;
		}
	}

	return -ENOENT;
}


static struct lwm2m_engine_obj_inst *water_meter_create(uint16_t obj_inst_id)
{
	int index, i = 0, j = 0;

	/* Check that there is no other instance with this ID */
	for (index = 0; index < MAX_INSTANCE_COUNT; index++) {
		if (inst[index].obj && inst[index].obj_inst_id == obj_inst_id) {
			LOG_ERR("Can not create instance - already existing: %u", obj_inst_id);
			return NULL;
		}
	}

	for (index = 0; index < MAX_INSTANCE_COUNT; index++) {
		if (!inst[index].obj) {
			break;
		}
	}

	if (index >= MAX_INSTANCE_COUNT) {
		LOG_ERR("Can not create instance - no more room: %u", obj_inst_id);
		return NULL;
	}

	(void)memset(res[index], 0, sizeof(res[index][0]) * ARRAY_SIZE(res[index]));
	init_res_instance(res_inst[index], ARRAY_SIZE(res_inst[index]));

	/* initialize instance resource data */
	// INIT_OBJ_RES(WATER_METER_CUMULATED_WATER_VOLUME_RID, res[index], i, res_inst[index], j, 1,
	// 	     false, true, &meter_data[index].volume, sizeof(meter_data[index].volume),
	// 	     NULL, NULL, NULL, meter_value_write_cb, NULL);
	INIT_OBJ_RES_DATA(WATER_METER_CUMULATED_WATER_VOLUME_RID, res[index], i, res_inst[index], j, &meter_data[index].volume,
				sizeof(meter_data[index].volume));
	INIT_OBJ_RES_EXECUTE(WATER_METER_CUMULATED_WATER_METER_VALUE_RESET_RID, res[index], i,
			     reset_meter_measured_values_cb);
	INIT_OBJ_RES_OPTDATA(WATER_METER_TYPE_RID, res[index], i, res_inst[index], j);
	INIT_OBJ_RES_DATA(WATER_METER_CUMULATED_PULSE_VALUE_RID, res[index], i, res_inst[index], j, &meter_data[index].pulse_value,
			      sizeof(meter_data[index].pulse_value));
	INIT_OBJ_RES_EXECUTE(WATER_METER_CUMULATED_PULSE_VALUE_RESET_RID, res[index], i,
			     reset_meter_pulse_values_cb);
	INIT_OBJ_RES_DATA(WATER_METER_PULSE_RATIO_RID, res[index], i, res_inst[index], j, &meter_data[index].pulse_ratio,
			    sizeof(meter_data[index].pulse_ratio));
	INIT_OBJ_RES_DATA(WATER_METER_MINIMUM_FLOW_RATE_RID, res[index], i, res_inst[index], j, &meter_data[index].min_flow_rate,
			    sizeof(meter_data[index].min_flow_rate));
	INIT_OBJ_RES_DATA(WATER_METER_MAXIMUM_FLOW_RATE_RID, res[index], i, res_inst[index], j, &meter_data[index].max_flow_rate,
				sizeof(meter_data[index].max_flow_rate));
	INIT_OBJ_RES_DATA(WATER_METER_LEAK_SUSPECTED_RID, res[index], i, res_inst[index], j, &meter_data[index].leak_suspected,
				sizeof(meter_data[index].leak_suspected));
	INIT_OBJ_RES_DATA(WATER_METER_LEAK_DETECTE_RID, res[index], i, res_inst[index], j, &meter_data[index].leak_detected,
				sizeof(meter_data[index].leak_detected));
	INIT_OBJ_RES_DATA(WATER_METER_BACK_FLOW_DETECTED_RID, res[index], i, res_inst[index], j, &meter_data[index].back_flow_detected,
				sizeof(meter_data[index].back_flow_detected));
	INIT_OBJ_RES_DATA(WATER_METER_BLOCKED_METER_RID, res[index], i, res_inst[index], j, &meter_data[index].blocked_meter,
				sizeof(meter_data[index].blocked_meter));
	INIT_OBJ_RES_DATA(WATER_METER_FRAUD_DETECTED_RID, res[index], i, res_inst[index], j, &meter_data[index].fraud_detected,
				sizeof(meter_data[index].fraud_detected));
	// INIT_OBJ_RES_OPTDATA(WATER_METER_BACK_FLOW_DETECTED_RID, res[index], i, res_inst[index], j);
	// INIT_OBJ_RES_OPTDATA(WATER_METER_BLOCKED_METER_RID, res[index], i, res_inst[index], j);
	// INIT_OBJ_RES_OPTDATA(WATER_METER_FRAUD_DETECTED_RID, res[index], i, res_inst[index], j);

	inst[index].resources = res[index];
	inst[index].resource_count = i;
	LOG_DBG("Created UCIFI %s instance: %d", METER_NAME, obj_inst_id);
	return &inst[index];
}

static int ipso_water_meter_init(void)
{
	water_meter.obj_id = UCIFI_OBJECT_WATER_METER_ID;
	water_meter.version_major = WATER_METER_VERSION_MAJOR;
	water_meter.version_minor = WATER_METER_VERSION_MINOR;
	water_meter.is_core = false;
	water_meter.fields = fields;
	water_meter.field_count = ARRAY_SIZE(fields);
	water_meter.max_instance_count = MAX_INSTANCE_COUNT;
	water_meter.create_cb = water_meter_create;
	lwm2m_register_obj(&water_meter);

	return 0;
}

SYS_INIT(ipso_water_meter_init, APPLICATION, CONFIG_KERNEL_INIT_PRIORITY_DEFAULT);
