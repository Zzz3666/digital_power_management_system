/**
 * @file hal_spi.h
 * @brief SPI (串行外设接口) HAL 接口
 */

#ifndef HAL_SPI_H
#define HAL_SPI_H

#include <stddef.h>
#include <stdint.h>


/**
 * @brief SPI 实例
 */
typedef enum { SPI_0 = 0, SPI_1, SPI_2 } hal_spi_t;

/**
 * @brief SPI 模式
 */
typedef enum {
  SPI_MODE_0 = 0, /**< CPOL=0, CPHA=0 */
  SPI_MODE_1,     /**< CPOL=0, CPHA=1 */
  SPI_MODE_2,     /**< CPOL=1, CPHA=0 */
  SPI_MODE_3      /**< CPOL=1, CPHA=1 */
} hal_spi_mode_t;

/**
 * @brief SPI 配置结构体
 */
typedef struct {
  hal_spi_mode_t mode;  /**< SPI 模式 */
  uint32_t clock_speed; /**< 时钟速度 (Hz) */
  uint8_t data_size;    /**< 数据位数 (8 或 16) */
} hal_spi_config_t;

/**
 * @brief 初始化 SPI
 * @param spi SPI 实例
 * @param config SPI 配置
 * @return 0 表示成功，非0 表示失败
 */
int hal_spi_init(hal_spi_t spi, const hal_spi_config_t *config);

/**
 * @brief 反初始化 SPI
 * @param spi SPI 实例
 */
void hal_spi_deinit(hal_spi_t spi);

/**
 * @brief 传输数据 (同时发送和接收)
 * @param spi SPI 实例
 * @param tx_data 发送数据缓冲区
 * @param rx_data 接收数据缓冲区
 * @param size 数据大小
 * @param timeout 超时时间 (ms)
 * @return 0 表示成功，非0 表示失败
 */
int hal_spi_transfer(hal_spi_t spi, const uint8_t *tx_data, uint8_t *rx_data,
                     size_t size, uint32_t timeout);

/**
 * @brief 仅发送数据
 * @param spi SPI 实例
 * @param data 发送数据缓冲区
 * @param size 数据大小
 * @param timeout 超时时间 (ms)
 * @return 0 表示成功，非0 表示失败
 */
int hal_spi_send(hal_spi_t spi, const uint8_t *data, size_t size,
                 uint32_t timeout);

/**
 * @brief 仅接收数据
 * @param spi SPI 实例
 * @param data 接收数据缓冲区
 * @param size 数据大小
 * @param timeout 超时时间 (ms)
 * @return 0 表示成功，非0 表示失败
 */
int hal_spi_receive(hal_spi_t spi, uint8_t *data, size_t size,
                    uint32_t timeout);

#endif /* HAL_SPI_H */
