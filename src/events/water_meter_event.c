/*
 * Copyright (c) 2021 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#include "water_meter_event.h"

static const char *alarm_type_to_string(enum meter_alarm_type type)
{
	switch (type) {
	case FLOW_OVERSPEED_ALARM:
		return "flow overspeed alarm ";

	case LEAK_DETECTION_ALARM:
		return "leak detection alarm ";

	default:
		return "water flow normally";
	}
}

static void log_water_meter_event(const struct app_event_header *aeh)
{
	struct water_meter_event *event = cast_water_meter_event(aeh);

	APP_EVENT_MANAGER_LOG(aeh, "%s event", alarm_type_to_string(event->type));
}

APP_EVENT_TYPE_DEFINE(water_meter_event,
			      log_water_meter_event,
			      NULL,
			      APP_EVENT_FLAGS_CREATE());
