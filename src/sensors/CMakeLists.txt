#
# Copyright (c) 2021 Nordic Semiconductor ASA
#
# SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
#

zephyr_include_directories(include)

target_sources_ifdef(CONFIG_APP_WATER_METER
					 app PRIVATE 
					 ${CMAKE_CURRENT_SOURCE_DIR}/meter_sensor.c
					 ${CMAKE_CURRENT_SOURCE_DIR}/meter_settings.c)

target_sources_ifdef(CONFIG_GNSS
	app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/gnss_module.c)


