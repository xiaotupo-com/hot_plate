#include "led.h"

#include "gpio_init.h"
#include "xtp_defines.h"

static void state_led_toggle(void);
static void state_led_off(void);
static void state_led_on(void);

struct Led state_led = {
	.off = state_led_off,
	.on = state_led_on,
	.toggle = state_led_toggle
};


/**
 * @brief 打开 LED
*/
static void state_led_on(void)
{
    gpio_bits_write(STATE_LED_PORT, STATE_LED_PIN, TRUE);
}

/**
 * @brief 关闭 LED
*/
static void state_led_off(void)
{
    gpio_bits_write(STATE_LED_PORT, STATE_LED_PIN, FALSE);
}

/**
 * @brief 反转LED的状态
*/
static void state_led_toggle(void)
{
    STATE_LED_PORT->odt ^= STATE_LED_PIN;
}
