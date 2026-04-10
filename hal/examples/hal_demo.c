/**
 * @file hal_demo.c
 * @brief HAL 库使用示例
 *
 * 演示如何使用 HAL 层的各种接口
 */

#include "hal.h"
#include "hal_adc.h"
#include "hal_gpio.h"
#include "hal_i2c.h"
#include "hal_pwm.h"
#include "hal_timer.h"
#include "hal_uart.h"
#include <stdio.h>


/* 定时器回调函数示例 */
void timer_callback_example(void *arg) {
  printf("[Timer] Callback triggered! Arg: %s\n", (char *)arg);
}

/* GPIO 示例 */
void gpio_example(void) {
  printf("\n=== GPIO Example ===\n");

  hal_gpio_config_t config = {.pin = GPIO_PIN_5,
                              .mode = GPIO_MODE_OUTPUT,
                              .pull = GPIO_NOPULL,
                              .speed = GPIO_SPEED_MEDIUM};

  /* 注意：这是示例代码，实际使用时需要正确的端口号 */
  printf("GPIO configuration initialized\n");
  printf("Pin: GPIO_PIN_5, Mode: OUTPUT\n");

  /* 模拟 GPIO 操作 */
  printf("Writing HIGH to GPIO_PIN_5\n");
  printf("Writing LOW to GPIO_PIN_5\n");
  printf("Toggling GPIO_PIN_5\n");
}

/* UART 示例 */
void uart_example(void) {
  printf("\n=== UART Example ===\n");

  hal_uart_config_t config = {.baud = UART_BAUD_115200,
                              .data_bits = UART_DATA_8BITS,
                              .stop_bits = UART_STOP_1BIT,
                              .parity = UART_PARITY_NONE};

  printf("UART configuration:\n");
  printf("  Baud: 115200\n");
  printf("  Data bits: 8\n");
  printf("  Stop bits: 1\n");
  printf("  Parity: None\n");

  /* 发送数据示例 */
  const char *message = "Hello from UART!\n";
  printf("Sending: %s", message);
}

/* I2C 示例 */
void i2c_example(void) {
  printf("\n=== I2C Example ===\n");

  hal_i2c_config_t config = {.speed = I2C_SPEED_STANDARD};

  printf("I2C configuration:\n");
  printf("  Speed: 100kHz (Standard)\n");
  printf("  Device address: 0x68\n");

  /* 模拟寄存器读写 */
  printf("Writing to register 0x00: 0x55\n");
  printf("Reading from register 0x00: 0x55\n");
}

/* ADC 示例 */
void adc_example(void) {
  printf("\n=== ADC Example ===\n");

  hal_adc_config_t config = {.resolution = ADC_RESOLUTION_12BIT};

  printf("ADC configuration:\n");
  printf("  Resolution: 12-bit\n");

  /* 模拟 ADC 读取 */
  uint16_t raw_value = 2048;
  uint32_t voltage_mv = 1650;

  printf("Channel 0 reading:\n");
  printf("  Raw value: %u\n", raw_value);
  printf("  Voltage: %lu mV\n", voltage_mv);
}

/* Timer 示例 */
void timer_example(void) {
  printf("\n=== Timer Example ===\n");

  hal_timer_config_t config = {.mode = TIMER_MODE_PERIODIC,
                               .period_ms = 1000,
                               .callback = timer_callback_example,
                               .callback_arg = "Periodic Timer"};

  printf("Timer configuration:\n");
  printf("  Mode: Periodic\n");
  printf("  Period: 1000 ms\n");
  printf("  Callback: timer_callback_example\n");

  /* 演示延时功能 */
  printf("Delaying for 100ms...\n");
  hal_delay_ms(100);
  printf("Delay completed\n");
}

/* PWM 示例 */
void pwm_example(void) {
  printf("\n=== PWM Example ===\n");

  hal_pwm_config_t config = {.frequency = 1000, .duty_cycle = 50};

  printf("PWM configuration:\n");
  printf("  Frequency: 1000 Hz\n");
  printf("  Duty cycle: 50%%\n");

  /* 动态调整占空比 */
  printf("Changing duty cycle to 75%%\n");
  printf("Changing duty cycle to 25%%\n");
}

int main() {
  printf("========================================\n");
  printf("  HAL Library Demo\n");
  printf("  Version: %s\n", hal_get_version());
  printf("========================================\n");

  /* 初始化 HAL */
  if (hal_init() != HAL_OK) {
    printf("Failed to initialize HAL!\n");
    return -1;
  }

  /* 运行各个模块的示例 */
  gpio_example();
  uart_example();
  i2c_example();
  adc_example();
  timer_example();
  pwm_example();

  /* 反初始化 HAL */
  hal_deinit();

  printf("\n========================================\n");
  printf("  Demo completed successfully!\n");
  printf("========================================\n");

  return 0;
}
