/**
 * @file bsp_uart.h
 * @brief BSP UART 底层接口
 *
 * 提供平台特定的 UART 原始操作接口
 * HAL 层将基于这些接口进行封装
 */

#ifndef BSP_UART_H
#define BSP_UART_H

#include <stddef.h>
#include <stdint.h>


#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief BSP UART 初始化
 * @param uart_id UART 实例ID
 * @param baud 波特率
 * @return 0 表示成功，非0 表示失败
 */
int bsp_uart_init(uint8_t uart_id, uint32_t baud);

/**
 * @brief BSP UART 反初始化
 * @param uart_id UART 实例ID
 */
void bsp_uart_deinit(uint8_t uart_id);

/**
 * @brief BSP UART 发送数据
 * @param uart_id UART 实例ID
 * @param data 数据缓冲区
 * @param size 数据大小
 * @return 实际发送的字节数
 */
int bsp_uart_send(uint8_t uart_id, const uint8_t *data, size_t size);

/**
 * @brief BSP UART 接收数据
 * @param uart_id UART 实例ID
 * @param data 数据缓冲区
 * @param size 缓冲区大小
 * @return 实际接收的字节数
 */
int bsp_uart_receive(uint8_t uart_id, uint8_t *data, size_t size);

/**
 * @brief BSP UART 查询可接收字节数
 * @param uart_id UART 实例ID
 * @return 可接收的字节数
 */
int bsp_uart_available(uint8_t uart_id);

#ifdef __cplusplus
}
#endif

#endif /* BSP_UART_H */
