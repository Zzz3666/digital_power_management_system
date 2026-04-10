/**
 * @file hal_pwm.h
 * @brief PWM (脉冲宽度调制) HAL 接口
 */

#ifndef HAL_PWM_H
#define HAL_PWM_H

#include <stdint.h>

/**
 * @brief PWM 实例
 */
typedef enum { PWM_0 = 0, PWM_1, PWM_2, PWM_3 } hal_pwm_t;

/**
 * @brief PWM 配置结构体
 */
typedef struct {
  uint32_t frequency; /**< 频率 (Hz) */
  uint8_t duty_cycle; /**< 占空比 (0-100%) */
} hal_pwm_config_t;

/**
 * @brief 初始化 PWM
 * @param pwm PWM 实例
 * @param config PWM 配置
 * @return 0 表示成功，非0 表示失败
 */
int hal_pwm_init(hal_pwm_t pwm, const hal_pwm_config_t *config);

/**
 * @brief 反初始化 PWM
 * @param pwm PWM 实例
 */
void hal_pwm_deinit(hal_pwm_t pwm);

/**
 * @brief 启动 PWM
 * @param pwm PWM 实例
 * @return 0 表示成功，非0 表示失败
 */
int hal_pwm_start(hal_pwm_t pwm);

/**
 * @brief 停止 PWM
 * @param pwm PWM 实例
 * @return 0 表示成功，非0 表示失败
 */
int hal_pwm_stop(hal_pwm_t pwm);

/**
 * @brief 设置 PWM 频率
 * @param pwm PWM 实例
 * @param frequency 频率 (Hz)
 * @return 0 表示成功，非0 表示失败
 */
int hal_pwm_set_frequency(hal_pwm_t pwm, uint32_t frequency);

/**
 * @brief 设置 PWM 占空比
 * @param pwm PWM 实例
 * @param duty_cycle 占空比 (0-100%)
 * @return 0 表示成功，非0 表示失败
 */
int hal_pwm_set_duty_cycle(hal_pwm_t pwm, uint8_t duty_cycle);

#endif /* HAL_PWM_H */
