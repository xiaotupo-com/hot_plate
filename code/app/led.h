/**
 * @file led.h
 * @brief led 所接IO为PB12
*/

#ifndef __LED_H
#define __LED_H

#include "FreeRTOS.h"
#include "task.h"
#include "at32f413.h"

struct Led
{
	void (*off)(void);
	void (*on)(void);
	void (*toggle)(void);
};


extern struct Led state_led;
#endif
