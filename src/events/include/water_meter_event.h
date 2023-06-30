/*
 * Copyright (c) 2021 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#ifndef __WATER_METER_EVENT_H__
#define __WATER_METER_EVENT_H__

#include <zephyr/kernel.h>
#include <app_event_manager.h>
#include <app_event_manager_profiler_tracer.h>

#ifdef __cplusplus
extern "C" {
#endif


enum meter_alarm_type {
	FLOW_ALARM_NONE=0,
	FLOW_OVERSPEED_ALARM,
	LEAK_DETECTION_ALARM
};


struct water_meter_event {
	struct app_event_header header;
	enum meter_alarm_type type;
};

APP_EVENT_TYPE_DECLARE(water_meter_event);

#ifdef __cplusplus
}
#endif

#endif /* __WATER_METER_EVENT_H__ */
