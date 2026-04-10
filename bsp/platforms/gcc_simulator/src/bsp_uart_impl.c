/**
 * @file bsp_uart_impl.c
 * @brief GCC Simulator 平台的 UART 实现
 */

#include "bsp_uart.h"
#include <stdio.h>
#include <string.h>

/* 模拟器中的 UART 缓冲区 */
#define UART_BUFFER_SIZE 256
static uint8_t uart_rx_buffer[4][UART_BUFFER_SIZE]; /* 4个UART实例 */
static int uart_rx_count[4] = {0};

int bsp_uart_init(uint8_t uart_id, uint32_t baud) {
  if (uart_id >= 4) {
    return -1; /* 参数错误 */
  }

  printf("[BSP-UART] Init: UART%d, Baud=%lu\n", uart_id, baud);
  uart_rx_count[uart_id] = 0;
  memset(uart_rx_buffer[uart_id], 0, UART_BUFFER_SIZE);

  return 0;
}

void bsp_uart_deinit(uint8_t uart_id) {
  if (uart_id >= 4) {
    return;
  }

  printf("[BSP-UART] Deinit: UART%d\n", uart_id);
  uart_rx_count[uart_id] = 0;
}

int bsp_uart_send(uint8_t uart_id, const uint8_t *data, size_t size) {
  if (uart_id >= 4 || !data || size == 0) {
    return -1;
  }

  /* 模拟器中直接打印到控制台 */
  printf("[BSP-UART%d TX] ", uart_id);
  for (size_t i = 0; i < size; i++) {
    putchar(data[i]);
  }
  printf("\n");

  return (int)size;
}

int bsp_uart_receive(uint8_t uart_id, uint8_t *data, size_t size) {
  if (uart_id >= 4 || !data || size == 0) {
    return -1;
  }

  /* 模拟器中从缓冲区读取 */
  int count = (size < (size_t)uart_rx_count[uart_id]) ? (int)size
                                                      : uart_rx_count[uart_id];

  memcpy(data, uart_rx_buffer[uart_id], count);

  /* 移动剩余数据 */
  if (count < uart_rx_count[uart_id]) {
    memmove(uart_rx_buffer[uart_id], uart_rx_buffer[uart_id] + count,
            uart_rx_count[uart_id] - count);
  }
  uart_rx_count[uart_id] -= count;

  printf("[BSP-UART%d RX] Received %d bytes\n", uart_id, count);

  return count;
}

int bsp_uart_available(uint8_t uart_id) {
  if (uart_id >= 4) {
    return 0;
  }

  return uart_rx_count[uart_id];
}
