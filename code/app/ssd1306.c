/**
 **************************************************************************
 * @file     iic_oled.c
 * @brief    iic_oled
 **************************************************************************
 *
 *
 **************************************************************************
 *
 */
#include "ssd1306.h"
#include <stdlib.h>
#include <string.h>

#include "at32f413.h"

#include "i2c_application.h"
#include "oled_font.h"
#include "state_led.h"
#include "stdio.h"
#include "utils.h"
#include "xtp_type.h"

extern uint32_t systick_count;
i2c_handle_type hi2cx;
i2c_status_type i2c_status;

static void ssd1306_send_command(u8 command);
static void ssd1306_send_data(u8 *data, u16 size);
static void _set_color(SSD1306_Color_enum_t color);
static void _update_screen(void);
static void _draw_pixel(u8 x, u8 y);
static void _draw_line(i16 x0, i16 y0, i16 x1, i16 y1);
static void _draw_horizontal_line(i16 x, i16 y, i16 length);
static void _draw_vertical_line(i16 x, i16 y, i16 length);
static void _draw_rect(i16 x, i16 y, i16 width, i16 height);
static void _draw_fill_rect(i16 xMove, i16 yMove, i16 width, i16 height);
static void _draw_triangle(u16 x1, u16 y1, u16 x2, u16 y2, u16 x3, u16 y3);
static void _draw_fill_triangle(u16 x1, u16 y1, u16 x2, u16 y2, u16 x3, u16 y3);
static void _draw_circle(i16 x0, i16 y0, i16 radius);
static void _draw_fill_circle(i16 x0, i16 y0, i16 radius);
void _init(void);
static void _On(void);
static void _Off(void);
static void _clear(void);
static void _fill(void);
static void _set_display_deg(Display_DEG_enum_t DEG);
static void _set_inverted(bool value);
static void _set_color_mode(DisplayColor_enum_t ColorMode);
void ssd1306_type_init(SSD1306_t *_ssd1306);

SSD1306_t ssd1306 = {
    .color = WHITE,
    .current_x = 0,
    .current_y = 0,
    .inverted = false,
    .init = _init,
    .update = _update_screen,
    .set_color_mode = _set_color_mode,
    .set_color = _set_color,
    .clear = _clear,
    .on = _On,
    .off = _Off,
    .fill = _fill,
    .draw_pixel = _draw_pixel,
    .draw_line = _draw_line,
    .draw_horizontal_line = _draw_horizontal_line,
    .draw_vertical_line = _draw_vertical_line,
    .draw_rect = _draw_rect,
    .draw_fill_rect = _draw_fill_rect,
    .draw_triangle = _draw_triangle,
    .draw_fill_triangle = _draw_fill_triangle,
    .draw_circle = _draw_circle,
    .draw_fill_circle = _draw_fill_circle,
    .set_display_deg = _set_display_deg,
    .set_inverted = _set_inverted};

static u8 ssd1306_buffer[SSD1306_BUFFER_SIZE];

static u16 width(void) { return SSD1306_WIDTH; }
static u16 height(void) { return SSD1306_HEIGHT; }

/**
 * @brief  initializes peripherals used by the i2c.
 * @param  none
 * @retval none
 */
void i2c_lowlevel_init(i2c_handle_type *hi2c)
{
    gpio_init_type gpio_initstructure;

    if (hi2c->i2cx == OLED_IIC)
    {
        /* i2c periph clock enable */
        crm_periph_clock_enable(OLED_IIC_PERIPH_CLK, TRUE);
        crm_periph_clock_enable(OLED_SDA_CLK, TRUE);
        crm_periph_clock_enable(OLED_SCL_CLK, TRUE);

        /* gpio configuration */
        gpio_initstructure.gpio_out_type = GPIO_OUTPUT_OPEN_DRAIN;
        gpio_initstructure.gpio_pull = GPIO_PULL_UP;
        gpio_initstructure.gpio_mode = GPIO_MODE_MUX;
        gpio_initstructure.gpio_drive_strength = GPIO_DRIVE_STRENGTH_MODERATE;

        /* configure i2c pins: scl */
        gpio_initstructure.gpio_pins = OLED_SCL_PIN;
        gpio_init(OLED_SCL_PORT, &gpio_initstructure);

        /* configure i2c pins: sda */
        gpio_initstructure.gpio_pins = OLED_SDA_PIN;
        gpio_init(OLED_SDA_PORT, &gpio_initstructure);

        /* configure and enable i2c dma channel interrupt */
        nvic_irq_enable(I2Cx_DMA_TX_IRQn, 0, 0);

        nvic_irq_enable(I2C2_EVT_IRQn, 0, 0);
        nvic_irq_enable(I2C2_ERR_IRQn, 0, 0);

        crm_periph_clock_enable(CRM_DMA1_PERIPH_CLOCK, TRUE);

        dma_reset(hi2c->dma_tx_channel);

        hi2c->dma_tx_channel = DMA1_CHANNEL4;

        dma_default_para_init(&hi2c->dma_init_struct);
        hi2c->dma_init_struct.peripheral_inc_enable = FALSE;
        hi2c->dma_init_struct.memory_inc_enable = TRUE;
        hi2c->dma_init_struct.peripheral_data_width =
            DMA_PERIPHERAL_DATA_WIDTH_BYTE;
        hi2c->dma_init_struct.memory_data_width = DMA_MEMORY_DATA_WIDTH_BYTE;
        hi2c->dma_init_struct.loop_mode_enable = FALSE;
        hi2c->dma_init_struct.priority = DMA_PRIORITY_LOW;
        hi2c->dma_init_struct.direction = DMA_DIR_MEMORY_TO_PERIPHERAL;

        dma_init(hi2c->dma_tx_channel, &hi2c->dma_init_struct);

        i2c_init(hi2c->i2cx, I2C_FSMODE_DUTY_2_1, OLED_IIC_SPEED);
        i2c_own_address1_set(hi2c->i2cx, I2C_ADDRESS_MODE_7BIT, SSD1306_ADDR);
    }
}

/**
 * @brief OLED 初始化
 * @param None
 * @return None
 */
void _init(void)
{

    hi2cx.i2cx = OLED_IIC;
    i2c_config(&hi2cx);

    ssd1306_send_command(0xAE);           //--turn off oled panel
    ssd1306_send_command(0x00);           //---set low column address
    ssd1306_send_command(0x10);           //---set high column address
    ssd1306_send_command(0x40);           //--set start line address  Set Mapping RAM
                                          // Display Start Line (0x00~0x3F)
    ssd1306_send_command(0x81);           //--set contrast control register
    ssd1306_send_command(0xCF);           // Set SEG Output Current Brightness
    ssd1306_send_command(0xA1);           //--Set SEG/Column Mapping     0xa0左右反置 0xa1正常显示
    ssd1306_send_command(0xC8);           // Set COM/Row Scan Direction   0xc0上下反置 0xc8正常显示
    ssd1306_send_command(NORMAL_DISPLAY); //--set normal display
    ssd1306_send_command(0xA8);           //--set multiplex ratio(1 to 64)
    ssd1306_send_command(0x3f);           //--1/64 duty
    ssd1306_send_command(0xD3);           //-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
    ssd1306_send_command(0x00);           //-not offset
    ssd1306_send_command(0xd5);           //--set display clock divide ratio/oscillator frequency
    ssd1306_send_command(0x80);           //--set divide ratio, Set Clock as 100 Frames/Sec
    ssd1306_send_command(0xD9);           //--set pre-charge period
    ssd1306_send_command(0xF1);           // Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
    ssd1306_send_command(0xDA);           //--set com pins hardware configuration
    ssd1306_send_command(0x12);
    ssd1306_send_command(0xDB); //--set vcomh
    ssd1306_send_command(0x30); // Set VCOM Deselect Level
    ssd1306_send_command(0x20); //-Set Page Addressing Mode (0x00/0x01/0x02)
    ssd1306_send_command(0x02); //
    ssd1306_send_command(0x8D); //--set Charge Pump enable/disable
    ssd1306_send_command(0x14); //--set(0x10) disable
    ssd1306.clear();
    ssd1306_send_command(0xAF);
    printf("OLED init END!\n");
}

/**
 * @brief 正常显示/反色显示
 * @param ColorMode
 *        - NORMAL_COLOR 正常显示
 *        - INVERT_COLOR 反色显示
 * @return None
 */
static void _set_color_mode(DisplayColor_enum_t ColorMode)
{
    switch (ColorMode)
    {
    case NORMAL_COLOR:
        ssd1306_send_command(0xA6); // 正常显示
        break;
    case INVERT_COLOR:
        ssd1306_send_command(0xA7); // 反色显示
        break;
    default:
        break;
    }
}

/**
 * @brief 打开显示
 * @param None
 * @return None
 */
static void _On(void) { ssd1306_send_command(DISPLAY_ON); }

/**
 * @brief 关闭显示
 * @param None
 * @return None
 */
static void _Off(void) { ssd1306_send_command(DISPLAY_OFF); }

/**
 * @brief 显示旋转
 * @param DEG
 *        - DISPLAY_0_DEG   正常显示
 *        - DISPLAY_180_DEG 屏幕旋转180度
 *        - DISPLAY_MIRROR  镜像显示
 * @return None
 */
static void _set_display_deg(Display_DEG_enum_t DEG)
{
    switch (DEG)
    {
        // 正常显示
    case DISPLAY_0_DEG:
        ssd1306_send_command(0xC8); // 上下正常显示
        ssd1306_send_command(0xA1); // 水平正常显示
        break;
    case DISPLAY_180_DEG:
        // 旋转180度
        ssd1306_send_command(0xC0); // 上下翻转显示
        ssd1306_send_command(0xA0); // 水平反转显示
        break;
    case DISPLAY_MIRROR:
        // 镜像显示
        ssd1306_send_command(0xC8); // 上下正常显示
        ssd1306_send_command(0xA0); // 水平反转显示
        break;
    default:
        break;
    }
}

void _clear(void) { memset(ssd1306_buffer, 0x00, SSD1306_BUFFER_SIZE); }

void _fill(void)
{
    u8 temp = ssd1306.color == BLACK ? 0x00 : 0xFF;
    memset(ssd1306_buffer, temp, SSD1306_BUFFER_SIZE);
}

/**
 * @brief 写像素,写 x与y 的交汇点，屏幕的其它像素保持原来的值
 *        - 如：要不第一行第一列的像素点亮
 * @param x x坐标 左右，x代表第几列 0~128
 * @param y y坐标 上下，y代表第几行，一个8页 0~64
 * @return None
 */
static void _draw_pixel(u8 x, u8 y)
{
    SSD1306_Color_enum_t color = ssd1306.color;

    if (ssd1306.inverted)
    {
        color = (SSD1306_Color_enum_t)!color;
    }

    if (color == WHITE)
    {
        ssd1306_buffer[x + (y / 8) * width()] |= 1 << (y % 8);
    }
    else
    {
        ssd1306_buffer[x + (y / 8) * width()] &= ~(1 << (y % 8));
    }
}

/**
 * @brief 发送 SSD1306 的命令，格式：地址（0x78)+模式（0x40）+要发送的命令
 * @param
 * @return
 */
static void ssd1306_send_command(u8 command)
{
    i2c_memory_write_dma(&hi2cx, I2C_MEM_ADDR_WIDIH_8, SSD1306_ADDR, SSD1306_CMD,
                         &command, 1, IIC_TIMEOUT);
}

/**
 * @brief 写数据
 * @param data 要发送数据的地址
 * @param size 要发送数据的大小
 * @return None
 */
static void ssd1306_send_data(u8 *data, u16 size)
{
    i2c_memory_write_dma(&hi2cx, I2C_MEM_ADDR_WIDIH_8, SSD1306_ADDR, SSD1306_DATA,
                         data, size, IIC_TIMEOUT);
}

/**
 * @brief 更新屏幕内容，
 */
static void _update_screen(void)
{
    u8 i;
    for (i = 0; i < 8; i++)
    {
        ssd1306_send_command(0xB0 + i);
        ssd1306_send_command(0x00); //---set low column address
        ssd1306_send_command(0x10); //---set high column address
        ssd1306_send_data(&ssd1306_buffer[SSD1306_WIDTH * i], width());
    }
}

static void _set_color(SSD1306_Color_enum_t color) { ssd1306.color = color; }

static void _set_inverted(bool value) { ssd1306.inverted = value; }

static void _draw_line(i16 x0, i16 y0, i16 x1, i16 y1)
{
    i16 steep = abs(y1 - y0) > abs(x1 - x0);
    if (steep)
    {
        swap_num(&x0, &y0, sizeof(i16));
        swap_num(&x1, &y1, sizeof(i16));
    }

    if (x0 > x1)
    {
        swap_num(&x0, &x1, sizeof(i16));
        swap_num(&y0, &y1, sizeof(i16));
    }

    i16 dx, dy;
    dx = x1 - x0;
    dy = abs(y1 - y0);

    i16 err = dx / 2;
    i16 ystep;

    if (y0 < y1)
    {
        ystep = 1;
    }
    else
    {
        ystep = -1;
    }

    for (; x0 <= x1; x0++)
    {
        if (steep)
        {
            ssd1306.draw_pixel(y0, x0);
        }
        else
        {
            ssd1306.draw_pixel(x0, y0);
        }
        err -= dy;
        if (err < 0)
        {
            y0 += ystep;
            err += dx;
        }
    }
}

/**
 * @brief 绘制水平线
 * @param x 水平方向 0~128
 * @param y 垂直方向 0~64
 * @param length
 * @return None
 */
static void _draw_horizontal_line(i16 x, i16 y, i16 length)
{
    if (y < 0 || y >= height())
    {
        return;
    }

    if (x < 0)
    {
        length += x;
        x = 0;
    }

    if ((x + length) > width())
    {
        length = (width() - x);
    }

    if (length <= 0)
    {
        return;
    }

    uint8_t *bufferPtr = ssd1306_buffer;
    bufferPtr += (y >> 3) * width();
    bufferPtr += x;

    uint8_t drawBit = 1 << (y & 7);

    switch (ssd1306.color)
    {
    case WHITE:
        while (length--)
        {
            *bufferPtr++ |= drawBit;
        };
        break;
    case BLACK:
        drawBit = ~drawBit;
        while (length--)
        {
            *bufferPtr++ &= drawBit;
        };
        break;
    case INVERSE:
        while (length--)
        {
            *bufferPtr++ ^= drawBit;
        };
        break;
    }
}

/**
 * @brief 绘制垂直线
 * @param x 水平方向 0~128
 * @param y 垂直方向 0~64
 * @param length
 * @return None
 */
static void _draw_vertical_line(i16 x, i16 y, i16 length)
{
    if (x < 0 || x >= width())
        return;

    if (y < 0)
    {
        length += y;
        y = 0;
    }

    if ((y + length) > height())
    {
        length = (height() - y);
    }

    if (length <= 0)
        return;

    uint8_t yOffset = y & 7;
    uint8_t drawBit;
    uint8_t *bufferPtr = ssd1306_buffer;

    bufferPtr += (y >> 3) * width();
    bufferPtr += x;

    if (yOffset)
    {
        yOffset = 8 - yOffset;
        drawBit = ~(0xFF >> (yOffset));

        if (length < yOffset)
        {
            drawBit &= (0xFF >> (yOffset - length));
        }

        switch (ssd1306.color)
        {
        case WHITE:
            *bufferPtr |= drawBit;
            break;
        case BLACK:
            *bufferPtr &= ~drawBit;
            break;
        case INVERSE:
            *bufferPtr ^= drawBit;
            break;
        }

        if (length < yOffset)
            return;

        length -= yOffset;
        bufferPtr += width();
    }

    if (length >= 8)
    {
        switch (ssd1306.color)
        {
        case WHITE:
        case BLACK:
            drawBit = (ssd1306.color == WHITE) ? 0xFF : 0x00;
            do
            {
                *bufferPtr = drawBit;
                bufferPtr += width();
                length -= 8;
            } while (length >= 8);
            break;
        case INVERSE:
            do
            {
                *bufferPtr = ~(*bufferPtr);
                bufferPtr += width();
                length -= 8;
            } while (length >= 8);
            break;
        }
    }

    if (length > 0)
    {
        drawBit = (1 << (length & 7)) - 1;
        switch (ssd1306.color)
        {
        case WHITE:
            *bufferPtr |= drawBit;
            break;
        case BLACK:
            *bufferPtr &= ~drawBit;
            break;
        case INVERSE:
            *bufferPtr ^= drawBit;
            break;
        }
    }
}

/**
 * @brief 绘制矩形
 * @param x
 * @param y
 * @param width
 * @param height
 * @return None
 */
static void _draw_rect(i16 x, i16 y, i16 width, i16 height)
{
    _draw_horizontal_line(x, y, width);
    _draw_vertical_line(x, y, height);
    _draw_vertical_line(x + width - 1, y, height);
    _draw_horizontal_line(x, y + height - 1, width);
}

void _draw_fill_rect(i16 xMove, i16 yMove, i16 width, i16 height)
{
    for (i16 x = xMove; x < xMove + width; x++)
    {
        _draw_vertical_line(x, yMove, height);
    }
}

static void _draw_triangle(u16 x1, u16 y1, u16 x2, u16 y2, u16 x3, u16 y3)
{
    /* Draw lines */
    _draw_line(x1, y1, x2, y2);
    _draw_line(x2, y2, x3, y3);
    _draw_line(x3, y3, x1, y1);
}

static void _draw_fill_triangle(u16 x1, u16 y1, u16 x2, u16 y2, u16 x3, u16 y3)
{
    int16_t deltax = 0, deltay = 0, x = 0, y = 0, xinc1 = 0, xinc2 = 0,
            yinc1 = 0, yinc2 = 0, den = 0, num = 0, numadd = 0, numpixels = 0,
            curpixel = 0;

    deltax = abs(x2 - x1);
    deltay = abs(y2 - y1);
    x = x1;
    y = y1;

    if (x2 >= x1)
    {
        xinc1 = 1;
        xinc2 = 1;
    }
    else
    {
        xinc1 = -1;
        xinc2 = -1;
    }

    if (y2 >= y1)
    {
        yinc1 = 1;
        yinc2 = 1;
    }
    else
    {
        yinc1 = -1;
        yinc2 = -1;
    }

    if (deltax >= deltay)
    {
        xinc1 = 0;
        yinc2 = 0;
        den = deltax;
        num = deltax / 2;
        numadd = deltay;
        numpixels = deltax;
    }
    else
    {
        xinc2 = 0;
        yinc1 = 0;
        den = deltay;
        num = deltay / 2;
        numadd = deltax;
        numpixels = deltay;
    }

    for (curpixel = 0; curpixel <= numpixels; curpixel++)
    {
        _draw_line(x, y, x3, y3);

        num += numadd;
        if (num >= den)
        {
            num -= den;
            x += xinc1;
            y += yinc1;
        }
        x += xinc2;
        y += yinc2;
    }
}

static void _draw_circle(i16 x0, i16 y0, i16 radius)
{
    i16 x = 0, y = radius;
    i16 dp = 1 - radius;
    do
    {
        if (dp < 0)
            dp = dp + 2 * (++x) + 3;
        else
            dp = dp + 2 * (++x) - 2 * (--y) + 5;

        _draw_pixel(x0 + x, y0 + y); // For the 8 octants
        _draw_pixel(x0 - x, y0 + y);
        _draw_pixel(x0 + x, y0 - y);
        _draw_pixel(x0 - x, y0 - y);
        _draw_pixel(x0 + y, y0 + x);
        _draw_pixel(x0 - y, y0 + x);
        _draw_pixel(x0 + y, y0 - x);
        _draw_pixel(x0 - y, y0 - x);

    } while (x < y);

    _draw_pixel(x0 + radius, y0);
    _draw_pixel(x0, y0 + radius);
    _draw_pixel(x0 - radius, y0);
    _draw_pixel(x0, y0 - radius);
}

static void _draw_fill_circle(i16 x0, i16 y0, i16 radius)
{
    i16 x = 0, y = radius;
    i16 dp = 1 - radius;
    do
    {
        if (dp < 0)
        {
            dp = dp + 2 * (++x) + 3;
        }
        else
        {
            dp = dp + 2 * (++x) - 2 * (--y) + 5;
        }

        _draw_horizontal_line(x0 - x, y0 - y, 2 * x);
        _draw_horizontal_line(x0 - x, y0 + y, 2 * x);
        _draw_horizontal_line(x0 - y, y0 - x, 2 * y);
        _draw_horizontal_line(x0 - y, y0 + x, 2 * y);

    } while (x < y);
    _draw_horizontal_line(x0 - radius, y0, 2 * radius);
}