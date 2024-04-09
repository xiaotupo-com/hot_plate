/**
 * @file led.h
 * @brief led 所接IO为PB12
*/

#ifndef __LED_H
#define __LED_H

#include "FreeRTOS.h"
#include "task.h"
#include "at32f413.h"


class Led
{
public:
    uint32_t m_pins;
    gpio_pull_type m_pull;
    gpio_type* m_gpiox;

public:
    Led(gpio_type* gpiox, uint32_t pins, gpio_pull_type pull, crm_periph_clock_type crm_periph_clock);
    
    void on();
    void off();
    void toggle();
};

#endif
