/* Copyright (c) 2022 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#ifndef _MEASURE_H_
#define _MEASURE_H_


#define METER_VOLUME    1
#define PULSE_VALUE     2

struct meter_data {
	double volume;				//total volume of the meter
	uint32_t pulse_value;		//cumulated number of pulse detected on the meter
	// uint32_t pulse_ratio;
	// bool leak_suspected;
	bool leak_detected;
};



struct recoder_interval {
	uint64_t start;			//start time of the recorder
	uint64_t end;			//end time of the recorder
	uint64_t interval;		//interval of the recorder
};	//record the leak detection event start time and end time


struct meter_data get_water_meter_volume(void);

void clean_meter_data(uint16_t res_id);

int water_meter_init(void);


#endif /* _MEASURE_H_ */
