/*
 * Copyright (c) 2019 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#ifndef LWM2M_CLIENT_APP_H__
#define LWM2M_CLIENT_APP_H__

#include <zephyr/kernel.h>
#include <zephyr/net/lwm2m.h>

#if defined(CONFIG_LWM2M_APP_WATER_METER)
#include "meter_sensor.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

int lwm2m_app_init_device(char *serial_num);


#if defined(CONFIG_LWM2M_APP_WATER_METER)
int lwm2m_init_water_meter(void);

void update_water_meter_value(struct meter_data val);
#endif

#if defined(CONFIG_LWM2M_PORTFOLIO_OBJ_SUPPORT)
int lwm2m_init_portfolio_object(void);
#endif

#ifdef __cplusplus
}
#endif

#endif /* LWM2M_CLIENT_APP_H__ */
