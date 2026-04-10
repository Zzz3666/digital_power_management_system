/**
 * @file hal_uart.c
 * @brief HAL UART 实现 - 封装 BSP 接口
 */

#include "hal_uart.h"
#include "bsp_uart.h"

int hal_uart_init(hal_uart_t uart, const hal_uart_config_t *config) {
  if (!config) {
    return -1;
  }

  /* 调用 BSP 底层接口 */
  return bsp_uart_init((uint8_t)uart, (uint32_t)config->baud);
}

void hal_uart_deinit(hal_uart_t uart) { bsp_uart_deinit((uint8_t)uart); }

int hal_uart_send(hal_uart_t uart, const uint8_t *data, size_t size,
                  uint32_t timeout) {
  /* 注意：BSP 层目前不支持超时，HAL 层可以在此添加超时控制 */
  (void)timeout; /* 暂时忽略超时参数 */
  return bsp_uart_send((uint8_t)uart, data, size);
}

int hal_uart_receive(hal_uart_t uart, uint8_t *data, size_t size,
                     uint32_t timeout) {
  /* 注意：BSP 层目前不支持超时，HAL 层可以在此添加超时控制 */
  (void)timeout; /* 暂时忽略超时参数 */
  return bsp_uart_receive((uint8_t)uart, data, size);
}

int hal_uart_available(hal_uart_t uart) {
  return bsp_uart_available((uint8_t)uart);
}
