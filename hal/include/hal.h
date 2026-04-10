/**
 * @file hal.h
 * @brief HAL (Hardware Abstraction Layer) 主头文件
 *
 * 提供统一的硬件抽象接口，与具体平台无关
 */

#ifndef HAL_H
#define HAL_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

/* HAL 模块包含 */

/**
 * @brief HAL 状态码
 */
typedef enum {
  HAL_OK = 0,          /**< 成功 */
  HAL_ERROR = -1,      /**< 错误 */
  HAL_BUSY = -2,       /**< 忙 */
  HAL_TIMEOUT = -3,    /**< 超时 */
  HAL_UNSUPPORTED = -4 /**< 不支持 */
} hal_status_t;

/**
 * @brief 初始化 HAL 层
 * @return HAL_OK 表示成功，其他值表示失败
 */
hal_status_t hal_init(void);

/**
 * @brief 反初始化 HAL 层
 */
void hal_deinit(void);

/**
 * @brief 获取 HAL 版本信息
 * @return 版本字符串
 */
const char *hal_get_version(void);

#ifdef __cplusplus
}
#endif

#endif /* HAL_H */
