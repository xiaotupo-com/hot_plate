/**
  **************************************************************************
  * @file     main.c
  * @brief    main program
  **************************************************************************
  *
  *
  */
#include "at32f413_clock.h"
#include "xtp_defines.h"
#include "FreeRTOS.h"
#include "task.h"
#include "hot_plate.h"


/**
  * @brief  main function.
  * @param  none
  * @retval none
  */
int main(void)
{
    system_clock_config();
    SysTick_Config(SystemCoreClock / 1000);

    

    // vTaskStartScheduler(); 启动任务调度器

    while (1)
    {

    }
}

void vApplicationTickHook(void)
{

}
