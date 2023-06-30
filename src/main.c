/*
 * Copyright (c) 2016 Intel Corporation
 * Copyright (c) 2020 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @file Sample app to demonstrate PWM.
 */

#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/device.h>
#include <zephyr/drivers/pwm.h>
#include <zephyr/drivers/gpio.h>


// #define MIN_PERIOD PWM_SEC(1U) / 128U

#define MIN_PERIOD PWM_SEC(1U) / 128U
#define MAX_PERIOD PWM_SEC(1U)


/*
 * Get button configuration from the devicetree sw0 alias. This is mandatory.
 */
#define SW0_NODE	DT_ALIAS(sw0)
#if !DT_NODE_HAS_STATUS(SW0_NODE, okay)
#error "Unsupported board: sw0 devicetree alias is not defined"
#endif
static const struct gpio_dt_spec button = GPIO_DT_SPEC_GET_OR(SW0_NODE, gpios,{0});
static struct gpio_callback button_cb_data;


// static const struct pwm_dt_spec pwm_led = PWM_DT_SPEC_GET(DT_ALIAS(pwm_led1));

static const struct pwm_dt_spec pwm_led = PWM_DT_SPEC_GET(DT_NODELABEL(pwm_led2));			//P0-10

uint32_t max_period;
uint32_t period;
uint8_t dir = 0U;
volatile bool pressFlag = false;

static void button_pressed(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{
	period = dir ? (period * 2U) : (period / 2U);
	if (period > max_period) {
		period = max_period / 2U;
		dir = 0U;
	} else if (period < MIN_PERIOD) {
		period = MIN_PERIOD * 2U;
		dir = 1U;
	}

	pressFlag = true;

	// printk("Button pressed at %ld\n", k_cycle_get_32());
}


void main(void)
{
	int ret;

	printk("PWM-based blinky\n");

	if (!device_is_ready(pwm_led.dev)) {
		printk("Error: PWM device %s is not ready\n",
		       pwm_led.dev->name);
		return;
	}

	/*
	 * In case the default MAX_PERIOD value cannot be set for
	 * some PWM hardware, decrease its value until it can.
	 *
	 * Keep its value at least MIN_PERIOD * 4 to make sure
	 * the sample changes frequency at least once.
	 */
	printk("Calibrating for channel %d...\n", pwm_led.channel);
	max_period = MAX_PERIOD;
	while (pwm_set_dt(&pwm_led, max_period, max_period / 2U)) {
		max_period /= 2U;
		if (max_period < (4U * MIN_PERIOD)) {
			printk("Error: PWM device "
			       "does not support a period at least %lu\n",
			       4U * MIN_PERIOD);
			return;
		}
	}

	printk("Done calibrating; maximum/minimum periods %u/%lu usec\n",
	       max_period, MIN_PERIOD);

	/* config the button interrupt which used to change the PWM period*/
	if (!device_is_ready(button.port)) {
		printk("Error: button device %s is not ready\n",
		       button.port->name);
		return;
	}

	ret = gpio_pin_configure_dt(&button, GPIO_INPUT);
	if (ret != 0) {
		printk("Error %d: failed to configure %s pin %d\n",
		       ret, button.port->name, button.pin);
		return;
	}

	ret = gpio_pin_interrupt_configure_dt(&button,
					      GPIO_INT_EDGE_TO_ACTIVE);
	if (ret != 0) {
		printk("Error %d: failed to configure interrupt on %s pin %d\n",
			ret, button.port->name, button.pin);
		return;
	}

	gpio_init_callback(&button_cb_data, button_pressed, BIT(button.pin));
	gpio_add_callback(button.port, &button_cb_data);
	printk("Set up button at %s pin %d\n", button.port->name, button.pin);

	period = MIN_PERIOD;
	pwm_set_dt(&pwm_led, period, period);

	while (1) {

		if(pressFlag)
		{
			ret = pwm_set_dt(&pwm_led, period, period / 2U);
			if (ret) {
				printk("Error %d: failed to set pulse width\n", ret);
				return;
			}

			pressFlag = false;
			printk("period=%d usec, frequency=%d\n" , period/1000, 1000000/(period/1000));
		}
		else
		{
			k_sleep(K_MSEC(300U));
		}
	}
}
