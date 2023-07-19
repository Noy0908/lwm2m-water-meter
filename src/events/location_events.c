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

#include <stdio.h>
#include <net/lwm2m_client_utils_location.h>

#include "location_events.h"

#if defined(CONFIG_LWM2M_CLIENT_UTILS_LOCATION_ASSIST_AGPS)
static void log_gnss_agps_request_event(const struct app_event_header *eh)
{
	APP_EVENT_MANAGER_LOG(eh, "got agps request event");
}

APP_EVENT_TYPE_DEFINE(gnss_agps_request_event, log_gnss_agps_request_event, NULL,
		      APP_EVENT_FLAGS_CREATE());

#endif
#if defined(CONFIG_LWM2M_CLIENT_UTILS_GROUND_FIX_OBJ_SUPPORT)
static void log_ground_fix_location_request_event(const struct app_event_header *eh)
{
	APP_EVENT_MANAGER_LOG(eh, "got ground fix location request event");
}

APP_EVENT_TYPE_DEFINE(ground_fix_location_request_event, log_ground_fix_location_request_event,
		      NULL, APP_EVENT_FLAGS_CREATE());

static void log_ground_fix_location_inform_event(const struct app_event_header *eh)
{
	APP_EVENT_MANAGER_LOG(eh, "got ground fix location inform event");
}

APP_EVENT_TYPE_DEFINE(ground_fix_location_inform_event, log_ground_fix_location_inform_event, NULL,
		      APP_EVENT_FLAGS_CREATE());
#endif
#if defined(CONFIG_LWM2M_CLIENT_UTILS_LOCATION_ASSIST_PGPS)
static void log_pgps_data_request_event(const struct app_event_header *eh)
{
	APP_EVENT_MANAGER_LOG(eh, "got pgps data request event");
}

APP_EVENT_TYPE_DEFINE(pgps_data_request_event, log_pgps_data_request_event, NULL,
		      APP_EVENT_FLAGS_CREATE());
#endif
