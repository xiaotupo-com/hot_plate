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

#define PTC_OFF()	gpio_bits_write(PWM_PORT, PWM_PIN, TRUE)
#define PTC_ON()	gpio_bits_write(PWM_PORT, PWM_PIN, FALSE)


#endif
