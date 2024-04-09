/**
  **************************************************************************
  * @file     ec11.h
  * @brief    EC11旋转编码器相关定义。
  **************************************************************************
  * 引脚定义
  * SW: PA0
  * A: 	PA1
  * B: 	PA2
  **************************************************************************
  */
#ifndef __EC11_H
#define __EC11_H

  // 包含 at32f413.h 
#include "at32f413.h"

/* EC11 旋转编码器 IO定义 */
#define EC11_SW_GPIOX				GPIOA
#define EC11_SW_PIN					GPIO_PINS_0
#define EC11_SW_CRM_PERIPH_CLOCK	CRM_GPIOA_PERIPH_CLOCK

#define EC11_A_GPIOX				GPIOA
#define EC11_A_PIN					GPIO_PINS_1
#define EC11_A_CRM_PERIPH_CLOCK		CRM_GPIOA_PERIPH_CLOCK

#define EC11_B_GPIOX				GPIOA
#define EC11_B_PIN					GPIO_PINS_2
#define EC11_A_CRM_PERIPH_CLOCK		CRM_GPIOA_PERIPH_CLOCK

#define EC11_SW_GET() gpio_input_data_bit_read(EC11_SW_GPIOX, EC11_SW_PIN)


enum class Ec11Dir
{
    CW = 0, 	/* 顺时针方向 */
    CCW,		/* 逆时针方向 */
    NONE		/* 未转动 */
};

enum class Ec11Btn
{
    CLICK = 0,    /* 单击 */
    DOUBLE_CLICK, /* 双击 */
    THREE_CLICK,  /* 三连击 */
    LOOP_PRESS,   /* 按键一直按下 */
    NO_CLICK
};

class Ec11
{
public:
    Ec11Dir dir;

public:
    Ec11();
};

#endif
