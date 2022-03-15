#include <zephyr.h>
#include <device.h>
#include <devicetree.h>
#include <drivers/gpio.h>
/* The devicetree node identifier for the "led0" alias. */
#define LED_0 DT_ALIAS(led_0)
#define LED_1 DT_ALIAS(led_1)
#define LED_2 DT_ALIAS(led_2)
#define LED_3 DT_ALIAS(led_3)

#define LED_LABEL(led) DT_GPIO_LABEL(led, gpios)
#define LED_PIN(led) DT_GPIO_PIN(led, gpios)
#define LED_FLAGS(led) DT_GPIO_FLAGS(led, gpios)

static int set_led(const char* led, int state) {

	dev = device_get_binding(LED_LABEL(led));
	if (dev == NULL) {  return -EIO;  }
	ret = gpio_pin_configure(dev, LED_PIN(led), GPIO_OUTPUT | LED_FLAGS(led));
	if (ret < 0) {  return -EIO;  }

	return gpio_pin_set(dev, LED_PIN(led), true) == 0; // 0 if successful
}

static int layer_led_callback(struct zmk_layer_state_changed *event) {
	return 
		set_led(LED_0, (*event).layer == 0) && 
		set_led(LED_1, (*event).layer == 1) &&
		set_led(LED_2, (*event).layer == 2) &&
		set_led(LED_3, (*event).layer == 3);
}

SYS_INIT(layer_led_callback, APPLICATION, CONFIG_APPLICATION_INIT_PRIORITY);

// zmk_layer_state_changed