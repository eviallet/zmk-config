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
	if (dev == NULL) {  return -EIO;  }
	ret = gpio_pin_configure(dev, ledPin, GPIO_OUTPUT | ledFlags);
	if (ret < 0) {  return -EIO;  }

	return gpio_pin_set(dev, ledPin, state) == 0; // 0 for success
}

static int zmk_listener_layer_led_callback(struct zmk_layer_state_changed *event) {
	if((*event).state == 0)
		return 0;
	return !(
		set_led(LED_PARAMS(DT_ALIAS(led0)), (*event).layer == 0) && 
		set_led(LED_PARAMS(DT_ALIAS(led1)), (*event).layer == 1) &&
		set_led(LED_PARAMS(DT_ALIAS(led2)), (*event).layer == 2) &&
		set_led(LED_PARAMS(DT_ALIAS(led3)), (*event).layer == 3)
	); // 0 for success
}

ZMK_SUBSCRIPTION(layer_led_callback, zmk_layer_state_changed);
// ZMK_LISTENER(zmk_layer_state_changed, layer_led_callback);
