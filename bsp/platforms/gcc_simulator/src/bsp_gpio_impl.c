/**
 * @file bsp_gpio_impl.c
 * @brief GCC Simulator 平台的 GPIO 实现
 */

#include "bsp_gpio.h"
#include <stdio.h>
#include <string.h>

/* 模拟器中的 GPIO 状态存储 */
static uint16_t gpio_output_state[16] = {0}; /* 16个端口 */
static uint16_t gpio_mode[16] = {0};         /* 0=输入, 1=输出 */

int bsp_gpio_init(uint8_t port, uint8_t pin, uint8_t mode) {
  if (port >= 16 || pin >= 16) {
    return -1; /* 参数错误 */
  }

  printf("[BSP-GPIO] Init: Port=%d, Pin=%d, Mode=%s\n", port, pin,
         mode == 0 ? "INPUT" : "OUTPUT");

  gpio_mode[port] &= ~(1 << pin);
  gpio_mode[port] |= (mode << pin);

  return 0;
}

void bsp_gpio_deinit(uint8_t port, uint8_t pin) {
  if (port >= 16 || pin >= 16) {
    return;
  }

  printf("[BSP-GPIO] Deinit: Port=%d, Pin=%d\n", port, pin);
  gpio_mode[port] &= ~(1 << pin);
  gpio_output_state[port] &= ~(1 << pin);
}

void bsp_gpio_write(uint8_t port, uint8_t pin, uint8_t state) {
  if (port >= 16 || pin >= 16) {
    return;
  }

  if (state) {
    gpio_output_state[port] |= (1 << pin);
  } else {
    gpio_output_state[port] &= ~(1 << pin);
  }

  printf("[BSP-GPIO] Write: Port=%d, Pin=%d, State=%d\n", port, pin, state);
}

uint8_t bsp_gpio_read(uint8_t port, uint8_t pin) {
  if (port >= 16 || pin >= 16) {
    return 0;
  }

  uint8_t state = (gpio_output_state[port] >> pin) & 0x01;
  printf("[BSP-GPIO] Read: Port=%d, Pin=%d, State=%d\n", port, pin, state);

  return state;
}

void bsp_gpio_toggle(uint8_t port, uint8_t pin) {
  if (port >= 16 || pin >= 16) {
    return;
  }

  gpio_output_state[port] ^= (1 << pin);

  uint8_t state = (gpio_output_state[port] >> pin) & 0x01;
  printf("[BSP-GPIO] Toggle: Port=%d, Pin=%d, New State=%d\n", port, pin,
         state);
}
