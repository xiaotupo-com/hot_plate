#include "gpio_init.h"
#include "xtp_defines.h"

void xtp_gpio_init(void)
{
    /* 定义 GPIO 初始化结构体 */
    gpio_init_type gpio_init_struct;

    /* 使能 GPIOA 外设时钟 */
    crm_periph_clock_enable(STATE_LED_CRM_CLOCK | BUZZER_CRM_CLOCK | PWM_CRM_CLOCK |
                                EC11SW_CRM_CLOCK | EC11A_CRM_CLOCK | EC11B_CRM_CLOCK |
                                PT100ADC_CRM_CLOCK | OLED_CLK_CRM_CLOCK | OLED_SDA_CRM_CLOCK,
                            TRUE);

    /* 设置默认参数 */
    gpio_default_para_init(&gpio_init_struct);

    /* 配置 STATE_LED 的 GPIO 参数 */
    gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER; /* 较大电流推动/吸入能力 */
    gpio_init_struct.gpio_out_type       = GPIO_OUTPUT_OPEN_DRAIN;                 /* 推挽输出 */
    gpio_init_struct.gpio_mode           = GPIO_MODE_OUTPUT;
    gpio_init_struct.gpio_pins           = STATE_LED_PIN;
    gpio_init_struct.gpio_pull           = GPIO_PULL_NONE;
    gpio_init(STATE_LED_PORT, &gpio_init_struct);

    /* 配置 BUZZER 的 GPIO 参数 */
    gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER; /* 较大电流推动/吸入能力 */
    gpio_init_struct.gpio_out_type       = GPIO_OUTPUT_PUSH_PULL;                 /* 推挽输出 */
    gpio_init_struct.gpio_mode           = GPIO_MODE_OUTPUT;
    gpio_init_struct.gpio_pins           = BUZZER_PIN;
    gpio_init_struct.gpio_pull           = GPIO_PULL_NONE;
    gpio_init(BUZZER_PORT, &gpio_init_struct);
}