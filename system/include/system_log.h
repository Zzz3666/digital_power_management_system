/**
 * @file system_log.h
 * @brief 系统日志模块 - 可移植的日志接口
 *
 * 提供统一的日志接口，支持：
 * - 多级别日志（DEBUG, INFO, WARN, ERROR）
 * - 编译时开关
 * - 运行时级别控制
 * - 多种输出后端（UART, RTT, SWO, Semihosting等）
 */

#ifndef SYSTEM_LOG_H
#define SYSTEM_LOG_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ============================================================================
 * 日志级别定义
 * ============================================================================
 */

typedef enum {
  LOG_LEVEL_NONE = 0,  /**< 禁用所有日志 */
  LOG_LEVEL_ERROR = 1, /**< 仅错误 */
  LOG_LEVEL_WARN = 2,  /**< 警告及以上 */
  LOG_LEVEL_INFO = 3,  /**< 信息及以上 */
  LOG_LEVEL_DEBUG = 4  /**< 调试及以上 */
} log_level_t;

/* ============================================================================
 * 编译时配置（通过 CMake 或编译器标志设置）
 * ============================================================================
 */

/* 默认日志级别（可在 CMakeLists.txt 中覆盖） */
#ifndef CONFIG_LOG_DEFAULT_LEVEL
#define CONFIG_LOG_DEFAULT_LEVEL LOG_LEVEL_INFO
#endif

/* 是否启用颜色输出（仅适用于支持 ANSI 的终端） */
#ifndef CONFIG_LOG_ENABLE_COLOR
#define CONFIG_LOG_ENABLE_COLOR 0
#endif

/* 最大日志消息长度 */
#ifndef CONFIG_LOG_MAX_MESSAGE_LENGTH
#define CONFIG_LOG_MAX_MESSAGE_LENGTH 256
#endif

/* ============================================================================
 * 日志宏定义
 * ============================================================================
 */

#if CONFIG_LOG_DEFAULT_LEVEL >= LOG_LEVEL_ERROR
#define LOG_ERROR(fmt, ...)                                                    \
  system_log_print(LOG_LEVEL_ERROR, fmt, ##__VA_ARGS__)
#else
#define LOG_ERROR(fmt, ...) ((void)0)
#endif

#if CONFIG_LOG_DEFAULT_LEVEL >= LOG_LEVEL_WARN
#define LOG_WARN(fmt, ...) system_log_print(LOG_LEVEL_WARN, fmt, ##__VA_ARGS__)
#else
#define LOG_WARN(fmt, ...) ((void)0)
#endif

#if CONFIG_LOG_DEFAULT_LEVEL >= LOG_LEVEL_INFO
#define LOG_INFO(fmt, ...) system_log_print(LOG_LEVEL_INFO, fmt, ##__VA_ARGS__)
#else
#define LOG_INFO(fmt, ...) ((void)0)
#endif

#if CONFIG_LOG_DEFAULT_LEVEL >= LOG_LEVEL_DEBUG
#define LOG_DEBUG(fmt, ...)                                                    \
  system_log_print(LOG_LEVEL_DEBUG, fmt, ##__VA_ARGS__)
#else
#define LOG_DEBUG(fmt, ...) ((void)0)
#endif

/* 无条件打印（用于关键信息） */
#define LOG_PRINT(fmt, ...) system_log_print(LOG_LEVEL_INFO, fmt, ##__VA_ARGS__)

/* ============================================================================
 * 核心 API
 * ============================================================================
 */

/**
 * @brief 初始化日志系统
 * @return 0 表示成功，负数表示失败
 */
int system_log_init(void);

/**
 * @brief 反初始化日志系统
 */
void system_log_deinit(void);

/**
 * @brief 打印日志消息
 * @param level 日志级别
 * @param fmt 格式字符串
 * @param ... 可变参数
 */
void system_log_print(log_level_t level, const char *fmt, ...);

/**
 * @brief 设置运行时日志级别
 * @param level 新的日志级别
 */
void system_log_set_level(log_level_t level);

/**
 * @brief 获取当前日志级别
 * @return 当前日志级别
 */
log_level_t system_log_get_level(void);

/**
 * @brief 将日志级别转换为字符串
 * @param level 日志级别
 * @return 级别字符串
 */
const char *system_log_level_to_string(log_level_t level);

/* ============================================================================
 * 平台特定的输出后端（需要在 platform_log.c 中实现）
 * ============================================================================
 */

/**
 * @brief 平台特定的字符输出函数
 * @param ch 要输出的字符
 *
 * 注意：此函数由日志系统内部调用，用户不应直接调用
 */
void system_log_platform_putchar(char ch);

/**
 * @brief 平台特定的字符串输出函数（可选，用于优化性能）
 * @param str 要输出的字符串
 * @param len 字符串长度
 *
 * 如果未实现，系统将使用 system_log_platform_putchar 逐个字符输出
 */
void system_log_platform_putstr(const char *str, uint16_t len);

#ifdef __cplusplus
}
#endif

#endif /* SYSTEM_LOG_H */
