/**
 * @file hal_adc.h
 * @brief ADC (模数转换器) HAL 接口
 */

#ifndef HAL_ADC_H
#define HAL_ADC_H

#include <stdint.h>

/**
 * @brief ADC 实例
 */
typedef enum { ADC_0 = 0, ADC_1, ADC_2 } hal_adc_t;

/**
 * @brief ADC 分辨率
 */
typedef enum {
  ADC_RESOLUTION_8BIT = 8,
  ADC_RESOLUTION_10BIT = 10,
  ADC_RESOLUTION_12BIT = 12,
  ADC_RESOLUTION_16BIT = 16
} hal_adc_resolution_t;

/**
 * @brief ADC 配置结构体
 */
typedef struct {
  hal_adc_resolution_t resolution; /**< 分辨率 */
} hal_adc_config_t;

/**
 * @brief 初始化 ADC
 * @param adc ADC 实例
 * @param config ADC 配置
 * @return 0 表示成功，非0 表示失败
 */
int hal_adc_init(hal_adc_t adc, const hal_adc_config_t *config);

/**
 * @brief 反初始化 ADC
 * @param adc ADC 实例
 */
void hal_adc_deinit(hal_adc_t adc);

/**
 * @brief 读取 ADC 通道值
 * @param adc ADC 实例
 * @param channel 通道号
 * @param value 读取的值
 * @return 0 表示成功，非0 表示失败
 */
int hal_adc_read(hal_adc_t adc, uint8_t channel, uint16_t *value);

/**
 * @brief 读取 ADC 通道电压值 (mV)
 * @param adc ADC 实例
 * @param channel 通道号
 * @param voltage_mv 电压值 (毫伏)
 * @return 0 表示成功，非0 表示失败
 */
int hal_adc_read_voltage(hal_adc_t adc, uint8_t channel, uint32_t *voltage_mv);

#endif /* HAL_ADC_H */
