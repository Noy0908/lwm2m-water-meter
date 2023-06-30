#
# Copyright (c) 2019 Nordic Semiconductor
#
# SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
#

zephyr_include_directories(include)

target_sources(app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/lwm2m_device.c)
target_sources(app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/lwm2m_location.c)
target_sources(app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/lwm2m_app_utils.c)


target_sources_ifdef(CONFIG_LWM2M_APP_LIGHT_CONTROL
	app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/lwm2m_light_control.c)

target_sources_ifdef(CONFIG_LWM2M_PORTFOLIO_OBJ_SUPPORT
	app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/lwm2m_portfolio_object.c)

target_sources_ifdef(CONFIG_LWM2M_APP_WATER_METER
	app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/lwm2m_water_meter.c)
