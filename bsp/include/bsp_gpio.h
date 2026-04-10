/**
 * @file bsp_gpio.h
 * @brief BSP GPIO 底层接口
 *
 * 提供平台特定的 GPIO 原始操作接口
 * HAL 层将基于这些接口进行封装
 */

#ifndef BSP_GPIO_H
#define BSP_GPIO_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief BSP GPIO 初始化
 * @param port GPIO 端口号
 * @param pin 引脚号
 * @param mode 模式 (0=输入, 1=输出)
 * @return 0 表示成功，非0 表示失败
 */
int bsp_gpio_init(uint8_t port, uint8_t pin, uint8_t mode);

/**
 * @brief BSP GPIO 反初始化
 * @param port GPIO 端口号
 * @param pin 引脚号
 */
void bsp_gpio_deinit(uint8_t port, uint8_t pin);

/**
 * @brief BSP GPIO 写操作
 * @param port GPIO 端口号
 * @param pin 引脚号
 * @param state 状态 (0=低电平, 1=高电平)
 */
void bsp_gpio_write(uint8_t port, uint8_t pin, uint8_t state);

/**
 * @brief BSP GPIO 读操作
 * @param port GPIO 端口号
 * @param pin 引脚号
 * @return 引脚状态 (0=低电平, 1=高电平)
 */
uint8_t bsp_gpio_read(uint8_t port, uint8_t pin);

/**
 * @brief BSP GPIO 翻转
 * @param port GPIO 端口号
 * @param pin 引脚号
 */
void bsp_gpio_toggle(uint8_t port, uint8_t pin);

#ifdef __cplusplus
}
#endif

#endif /* BSP_GPIO_H */
