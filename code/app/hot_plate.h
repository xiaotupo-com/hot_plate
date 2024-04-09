#ifndef __HOT_PLATE_H
#define __HOT_PLATE_H

#include "FreeRTOS.h"
#include "task.h"
#include "at32f413.h"
#include "led.hpp"
#include "ptc.hpp"

class HotPlate
{
public:
    u32 global_num_1;
    Led* led;
    Ptc* ptc;

public:
    HotPlate();
    static void create_task(void);
};


#endif