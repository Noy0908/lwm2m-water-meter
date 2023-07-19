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
#include <nrf_modem_gnss.h>
#include <zephyr/net/lwm2m.h>
#include <zephyr/net/lwm2m_path.h>
#include <zephyr/sys/timeutil.h>

#include "gnss_pvt_event.h"
#include "lwm2m_app_utils.h"

static time_t gnss_mktime(struct nrf_modem_gnss_datetime *date)
{
	struct tm tm = {
		.tm_sec = date->seconds,
		.tm_min = date->minute,
		.tm_hour = date->hour,
		.tm_mday = date->day,
		.tm_mon = date->month - 1,
		.tm_year = date->year - 1900
	};
	return timeutil_timegm(&tm);
}

static bool app_event_handler(const struct app_event_header *aeh)
{
	if (is_gnss_pvt_event(aeh)) {
		struct gnss_pvt_event *event = cast_gnss_pvt_event(aeh);

		double latitude = event->pvt.latitude;
		double longitude = event->pvt.longitude;
		double altitude = event->pvt.altitude;
		double speed = event->pvt.speed;
		double radius = event->pvt.accuracy;
		time_t timestamp = gnss_mktime(&event->pvt.datetime);

		lwm2m_set_f64(&LWM2M_OBJ(LWM2M_OBJECT_LOCATION_ID, 0, LATITUDE_RID), latitude);
		lwm2m_set_f64(&LWM2M_OBJ(LWM2M_OBJECT_LOCATION_ID, 0, LONGITUDE_RID), longitude);
		lwm2m_set_f64(&LWM2M_OBJ(LWM2M_OBJECT_LOCATION_ID, 0, ALTITUDE_RID), altitude);
		lwm2m_set_f64(&LWM2M_OBJ(LWM2M_OBJECT_LOCATION_ID, 0, LOCATION_RADIUS_RID), radius);
		lwm2m_set_f64(&LWM2M_OBJ(LWM2M_OBJECT_LOCATION_ID, 0, LOCATION_SPEED_RID), speed);
		lwm2m_set_time(&LWM2M_OBJ(LWM2M_OBJECT_LOCATION_ID, 0, LOCATION_TIMESTAMP_RID),
			       timestamp);

		return true;
	}

	return false;
}

APP_EVENT_LISTENER(location, app_event_handler);
APP_EVENT_SUBSCRIBE(location, gnss_pvt_event);
