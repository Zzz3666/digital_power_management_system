/**
 * @file hal_gpio.h
 * @brief GPIO (通用输入输出) HAL 接口
 */

#ifndef HAL_GPIO_H
#define HAL_GPIO_H

#include <stdint.h>

/**
 * @brief GPIO 引脚定义
 */
typedef enum {
  GPIO_PIN_0 = 0,
  GPIO_PIN_1,
  GPIO_PIN_2,
  GPIO_PIN_3,
  GPIO_PIN_4,
  GPIO_PIN_5,
  GPIO_PIN_6,
  GPIO_PIN_7,
  GPIO_PIN_8,
  GPIO_PIN_9,
  GPIO_PIN_10,
  GPIO_PIN_11,
  GPIO_PIN_12,
  GPIO_PIN_13,
  GPIO_PIN_14,
  GPIO_PIN_15,
  GPIO_PIN_ALL = 0xFFFF
} hal_gpio_pin_t;

/**
 * @brief GPIO 模式
 */
typedef enum {
  GPIO_MODE_INPUT,     /**< 输入模式 */
  GPIO_MODE_OUTPUT,    /**< 输出模式 */
  GPIO_MODE_ALTERNATE, /**< 复用功能 */
  GPIO_MODE_ANALOG     /**< 模拟模式 */
} hal_gpio_mode_t;

/**
 * @brief GPIO 上下拉配置
 */
typedef enum {
  GPIO_NOPULL,  /**< 无上下拉 */
  GPIO_PULLUP,  /**< 上拉 */
  GPIO_PULLDOWN /**< 下拉 */
} hal_gpio_pull_t;

/**
 * @brief GPIO 输出速度
 */
typedef enum {
  GPIO_SPEED_LOW,      /**< 低速 */
  GPIO_SPEED_MEDIUM,   /**< 中速 */
  GPIO_SPEED_HIGH,     /**< 高速 */
  GPIO_SPEED_VERY_HIGH /**< 超高速 */
} hal_gpio_speed_t;

/**
 * @brief GPIO 初始化结构体
 */
typedef struct {
  hal_gpio_pin_t pin;     /**< 引脚号 */
  hal_gpio_mode_t mode;   /**< 模式 */
  hal_gpio_pull_t pull;   /**< 上下拉 */
  hal_gpio_speed_t speed; /**< 速度 */
} hal_gpio_config_t;

/**
 * @brief 初始化 GPIO
 * @param port GPIO 端口号
 * @param config GPIO 配置
 * @return 0 表示成功，非0 表示失败
 */
int hal_gpio_init(uint8_t port, const hal_gpio_config_t *config);

/**
 * @brief 反初始化 GPIO
 * @param port GPIO 端口号
 * @param pin 引脚号
 */
void hal_gpio_deinit(uint8_t port, hal_gpio_pin_t pin);

/**
 * @brief 设置 GPIO 引脚状态
 * @param port GPIO 端口号
 * @param pin 引脚号
 * @param state 状态 (0=低电平, 1=高电平)
 */
void hal_gpio_write(uint8_t port, hal_gpio_pin_t pin, uint8_t state);

/**
 * @brief 读取 GPIO 引脚状态
 * @param port GPIO 端口号
 * @param pin 引脚号
 * @return 引脚状态 (0=低电平, 1=高电平)
 */
uint8_t hal_gpio_read(uint8_t port, hal_gpio_pin_t pin);

/**
 * @brief 切换 GPIO 引脚状态
 * @param port GPIO 端口号
 * @param pin 引脚号
 */
void hal_gpio_toggle(uint8_t port, hal_gpio_pin_t pin);

#endif /* HAL_GPIO_H */
