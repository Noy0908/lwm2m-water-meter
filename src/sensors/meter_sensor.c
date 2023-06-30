/* Copyright (c) 2022 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#include <zephyr/kernel.h>
#include <app_event_manager.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <string.h>
#include <zephyr/logging/log.h>
#include <zephyr/logging/log_ctrl.h>
#include "meter_sensor.h"
#include "lwm2m_client_app.h"
#include "ucifi_water_meter.h"
#include <nrfx_gpiote.h>
#include <zephyr/sys/atomic.h>
#include "water_meter_event.h"

LOG_MODULE_REGISTER(meter_measure,CONFIG_APP_LOG_LEVEL);

/* size of stack area used by each thread */
#define STACKSIZE 1024
/* scheduling priority used by each thread */
#define PRIORITY 7


#define SW0_NODE	DT_ALIAS(sw0)
#if !DT_NODE_HAS_STATUS(SW0_NODE, okay)
#error "Unsupported board: sw0 devicetree alias is not defined"
#endif

static K_SEM_DEFINE(pulse_sem, 0, 1);

struct meter_data meter = {0};

// static uint32_t pulse_count = 0;
// int64_t start_time = 0;				//start time of the interrupt
struct recoder_interval leak_recorder = { 0 };		

static const struct gpio_dt_spec meter_pulse = GPIO_DT_SPEC_GET_OR(DT_NODELABEL(signal1), gpios, {0});
static const struct gpio_dt_spec leak_detection = GPIO_DT_SPEC_GET_OR(SW0_NODE, gpios,{0});

int meter_setting_data_save(void);


/**@brief   Measure handler
 */
static void meter_event_handler(nrfx_gpiote_pin_t pin, nrfx_gpiote_trigger_t trigger, void *context)
{
	if (pin == meter_pulse.pin)
	{
		LOG_DBG("count meter pulse %d\n", meter.pulse_value);
		meter.pulse_value++;
	}
	else if (pin == leak_detection.pin)
	{
		if (!gpio_pin_get_dt(&leak_detection)) {
			LOG_INF("Leak alert interrupt has stopped, now end the time[%d]\n", gpio_pin_get_dt(&leak_detection));
			leak_recorder.end = k_ticks_to_ms_floor64(k_uptime_ticks());
			leak_recorder.interval = leak_recorder.end - leak_recorder.start;
			LOG_INF("start=%lld\t end=%lld\t interval=%lld\n", leak_recorder.start,leak_recorder.end,leak_recorder.interval);

			if (leak_recorder.interval > CONFIG_MAX_LEAK_DETECTION_INTERVAL_MS) {
				LOG_WRN("Water leak detection alert!!!\n");
				meter.leak_detected = true;
				// wake up device and send leak alert to server
				struct water_meter_event *alarm_event = new_water_meter_event();
				alarm_event->type = LEAK_DETECTION_ALARM;
				APP_EVENT_SUBMIT(alarm_event);
			}
			else if (leak_recorder.interval > CONFIG_MIN_LEAK_DETECTION_INTERVAL_MS) {
				LOG_WRN("Water flow overspeed alert!!!\n");
			} 
			else {
				LOG_WRN("Normal water flow!\n");
				meter.leak_detected = false;
				// wake up device and send leak alarm clearned to server
				// struct water_meter_event *alarm_event = new_water_meter_event();
				// alarm_event->type = FLOW_ALARM_NONE;
				// APP_EVENT_SUBMIT(alarm_event);
			}
		} 
		else {
			LOG_INF("Leak alert interrupt has been triggered, now start the time[%d]\n", gpio_pin_get_dt(&leak_detection));
			leak_recorder.start = k_ticks_to_ms_floor64(k_uptime_ticks());
		}
	}
	
	// k_sem_give(&pulse_sem);
}




void clean_meter_data(uint16_t res_id)
{
	LOG_INF("Excute reset operation, res_id=%d\n",res_id
	);

	if(res_id == WATER_METER_CUMULATED_WATER_VOLUME_RID)
	{
		meter.volume = 0.0;
		meter.pulse_value = 0;
	}
	else if(res_id == WATER_METER_CUMULATED_PULSE_VALUE_RID)
	{
		meter.volume = 0.0;
		meter.pulse_value = 0;
	}
	else
	{
		LOG_ERR("The res_id error, the resource didn't support!!!\n");
	}

	meter_setting_data_save();
	// send_data_to_server();		//maybe you want to update data immediately
	//save data to flash
}


struct meter_data get_water_meter_volume(void)
{
	meter.volume = 0.2663 * meter.pulse_value;
	LOG_DBG("Update meter volume:%lf\t total_pulse=%d\n", meter.volume, meter.pulse_value);
	return meter;
}

#if 0
void water_meter_measure_thread(void)
{
	for (;;) 
	{
		// k_sleep(K_FOREVER);

		/* Wait for pulse event */
		// k_sem_take(&pulse_sem, K_FOREVER);

		uint64_t currentTime = k_ticks_to_ms_floor64(k_uptime_ticks());// Every second, calculate and print litres/hour
		if(currentTime >= (start_time + 1000))
		{
			start_time = currentTime; // Updates cloopTime
			// Pulse frequency (Hz) = 7.5Q, Q is flow rate in L/min.
			// l_hour = (pulse_count * 60 / 7.5); // (Pulse frequency x 60 min) / 7.5Q = flowrate in L/hour
			meter.volume = 0.2663 * meter.pulse_value;

			/** refresh water meter date*/
			// update_water_meter_value(meter);

			// meter.pulse_value += pulse_count;
			// LOG_ERR("total_pulse = %d\t volume = %fml\n", meter.pulse_value, meter.volume); // Print pulse count and volume?

			// if(0== pulse_count)		//The valve has been closed, no more pulse 
			// {
			// 	start_time = 0;
			// 	//save data to flash
			// }
			// else
			// {
			// 	pulse_count = 0; // Reset Counter
			// }

			/* Wait for pulse event */
			k_sem_take(&pulse_sem, K_FOREVER);
		}
	}
}
#endif

int water_meter_init(void)
{
	int err = 0;

	if (!device_is_ready(meter_pulse.port)) {
		LOG_ERR("Error: measure device %s is not ready\n", meter_pulse.port->name);
		return -1;
	}

	if (!device_is_ready(leak_detection.port)) {
		printk("Error: button device %s is not ready\n",leak_detection.port->name);
		return -1;
	}

	static const nrfx_gpiote_input_config_t input_config = {
		.pull = NRF_GPIO_PIN_NOPULL,
	};
	const nrfx_gpiote_trigger_config_t trigger_config = {
		.trigger = NRFX_GPIOTE_TRIGGER_LOTOHI,
		.p_in_channel = NULL,
	};

	static const nrfx_gpiote_handler_config_t handler_config = {
		.handler = meter_event_handler,
	};
	err = nrfx_gpiote_input_configure((nrfx_gpiote_pin_t)meter_pulse.pin, &input_config,
					  &trigger_config, &handler_config);
	if (err != NRFX_SUCCESS) {
		LOG_ERR("nrfx_gpiote_input_configure error: 0x%08X", err);
		return -1;
	}

	const nrfx_gpiote_trigger_config_t leak_trigger_config = {
		.trigger = NRFX_GPIOTE_TRIGGER_TOGGLE,
		.p_in_channel = NULL,
	};
	err = nrfx_gpiote_input_configure((nrfx_gpiote_pin_t)leak_detection.pin, &input_config,
					  &leak_trigger_config, &handler_config);
	if (err != NRFX_SUCCESS) {
		LOG_ERR("nrfx_gpiote_input_configure error: 0x%08X", err);
		return -1;
	}

	k_sleep(K_MSEC(800));
	nrfx_gpiote_trigger_enable((nrfx_gpiote_pin_t)meter_pulse.pin, true);
	nrfx_gpiote_trigger_enable((nrfx_gpiote_pin_t)leak_detection.pin, true);

	return 0;
}



// K_THREAD_DEFINE(measure_thread_id, STACKSIZE, water_meter_measure_thread, NULL, NULL, NULL, PRIORITY, 0, 0);
