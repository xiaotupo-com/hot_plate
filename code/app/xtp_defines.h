/**
 * @file xtp_defines.h
 * @brief 小土坡专用宏定义
 * Copyright (C) 2024 xiaotupo.com
*/

#ifndef __XTP_DEFINES_H
#define __XTP_DEFINES_H

#ifdef __cplusplus
extern "C" {
#endif

#include "at32f413.h"

//================================================== GPIO 定义

// 状态 LED 的 GPIO 定义
#define STATE_LED_PORT GPIOB
#define STATE_LED_PIN GPIO_PINS_12
#define STATE_LED_CRM_CLOCK CRM_GPIOB_PERIPH_CLOCK

// 蜂鸣器的 GPIO 定义
#define BUZZER_PORT GPIOB
#define BUZZER_PIN GPIO_PINS_15
#define BUZZER_CRM_CLOCK CRM_GPIOB_PERIPH_CLOCK

// PWM控制的 GPIO 定义
#define PWM_PORT GPIOA
#define PWM_PIN GPIO_PINS_8
#define PWM_CRM_CLOCK CRM_GPIOA_PERIPH_CLOCK

#define EC11SW_PORT GPIOA
#define EC11SW_PIN GPIO_PINS_0
#define EC11SW_CRM_CLOCK CRM_GPIOA_PERIPH_CLOCK

#define EC11A_PORT GPIOA
#define EC11A_PIN GPIO_PINS_1
#define EC11A_CRM_CLOCK CRM_GPIOA_PERIPH_CLOCK

#define EC11B_PORT GPIOA
#define EC11B_PIN GPIO_PINS_2
#define EC11B_CRM_CLOCK CRM_GPIOA_PERIPH_CLOCK

#define PT100ADC_PORT GPIOA
#define PT100ADC_PIN GPIO_PINS_3
#define PT100ADC_CRM_CLOCK CRM_GPIOA_PERIPH_CLOCK

// OLED 定义用硬件IIC
#define OLED_CLK_PORT GPIOB
#define OLED_CLK_PIN GPIO_PINS_10
#define OLED_CLK_CRM_CLOCK CRM_GPIOB_PERIPH_CLOCK

#define OLED_SDA_PORT GPIOB
#define OLED_SDA_PIN GPIO_PINS_11
#define OLED_SDA_CRM_CLOCK CRM_GPIOB_PERIPH_CLOCK


#define USE_FREERTOS (1) // 1:使用 FreeRTOS 0:不使用 FreeRTOS

#define XTP_OK      0
#define XTP_FAIL    1
#define XTP_ERROR	1

#ifdef __cplusplus
}
#endif

#endif /* __XTP_DEFINES_H */
