#include "ec11.h"
#include "gpio_init.h"

/**
 * @brief Construct a new Ec 1 1:: Ec 1 1 object
 * 
 */
Ec11::Ec11()
{
    Gpio::init(EC11_SW_GPIOX, EC11_SW_PIN, GPIO_PULL_NONE, EC11_SW_CRM_PERIPH_CLOCK, GPIO_MODE_MUX, GPIO_OUTPUT_OPEN_DRAIN);
}
