/*
 * Copyright (c) 2021 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#include <zephyr/logging/log.h>
#include <zephyr/logging/log_ctrl.h>
#include <string.h>
#include <zephyr/kernel.h>
#include <zephyr/settings/settings.h>
#include <zephyr/drivers/uart.h>
#include <dfu/dfu_target.h>
#include "meter_sensor.h"

LOG_MODULE_REGISTER(meter_settings, CONFIG_APP_LOG_LEVEL);


// struct meter_data value;	/* meter: value */

extern struct meter_data meter;


static int settings_set(const char *name, size_t len, settings_read_cb read_cb, void *cb_arg)
{
	if (!strcmp(name, "volume")) {
		if (len != sizeof(meter.volume))
			return -EINVAL;
		if (read_cb(cb_arg, &meter.volume, len) > 0)
			return 0;
	} else if (!strcmp(name, "pulse_value")) {
		if (len != sizeof(meter.pulse_value))
			return -EINVAL;
		if (read_cb(cb_arg, &meter.pulse_value, len) > 0)
			return 0;
	} else if (!strcmp(name, "leak_detected")) {
		if (len != sizeof(meter.leak_detected))
			return -EINVAL;
		if (read_cb(cb_arg, &meter.leak_detected, len) > 0)
			return 0;
	}

	return -ENOENT;
}



static struct settings_handler meter_settings_conf = {
	.name = "meter_data",
	.h_set = settings_set
};



int water_meter_settings_init(void)
{
	int ret;

	ret = settings_subsys_init();
	if (ret) {
		LOG_ERR("Init setting failed: %d", ret);
		return ret;
	}
	ret = settings_register(&meter_settings_conf);
	if (ret) {
		LOG_ERR("Register setting failed: %d", ret);
		return ret;
	}
	ret = settings_load_subtree("meter_data");
	if (ret) {
		LOG_ERR("Load setting failed: %d", ret);
	}

    LOG_DBG("\nload meter data success:volume=%lf\t total_pulse=%d\n\n", meter.volume, meter.pulse_value);
	return ret;
}

int meter_setting_data_save(void)
{
	int ret;

	/* Write a single serialized value to persisted storage (if it has changed value). */
	ret = settings_save_one("meter_data/volume", &(meter.volume), sizeof(meter.volume));
	if (ret) {
		LOG_ERR("save meter_data/volume failed: %d", ret);
		return ret;
	}
	ret = settings_save_one("meter_data/pulse_value", &(meter.pulse_value), sizeof(meter.pulse_value));
	if (ret) {
		LOG_ERR("save meter_data/pulse_value failed: %d", ret);
		return ret;
	}
	ret = settings_save_one("meter_data/leak_detected", &(meter.leak_detected), sizeof(meter.leak_detected));
	if (ret) {
		LOG_ERR("save slm/fota_status failed: %d", ret);
		return ret;
	}

    LOG_DBG("\nsave meter data success:volume=%lf\t total_pulse=%d\n\n", meter.volume, meter.pulse_value);

	return 0;
}


