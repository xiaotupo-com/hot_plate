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

#define EC11_SW_GET() gpio_input_data_bit_read(EC11SW_PORT, EC11SW_PIN)


enum Ec11Dir
{
    CW = 0, 	/* 顺时针方向 */
    CCW,		/* 逆时针方向 */
    NONE		/* 未转动 */
};

enum Ec11Btn
{
    CLICK = 0,    /* 单击 */
    DOUBLE_CLICK, /* 双击 */
    THREE_CLICK,  /* 三连击 */
    LOOP_PRESS,   /* 按键一直按下 */
    NO_CLICK
};


#endif
