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
#include <zephyr/net/lwm2m_path.h>
#include <net/lwm2m_client_utils_location.h>
#include <net/lwm2m_client_utils.h>
#include <zephyr/net/lwm2m.h>

#include "location_events.h"

static struct lwm2m_ctx *client_ctx;

#if defined(CONFIG_LWM2M_CLIENT_UTILS_SIGNAL_MEAS_INFO_OBJ_SUPPORT)
#define CELL_LOCATION_PATHS 7
#else
#define CELL_LOCATION_PATHS 6
#endif

#define AGPS_LOCATION_PATHS 7

#define REQUEST_WAIT_INTERVAL K_SECONDS(5)

#if defined(CONFIG_LWM2M_CLIENT_UTILS_LOCATION_ASSIST_AGPS)
static bool handle_agps_request(const struct gnss_agps_request_event *event)
{
	while (location_assistance_agps_set_mask(&event->agps_req) == -EAGAIN) {
		k_sleep(REQUEST_WAIT_INTERVAL);
	}
	while (location_assistance_agps_request_send(client_ctx, true) == -EAGAIN) {
		k_sleep(REQUEST_WAIT_INTERVAL);
	}

	return true;
}
#endif

#if defined(CONFIG_LWM2M_CLIENT_UTILS_GROUND_FIX_OBJ_SUPPORT)
static bool handle_ground_fix_location_event(bool send_back)
{
	ground_fix_set_report_back(send_back);
	location_assistance_ground_fix_request_send(client_ctx);

	return true;
}
#endif

#if defined(CONFIG_LWM2M_CLIENT_UTILS_LOCATION_ASSIST_PGPS)
static bool handle_pgps_data_request_event(void)
{
	while (location_assistance_pgps_request_send(client_ctx, true) == -EAGAIN) {
		k_sleep(REQUEST_WAIT_INTERVAL);
	}
	return true;
}
#endif

static bool event_handler(const struct app_event_header *eh)
{
	if (client_ctx == 0) {
		return false;
	}
#if defined(CONFIG_LWM2M_CLIENT_UTILS_LOCATION_ASSIST_AGPS)
	if (is_gnss_agps_request_event(eh)) {
		struct gnss_agps_request_event *event = cast_gnss_agps_request_event(eh);

		handle_agps_request(event);
		return true;
	}
#endif
#if defined(CONFIG_LWM2M_CLIENT_UTILS_GROUND_FIX_OBJ_SUPPORT)
	if (is_ground_fix_location_request_event(eh)) {
		handle_ground_fix_location_event(true);
		return true;
	} else if (is_ground_fix_location_inform_event(eh)) {
		handle_ground_fix_location_event(false);
		return true;
	}
#endif
#if defined(CONFIG_LWM2M_CLIENT_UTILS_LOCATION_ASSIST_PGPS)
	if (is_pgps_data_request_event(eh)) {
		handle_pgps_data_request_event();
		return true;
	}
#endif

	return false;
}

APP_EVENT_LISTENER(location_handler, event_handler);
#if defined(CONFIG_LWM2M_CLIENT_UTILS_LOCATION_ASSIST_AGPS)
APP_EVENT_SUBSCRIBE(location_handler, gnss_agps_request_event);
#endif
#if defined(CONFIG_LWM2M_CLIENT_UTILS_GROUND_FIX_OBJ_SUPPORT)
APP_EVENT_SUBSCRIBE(location_handler, ground_fix_location_request_event);
APP_EVENT_SUBSCRIBE(location_handler, ground_fix_location_inform_event);
#endif
#if defined(CONFIG_LWM2M_CLIENT_UTILS_LOCATION_ASSIST_PGPS)
APP_EVENT_SUBSCRIBE(location_handler, pgps_data_request_event);
#endif

int location_event_handler_init(struct lwm2m_ctx *ctx)
{
	int err = 0;

	if (client_ctx == 0) {
		client_ctx = ctx;
	} else {
		err = -EALREADY;
	}

	return err;
}
