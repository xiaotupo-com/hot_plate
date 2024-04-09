/**
 **************************************************************************
 * @file     ssd1306.h
 * @brief    ssd1306 驱动
 **************************************************************************
 * MCU: AT32F413CCU7
 * IO分配：
 * PB10 -> I2C2_SCL
 * PB11 -> I2C2_SDA
 * 所用 IIC 为 I2C2
 **************************************************************************
 *
 */
#ifndef __IIC_OLED_H
#define __IIC_OLED_H

#include <stdbool.h>

#include "i2c_application.h"


// 使用DMA定义
#define USE_DMA

#define I2Cx_DMA_TX_IRQn DMA1_Channel4_IRQn

#define SSD1306_ADDR 0x78 // 0x78 or 0x7A
#define SSD1306_CMD 0x00  // D/C# 位置1 0b0x00_0000 x为0命令，x为1数据
#define SSD1306_DATA 0x40 // D/C# 位置1 0b0x00_0000 x为0命令，x为1数据
#define OLED_IIC_SPEED 1000000
#define IIC_TIMEOUT 0xFFFFFFFF

#define OLED_IIC I2C2 // I2CX
#define OLED_IIC_PERIPH_CLK CRM_I2C2_PERIPH_CLOCK

#define OLED_SDA_PORT GPIOB
#define OLED_SDA_PIN GPIO_PINS_11
#define OLED_SDA_CLK CRM_GPIOB_PERIPH_CLOCK

#define OLED_SCL_PORT GPIOB
#define OLED_SCL_PIN GPIO_PINS_10
#define OLED_SCL_CLK CRM_GPIOB_PERIPH_CLOCK

//================ SSD1306 命令定义
#define DISPLAY_OFF 0xAE
#define DISPLAY_ON 0xAF
#define NORMAL_DISPLAY 0xA6
#define INVERT_DISPLAY 0xA7
#define ENTIRE_DISPLAY 0xA5 // 强制全屏显示
#define GDDRAM_DISPLAY 0xA4 // 显示 GDDRAM 中的内容

#define SSD1306_WIDTH 128
#define SSD1306_HEIGHT 64
#define SSD1306_BUFFER_SIZE (SSD1306_WIDTH * SSD1306_HEIGHT / 8)

/**
 * 正常显示与反色显示
 */
typedef enum
{
    NORMAL_COLOR = 0,
    INVERT_COLOR
} DisplayColor_enum_t;

/**
 * 0度显示/180度显示/镜像显示
 */
typedef enum
{
    DISPLAY_0_DEG = 0,
    DISPLAY_180_DEG,
    DISPLAY_MIRROR // 镜像显示，就是左右反转，上下正常显示
} Display_DEG_enum_t;

typedef enum
{
    BLACK = 0,
    WHITE = 0x01,
    INVERSE = 0x02
} SSD1306_Color_enum_t;

typedef struct
{
    u16 current_x;
    u16 current_y;
    SSD1306_Color_enum_t color;
    bool inverted;

    void (*update)(void);
    void (*set_color)(SSD1306_Color_enum_t);
    void (*init)(void);
    void (*on)(void);
    void (*off)(void);
    void (*clear)(void);
    void (*fill)(void);
    void (*draw_pixel)(u8, u8);
    void (*draw_line)(i16 x0, i16 y0, i16 x1, i16 y1);
    void (*draw_horizontal_line)(i16 x, i16 y, i16 length);
    void (*draw_vertical_line)(i16 x, i16 y, i16 length);
    void (*draw_rect)(i16 x, i16 y, i16 width, i16 height);
    void (*draw_fill_rect)(i16 xMove, i16 yMove, i16 width, i16 height);
    void (*draw_triangle)(u16 x1, u16 y1, u16 x2, u16 y2, u16 x3, u16 y3);
    void (*draw_fill_triangle)(u16 x1, u16 y1, u16 x2, u16 y2, u16 x3, u16 y3);
    void (*draw_circle)(i16 x0, i16 y0, i16 radius);
    void (*draw_fill_circle)(i16 x0, i16 y0, i16 radius);
    void (*set_display_deg)(Display_DEG_enum_t);
    void (*set_inverted)(bool);
    void (*set_color_mode)(DisplayColor_enum_t);
} SSD1306_t;

extern i2c_status_type i2c_status;
extern i2c_handle_type hi2cx;

extern SSD1306_t ssd1306;

#endif /* __IIC_OLED_H */
