/**
  **************************************************************************
  * @file     ptc.hpp
  * @brief    ptc加热相关定义。
  **************************************************************************
  * 引脚定义: PA8
  * PA8 低电平开始加热，初始化时要设置成高电平，以免上电就开始加热
  **************************************************************************
  */
#ifndef __PTC_H
#define __PTC_H

  // 包含 at32f413.h 
#include "at32f413.h"

/* PTC加入控制IO定义 */
#define PTC_PWM_GPIOX					GPIOA
#define PTC_PWM_PIN						GPIO_PINS_8
#define PTC_PWM_CRM_PERIPH_CLOCK		CRM_GPIOA_PERIPH_CLOCK

#define PTC_OFF()	gpio_bits_write(PTC_PWM_GPIOX, PTC_PWM_PIN, TRUE)
#define PTC_ON()	gpio_bits_write(PTC_PWM_GPIOX, PTC_PWM_PIN, FALSE)


class Ptc
{
public:
    uint32_t m_pins;
    gpio_pull_type m_pull;
    gpio_type* m_gpiox;

public:
    Ptc(gpio_type* gpiox, uint32_t pins, gpio_pull_type pull, crm_periph_clock_type crm_periph_clock);
};

#endif
