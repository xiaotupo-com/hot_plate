#include "led.h"

#include "gpio_init.h"

/**
 * @brief Construct a new Led:: Led object
 * 
 * @param gpiox 
 * @param pins 
 * @param pull 
 * @param crm_periph_clock 
 */
Led::Led(gpio_type* gpiox, uint32_t pins, gpio_pull_type pull, crm_periph_clock_type crm_periph_clock)
{
    m_gpiox = gpiox;
    m_pins = pins;
    m_pull = pull;
    Gpio::init(gpiox, pins, pull, crm_periph_clock, GPIO_MODE_OUTPUT, GPIO_OUTPUT_PUSH_PULL);
}

/**
 * @brief 打开 LED
*/
void Led::on()
{
    gpio_bits_write(m_gpiox, m_pins, TRUE);
}

/**
 * @brief 关闭 LED
*/
void Led::off()
{
    gpio_bits_write(m_gpiox, m_pins, FALSE);
}

/**
 * @brief 反转LED的状态
*/
void Led::toggle()
{
    m_gpiox->odt ^= m_pins;
}
