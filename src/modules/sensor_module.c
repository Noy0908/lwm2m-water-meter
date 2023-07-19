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


