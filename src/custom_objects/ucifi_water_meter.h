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

#ifndef __UCIFI_WATER_METER__
#define __UCIFI_WATER_METER__

#include <zephyr/net/lwm2m.h>



#define UCIFI_OBJECT_WATER_METER_ID 3424

// #define MAX_INSTANCE_COUNT CONFIG_LWM2M_UCIFI_WATER_METER_INSTANCE_COUNT
#define MAX_INSTANCE_COUNT 			1

#define WATER_METER_VERSION_MAJOR 	1
#define WATER_METER_VERSION_MINOR 	0
#define NUMBER_OF_OBJECT_FIELDS 	13

#define RESOURCE_INSTANCE_COUNT 	NUMBER_OF_OBJECT_FIELDS

// #define UNIT_STR_MAX_SIZE 8
// #define COLOUR_STR_MAX_SIZE 16
#define APP_TYPE_STR_MAX_SIZE 		32


/* Resource IDs for water meter */
/* clang-format off */
#define WATER_METER_CUMULATED_WATER_VOLUME_RID                  1
#define WATER_METER_CUMULATED_WATER_METER_VALUE_RESET_RID       2
#define WATER_METER_TYPE_RID                                    3
#define WATER_METER_CUMULATED_PULSE_VALUE_RID                   4
#define WATER_METER_CUMULATED_PULSE_VALUE_RESET_RID             5
#define WATER_METER_PULSE_RATIO_RID                             6
#define WATER_METER_MINIMUM_FLOW_RATE_RID                       7
#define WATER_METER_MAXIMUM_FLOW_RATE_RID                       8
#define WATER_METER_LEAK_SUSPECTED_RID                          9
#define WATER_METER_LEAK_DETECTE_RID                            10
#define WATER_METER_BACK_FLOW_DETECTED_RID                      11
#define WATER_METER_BLOCKED_METER_RID                           12
#define WATER_METER_FRAUD_DETECTED_RID                          13
/* clang-format on */


typedef void (*clean_meter_data_cb_t)(uint16_t res_id);


void regitster_clean_meter_data_cb(clean_meter_data_cb_t cb);

#endif /* __UCIFI_WATER_METER__ */
