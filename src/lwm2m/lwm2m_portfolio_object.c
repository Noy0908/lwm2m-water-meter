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
#include <lwm2m_resource_ids.h>
#include "lwm2m_app_utils.h"

static char host_device_id[40] = "Host Device ID #1";
static char manufacturer_id[40] = "Host Develce Manufacturer #1";
static char device_model[40] = "Host Device Model #1";
static char software_version_id[40] = "Host Device Software Version #1";

int lwm2m_init_portfolio_object(void)
{
	/* create switch1 object */
	lwm2m_create_object_inst(&LWM2M_OBJ(16, 0));
	lwm2m_create_res_inst(&LWM2M_OBJ(16, 0, 0, 0));
	lwm2m_set_res_buf(&LWM2M_OBJ(16, 0, 0, 0), host_device_id, 40, 40, 0);
	lwm2m_create_res_inst(&LWM2M_OBJ(16, 0, 0, 1));
	lwm2m_set_res_buf(&LWM2M_OBJ(16, 0, 0, 1), manufacturer_id, 40, 40, 0);
	lwm2m_create_res_inst(&LWM2M_OBJ(16, 0, 0, 2));
	lwm2m_set_res_buf(&LWM2M_OBJ(16, 0, 0, 2), device_model, 40, 40, 0);
	lwm2m_create_res_inst(&LWM2M_OBJ(16, 0, 0, 3));
	lwm2m_set_res_buf(&LWM2M_OBJ(16, 0, 0, 3), software_version_id, 40, 40, 0);

	return 0;
}
