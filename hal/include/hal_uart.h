/**
 * @file hal_uart.h
 * @brief UART (通用异步收发器) HAL 接口
 */

#ifndef HAL_UART_H
#define HAL_UART_H

#include <stddef.h>
#include <stdint.h>


/**
 * @brief UART 实例
 */
typedef enum { UART_0 = 0, UART_1, UART_2, UART_3 } hal_uart_t;

/**
 * @brief UART 波特率
 */
typedef enum {
  UART_BAUD_9600 = 9600,
  UART_BAUD_19200 = 19200,
  UART_BAUD_38400 = 38400,
  UART_BAUD_57600 = 57600,
  UART_BAUD_115200 = 115200,
  UART_BAUD_230400 = 230400,
  UART_BAUD_460800 = 460800,
  UART_BAUD_921600 = 921600
} hal_uart_baud_t;

/**
 * @brief UART 数据位
 */
typedef enum {
  UART_DATA_7BITS = 7,
  UART_DATA_8BITS = 8,
  UART_DATA_9BITS = 9
} hal_uart_data_t;

/**
 * @brief UART 停止位
 */
typedef enum { UART_STOP_1BIT = 0, UART_STOP_2BITS } hal_uart_stop_t;

/**
 * @brief UART 校验位
 */
typedef enum {
  UART_PARITY_NONE = 0,
  UART_PARITY_EVEN,
  UART_PARITY_ODD
} hal_uart_parity_t;

/**
 * @brief UART 配置结构体
 */
typedef struct {
  hal_uart_baud_t baud;      /**< 波特率 */
  hal_uart_data_t data_bits; /**< 数据位 */
  hal_uart_stop_t stop_bits; /**< 停止位 */
  hal_uart_parity_t parity;  /**< 校验位 */
} hal_uart_config_t;

/**
 * @brief 初始化 UART
 * @param uart UART 实例
 * @param config UART 配置
 * @return 0 表示成功，非0 表示失败
 */
int hal_uart_init(hal_uart_t uart, const hal_uart_config_t *config);

/**
 * @brief 反初始化 UART
 * @param uart UART 实例
 */
void hal_uart_deinit(hal_uart_t uart);

/**
 * @brief 发送数据
 * @param uart UART 实例
 * @param data 数据缓冲区
 * @param size 数据大小
 * @param timeout 超时时间 (ms)
 * @return 实际发送的字节数
 */
int hal_uart_send(hal_uart_t uart, const uint8_t *data, size_t size,
                  uint32_t timeout);

/**
 * @brief 接收数据
 * @param uart UART 实例
 * @param data 数据缓冲区
 * @param size 缓冲区大小
 * @param timeout 超时时间 (ms)
 * @return 实际接收的字节数
 */
int hal_uart_receive(hal_uart_t uart, uint8_t *data, size_t size,
                     uint32_t timeout);

/**
 * @brief 查询可接收的字节数
 * @param uart UART 实例
 * @return 可接收的字节数
 */
int hal_uart_available(hal_uart_t uart);

#endif /* HAL_UART_H */
