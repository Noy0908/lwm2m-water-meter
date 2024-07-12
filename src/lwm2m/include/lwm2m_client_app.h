/*
 * Copyright (c) 2019 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#ifndef LWM2M_CLIENT_APP_H__
#define LWM2M_CLIENT_APP_H__

#ifdef __cplusplus
extern "C" {
#endif

int lwm2m_app_init_device(char *serial_num);


#if defined(CONFIG_LWM2M_APP_WATER_METER)
int lwm2m_init_water_meter(void);

void update_water_meter_value(struct meter_data val);
#endif

#ifdef __cplusplus
}
#endif

#endif /* LWM2M_CLIENT_APP_H__ */
