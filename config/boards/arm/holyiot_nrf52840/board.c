#include <zephyr.h>
#include <device.h>
#include <devicetree.h>
#include <drivers/gpio.h>
#include <zmk/event_manager.h>
#include <zmk/events/layer_state_changed.h>

#define LED_NODE(led) 	DT_ALIAS(led)
#define LED_LABEL(led) 	DT_GPIO_LABEL(led, gpios)
#define LED_PIN(led) 	DT_GPIO_PIN(led, gpios)
#define LED_FLAGS(led) 	DT_GPIO_FLAGS(led, gpios)

#define LED_PARAMS(led) LED_LABEL(led), LED_PIN(led), LED_FLAGS(led)

static int set_led(const char* ledNode, const gpio_pin_t ledPin, const int ledFlags, int state) {
	const struct device *dev = device_get_binding(ledNode);
	int ret;

	if (dev == NULL) { 
		return -EIO;
	}

	ret = gpio_pin_configure(dev, ledPin, GPIO_OUTPUT | ledFlags);
	if (ret < 0) { 
		return -EIO; 
	}

	return gpio_pin_set(dev, ledPin, state) == 0; // 0 for success, but for easier and-ing, return 1
}

static int set_single_led(int index) {
	return !(
		set_led(LED_PARAMS(DT_ALIAS(led0)), index == 0) &&
		set_led(LED_PARAMS(DT_ALIAS(led1)), index == 1) &&
		set_led(LED_PARAMS(DT_ALIAS(led2)), index == 2) &&
		set_led(LED_PARAMS(DT_ALIAS(led3)), index == 3)
	);
}

static int board_leds_init() {
	return set_single_led(0);
}

static int layer_state_changed_listener(const zmk_event_t *ev) {
    struct zmk_layer_state_changed *data = as_zmk_layer_state_changed(ev);
    if(data == NULL) { // only consider enabled events
		return 0;
	}

	if(data->layer == 3 && data->state == 0) { // first layer selected = last layer disabled
		return set_single_led(0);
	} else if(data->state) {
		return set_single_led(data->layer);
	} else {
		return 0;
	}
}

ZMK_LISTENER(board_leds, layer_state_changed_listener);
ZMK_SUBSCRIPTION(board_leds, zmk_layer_state_changed);

// Need to happen after GPIO driver init
SYS_INIT(board_leds_init, POST_KERNEL, CONFIG_KERNEL_INIT_PRIORITY_DEVICE);
