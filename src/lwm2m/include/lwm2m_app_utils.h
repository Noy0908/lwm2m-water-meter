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
#include <zephyr/net/lwm2m.h>
#include <zephyr/drivers/sensor.h>

#ifndef LWM2M_APP_UTILS_H__
#define LWM2M_APP_UTILS_H__

/* LwM2M Object IDs */
#define LWM2M_OBJECT_SECURITY_ID 0
#define LWM2M_OBJECT_SERVER_ID 1

#define LWM2M_OBJECT_DEVICE_ID 3

/* IPSO Object IDs */
#define IPSO_OBJECT_COLOUR_ID 3335

/* Server RIDs */
#define LIFETIME_RID 1

/* Device RIDs */
#define MANUFACTURER_RID 0
#define MODEL_NUMBER_RID 1
#define SERIAL_NUMBER_RID 2
#define FACTORY_RESET_RID 5
#define POWER_SOURCE_RID 6
#define POWER_SOURCE_VOLTAGE_RID 7
#define POWER_SOURCE_CURRENT_RID 8
#define BATTERY_LEVEL_RID 9
#define CURRENT_TIME_RID 13
#define UTC_OFFSET_RID 14
#define TIMEZONE_RID 15
#define DEVICE_TYPE_RID 17
#define HARDWARE_VERSION_RID 18
#define SOFTWARE_VERSION_RID 19
#define BATTERY_STATUS_RID 20
#define MEMORY_TOTAL_RID 21

/* Location RIDs */
#define LATITUDE_RID 0
#define LONGITUDE_RID 1
#define ALTITUDE_RID 2
#define LOCATION_RADIUS_RID 3
#define LOCATION_VELOCITY_RID 4
#define LOCATION_TIMESTAMP_RID 5
#define LOCATION_SPEED_RID 6

/* Misc */
#define LWM2M_RES_DATA_FLAG_RW 0 /* LwM2M Resource read-write flag */
#define MAX_LWM2M_PATH_LEN 20 /* Maximum string length of an LwM2M path */
#define RGBIR_STR_LENGTH 11 /* String length of RGB-IR colour string: '0xRRGGBBIR\0' */

#ifdef __cplusplus
extern "C" {
#endif

/* Set timestamp resource */
void set_ipso_obj_timestamp(int ipso_obj_id, unsigned int obj_inst_id);

/* Check whether notification read callback or regular read callback */
bool is_regular_read_cb(int64_t read_timestamp);

#ifdef __cplusplus
}
#endif

#endif /* LWM2M_APP_UTILS_H__ */
