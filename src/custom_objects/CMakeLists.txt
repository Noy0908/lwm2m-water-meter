#
# Copyright (c) 2021 Nordic Semiconductor
#
# SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
#

target_sources_ifdef(CONFIG_LWM2M_UCIFI_APP_WATER_METER
	app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/ucifi_water_meter.c)