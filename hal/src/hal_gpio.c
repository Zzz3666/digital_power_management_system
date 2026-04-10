/**
 * @file hal_gpio.c
 * @brief HAL GPIO 实现 - 封装 BSP 接口
 */

#include "hal_gpio.h"
#include "bsp_gpio.h"

int hal_gpio_init(uint8_t port, const hal_gpio_config_t *config) {
  if (!config) {
    return -1;
  }

  /* 调用 BSP 底层接口 */
  return bsp_gpio_init(port, (uint8_t)config->pin,
                       config->mode == GPIO_MODE_OUTPUT ? 1 : 0);
}

void hal_gpio_deinit(uint8_t port, hal_gpio_pin_t pin) {
  bsp_gpio_deinit(port, (uint8_t)pin);
}

void hal_gpio_write(uint8_t port, hal_gpio_pin_t pin, uint8_t state) {
  bsp_gpio_write(port, (uint8_t)pin, state);
}

uint8_t hal_gpio_read(uint8_t port, hal_gpio_pin_t pin) {
  return bsp_gpio_read(port, (uint8_t)pin);
}

void hal_gpio_toggle(uint8_t port, hal_gpio_pin_t pin) {
  bsp_gpio_toggle(port, (uint8_t)pin);
}
