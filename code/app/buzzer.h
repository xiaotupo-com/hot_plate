/**
  **************************************************************************
  * @file     buzzer.h
  * @brief    蜂鸣器相关定义。
  **************************************************************************
  * 引脚定义：		PB15
  * ON（响）: 		高电平
  * OFF（不响）: 	低电平
  *
  **************************************************************************
  */
#ifndef __BUZZER_H
#define __BUZZER_H

// 包含 at32f413.h 
#include "at32f413.h"
#include "xtp_defines.h"

#define BUZZER_OFF() 	gpio_bits_write(BUZZER_PORT, BUZZER_PIN, FALSE)
#define BUZZER_ON() 	gpio_bits_write(BUZZER_PORT, BUZZER_PIN, TRUE)

void buzzer_init(void);
void buzzer_run(uint16_t period);

#endif
