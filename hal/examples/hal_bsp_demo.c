/**
 * @file hal_bsp_demo.c
 * @brief 演示 HAL 如何封装 BSP 接口
 *
 * 架构说明：
 * - BSP 层：提供底层平台特定的原始接口 (bsp_gpio_xxx, bsp_uart_xxx)
 * - HAL 层：封装 BSP 接口，提供统一的高级 API (hal_gpio_xxx, hal_uart_xxx)
 * - 应用层：使用 HAL 接口，与具体平台无关
 */

#include "bsp.h"
#include "hal.h"
#include <stdio.h>
#include <string.h>


void gpio_demo(void) {
  printf("\n=== GPIO Demo (HAL wraps BSP) ===\n");

  /* 应用层使用 HAL 接口 */
  hal_gpio_config_t config = {.pin = GPIO_PIN_5,
                              .mode = GPIO_MODE_OUTPUT,
                              .pull = GPIO_NOPULL,
                              .speed = GPIO_SPEED_MEDIUM};

  /* HAL 内部会调用 BSP 的 bsp_gpio_init() */
  printf("Calling hal_gpio_init() -> internally calls bsp_gpio_init()\n");
  hal_gpio_init(0, &config);

  /* HAL 内部会调用 BSP 的 bsp_gpio_write() */
  printf("Calling hal_gpio_write(HIGH) -> internally calls bsp_gpio_write()\n");
  hal_gpio_write(0, GPIO_PIN_5, 1);

  printf("Calling hal_gpio_write(LOW) -> internally calls bsp_gpio_write()\n");
  hal_gpio_write(0, GPIO_PIN_5, 0);

  printf("Calling hal_gpio_toggle() -> internally calls bsp_gpio_toggle()\n");
  hal_gpio_toggle(0, GPIO_PIN_5);

  hal_gpio_deinit(0, GPIO_PIN_5);
}

void uart_demo(void) {
  printf("\n=== UART Demo (HAL wraps BSP) ===\n");

  /* 应用层使用 HAL 接口 */
  hal_uart_config_t config = {.baud = UART_BAUD_115200,
                              .data_bits = UART_DATA_8BITS,
                              .stop_bits = UART_STOP_1BIT,
                              .parity = UART_PARITY_NONE};

  /* HAL 内部会调用 BSP 的 bsp_uart_init() */
  printf("Calling hal_uart_init() -> internally calls bsp_uart_init()\n");
  hal_uart_init(UART_0, &config);

  /* HAL 内部会调用 BSP 的 bsp_uart_send() */
  const char *msg = "Hello from HAL!";
  printf("Calling hal_uart_send() -> internally calls bsp_uart_send()\n");
  hal_uart_send(UART_0, (const uint8_t *)msg, strlen(msg), 100);

  hal_uart_deinit(UART_0);
}

int main() {
  printf("========================================\n");
  printf("  HAL + BSP Architecture Demo\n");
  printf("========================================\n");
  printf("\nArchitecture:\n");
  printf("  Application -> HAL -> BSP -> Hardware\n");
  printf("\nDependency:\n");
  printf("  HAL depends on BSP (not vice versa)\n");
  printf("========================================\n");

  /* 1. 初始化 BSP (底层平台) */
  printf("\n[Step 1] Initialize BSP (platform layer)\n");
  if (init_bsp() != 0) {
    printf("BSP initialization failed!\n");
    return -1;
  }

  /* 2. 初始化 HAL (硬件抽象层) */
  printf("\n[Step 2] Initialize HAL (abstraction layer)\n");
  if (hal_init() != HAL_OK) {
    printf("HAL initialization failed!\n");
    deinit_bsp();
    return -1;
  }

  /* 3. 使用 HAL 接口（内部调用 BSP）*/
  printf("\n[Step 3] Use HAL APIs (which wrap BSP functions)\n");
  gpio_demo();
  uart_demo();

  /* 4. 清理 */
  printf("\n[Step 4] Cleanup\n");
  hal_deinit();
  deinit_bsp();

  printf("\n========================================\n");
  printf("  Demo completed successfully!\n");
  printf("========================================\n");

  return 0;
}
