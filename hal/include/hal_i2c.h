/**
 * @file hal_i2c.h
 * @brief I2C (集成电路总线) HAL 接口
 */

#ifndef HAL_I2C_H
#define HAL_I2C_H

#include <stddef.h>
#include <stdint.h>


/**
 * @brief I2C 实例
 */
typedef enum { I2C_0 = 0, I2C_1, I2C_2 } hal_i2c_t;

/**
 * @brief I2C 速度模式
 */
typedef enum {
  I2C_SPEED_STANDARD = 100000,  /**< 标准模式 100kHz */
  I2C_SPEED_FAST = 400000,      /**< 快速模式 400kHz */
  I2C_SPEED_FAST_PLUS = 1000000 /**< 快速+模式 1MHz */
} hal_i2c_speed_t;

/**
 * @brief I2C 配置结构体
 */
typedef struct {
  hal_i2c_speed_t speed; /**< 速度模式 */
} hal_i2c_config_t;

/**
 * @brief 初始化 I2C
 * @param i2c I2C 实例
 * @param config I2C 配置
 * @return 0 表示成功，非0 表示失败
 */
int hal_i2c_init(hal_i2c_t i2c, const hal_i2c_config_t *config);

/**
 * @brief 反初始化 I2C
 * @param i2c I2C 实例
 */
void hal_i2c_deinit(hal_i2c_t i2c);

/**
 * @brief 向从设备写入数据
 * @param i2c I2C 实例
 * @param dev_addr 从设备地址 (7位)
 * @param data 数据缓冲区
 * @param size 数据大小
 * @param timeout 超时时间 (ms)
 * @return 0 表示成功，非0 表示失败
 */
int hal_i2c_write(hal_i2c_t i2c, uint16_t dev_addr, const uint8_t *data,
                  size_t size, uint32_t timeout);

/**
 * @brief 从从设备读取数据
 * @param i2c I2C 实例
 * @param dev_addr 从设备地址 (7位)
 * @param data 数据缓冲区
 * @param size 数据大小
 * @param timeout 超时时间 (ms)
 * @return 0 表示成功，非0 表示失败
 */
int hal_i2c_read(hal_i2c_t i2c, uint16_t dev_addr, uint8_t *data, size_t size,
                 uint32_t timeout);

/**
 * @brief 向从设备写入寄存器地址并读取数据
 * @param i2c I2C 实例
 * @param dev_addr 从设备地址 (7位)
 * @param reg_addr 寄存器地址
 * @param data 数据缓冲区
 * @param size 数据大小
 * @param timeout 超时时间 (ms)
 * @return 0 表示成功，非0 表示失败
 */
int hal_i2c_mem_read(hal_i2c_t i2c, uint16_t dev_addr, uint16_t reg_addr,
                     uint8_t *data, size_t size, uint32_t timeout);

/**
 * @brief 向从设备写入寄存器地址和数据
 * @param i2c I2C 实例
 * @param dev_addr 从设备地址 (7位)
 * @param reg_addr 寄存器地址
 * @param data 数据缓冲区
 * @param size 数据大小
 * @param timeout 超时时间 (ms)
 * @return 0 表示成功，非0 表示失败
 */
int hal_i2c_mem_write(hal_i2c_t i2c, uint16_t dev_addr, uint16_t reg_addr,
                      const uint8_t *data, size_t size, uint32_t timeout);

#endif /* HAL_I2C_H */
