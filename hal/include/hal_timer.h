/**
 * @file hal_timer.h
 * @brief Timer (定时器) HAL 接口
 */

#ifndef HAL_TIMER_H
#define HAL_TIMER_H

#include <stdint.h>

/**
 * @brief Timer 实例
 */
typedef enum { TIMER_0 = 0, TIMER_1, TIMER_2, TIMER_3 } hal_timer_t;

/**
 * @brief Timer 模式
 */
typedef enum {
  TIMER_MODE_ONESHOT, /**< 单次模式 */
  TIMER_MODE_PERIODIC /**< 周期模式 */
} hal_timer_mode_t;

/**
 * @brief Timer 回调函数类型
 */
typedef void (*hal_timer_callback_t)(void *arg);

/**
 * @brief Timer 配置结构体
 */
typedef struct {
  hal_timer_mode_t mode;         /**< 模式 */
  uint32_t period_ms;            /**< 周期 (ms) */
  hal_timer_callback_t callback; /**< 回调函数 */
  void *callback_arg;            /**< 回调参数 */
} hal_timer_config_t;

/**
 * @brief 初始化 Timer
 * @param timer Timer 实例
 * @param config Timer 配置
 * @return 0 表示成功，非0 表示失败
 */
int hal_timer_init(hal_timer_t timer, const hal_timer_config_t *config);

/**
 * @brief 反初始化 Timer
 * @param timer Timer 实例
 */
void hal_timer_deinit(hal_timer_t timer);

/**
 * @brief 启动 Timer
 * @param timer Timer 实例
 * @return 0 表示成功，非0 表示失败
 */
int hal_timer_start(hal_timer_t timer);

/**
 * @brief 停止 Timer
 * @param timer Timer 实例
 * @return 0 表示成功，非0 表示失败
 */
int hal_timer_stop(hal_timer_t timer);

/**
 * @brief 获取当前计数值 (ms)
 * @param timer Timer 实例
 * @return 当前计数值
 */
uint32_t hal_timer_get_count(hal_timer_t timer);

/**
 * @brief 延时指定时间 (ms)
 * @param delay_ms 延时时间 (毫秒)
 */
void hal_delay_ms(uint32_t delay_ms);

/**
 * @brief 延时指定时间 (us)
 * @param delay_us 延时时间 (微秒)
 */
void hal_delay_us(uint32_t delay_us);

#endif /* HAL_TIMER_H */
