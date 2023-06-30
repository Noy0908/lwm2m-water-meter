/*
 * Copyright (c) 2021 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#include <zephyr/kernel.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/net/lwm2m.h>
#include <lwm2m_resource_ids.h>
#include <math.h>
#include <stdlib.h>
#include <adp536x.h>
#include "lwm2m_app_utils.h"
#include "meter_sensor.h"
#include "ucifi_water_meter.h"
#include "location_events.h"
#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(app_sensors, CONFIG_APP_LOG_LEVEL);

struct k_work_delayable water_meter_work;


#define PERIOD K_MINUTES(2)
#define PERIOD K_SECONDS(10)
// #define RETRY K_MSEC(200)



static void water_meter_work_cb(struct k_work *work)
{
	struct meter_data val = get_water_meter_volume();
	lwm2m_set_f64(&LWM2M_OBJ(UCIFI_OBJECT_WATER_METER_ID, 0, WATER_METER_CUMULATED_WATER_VOLUME_RID), val.volume);
	lwm2m_set_u32(&LWM2M_OBJ(UCIFI_OBJECT_WATER_METER_ID, 0, WATER_METER_CUMULATED_PULSE_VALUE_RID), val.pulse_value);
	printf("Update meter volume:%lf\t total_pulse=%d\n", val.volume, val.pulse_value);

	// k_work_schedule(&water_meter_work, PERIOD);
}



int sensor_module_init(void)
{
	if (IS_ENABLED(CONFIG_SENSOR_MODULE_WATER_METER)) {
		k_work_init_delayable(&water_meter_work, water_meter_work_cb);
		k_work_schedule(&water_meter_work, K_NO_WAIT);
	}

	return 0;
}


