#include <zephyr.h>
#include <device.h>
#include <devicetree.h>
#include <drivers/gpio.h>
/* The devicetree node identifier for the "led0" alias. */
#define LED_0 DT_ALIAS(led_0)
#define LED DT_GPIO_LABEL(LED_0, gpios)
#define PIN DT_GPIO_PIN(LED_0, gpios)
#define FLAGS DT_GPIO_FLAGS(LED_0, gpios)

#define LED_1 DT_ALIAS(led_1)
#define LED_2 DT_ALIAS(led_2)
#define LED_3 DT_ALIAS(led_3)


static int layer_led_callback(const struct device *dev) {
	int ret;

	dev = device_get_binding(LED);
	if (dev == NULL) {
		return -EIO;
	}

	ret = gpio_pin_configure(dev, PIN, GPIO_OUTPUT | FLAGS);
	if (ret < 0) {
		return -EIO;
	}

	return gpio_pin_set(dev, PIN, true);
}

SYS_INIT(layer_led_callback, APPLICATION, CONFIG_APPLICATION_INIT_PRIORITY);
