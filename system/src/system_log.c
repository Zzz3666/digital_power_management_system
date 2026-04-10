/**
 * @file system_log.c
 * @brief 系统日志模块实现
 */

#include "system_log.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

/* ============================================================================
 * 内部状态
 * ============================================================================
 */

static log_level_t g_current_log_level = CONFIG_LOG_DEFAULT_LEVEL;
static int g_log_initialized = 0;

/* ============================================================================
 * 颜色代码（可选）
 * ============================================================================
 */

#if CONFIG_LOG_ENABLE_COLOR
#define COLOR_RESET "\033[0m"
#define COLOR_ERROR "\033[31m" /* 红色 */
#define COLOR_WARN "\033[33m"  /* 黄色 */
#define COLOR_INFO "\033[32m"  /* 绿色 */
#define COLOR_DEBUG "\033[36m" /* 青色 */
#else
#define COLOR_RESET ""
#define COLOR_ERROR ""
#define COLOR_WARN ""
#define COLOR_INFO ""
#define COLOR_DEBUG ""
#endif

/* ============================================================================
 * 辅助函数
 * ============================================================================
 */

const char *system_log_level_to_string(log_level_t level) {
  switch (level) {
  case LOG_LEVEL_ERROR:
    return "ERROR";
  case LOG_LEVEL_WARN:
    return "WARN";
  case LOG_LEVEL_INFO:
    return "INFO";
  case LOG_LEVEL_DEBUG:
    return "DEBUG";
  default:
    return "UNKNOWN";
  }
}

static const char *get_level_color(log_level_t level) {
#if CONFIG_LOG_ENABLE_COLOR
  switch (level) {
  case LOG_LEVEL_ERROR:
    return COLOR_ERROR;
  case LOG_LEVEL_WARN:
    return COLOR_WARN;
  case LOG_LEVEL_INFO:
    return COLOR_INFO;
  case LOG_LEVEL_DEBUG:
    return COLOR_DEBUG;
  default:
    return COLOR_RESET;
  }
#else
  (void)level;
  return "";
#endif
}

/* ============================================================================
 * 平台特定的输出实现（GCC Simulator）
 * ============================================================================
 */

#ifdef BSP_PLATFORM_GCC_SIMULATOR
/* 在模拟器平台上使用 printf */

void system_log_platform_putchar(char ch) { putchar(ch); }

void system_log_platform_putstr(const char *str, uint16_t len) {
  /* 使用 fwrite 提高性能 */
  fwrite(str, 1, len, stdout);
}

#else
/*
 * 其他平台需要在此处实现
 * 例如：
 * - STM32: 通过 UART 发送
 * - ESP32: 通过 esp_log_write
 * - Nordic: 通过 RTT 或 UART
 */

/* 弱符号定义，允许平台覆盖 */
#ifndef __weak
#ifdef __GNUC__
#define __weak __attribute__((weak))
#elif defined(_MSC_VER)
#define __weak
#else
#define __weak
#endif
#endif

__weak void system_log_platform_putchar(char ch) {
  /* 默认空实现，避免链接错误 */
  (void)ch;
}

__weak void system_log_platform_putstr(const char *str, uint16_t len) {
  /* 默认使用逐个字符输出 */
  for (uint16_t i = 0; i < len; i++) {
    system_log_platform_putchar(str[i]);
  }
}
#endif

/* ============================================================================
 * 核心 API 实现
 * ============================================================================
 */

int system_log_init(void) {
  g_current_log_level = CONFIG_LOG_DEFAULT_LEVEL;
  g_log_initialized = 1;

  LOG_INFO("Log system initialized (level=%s)",
           system_log_level_to_string(g_current_log_level));

  return 0;
}

void system_log_deinit(void) {
  g_log_initialized = 0;
  LOG_INFO("Log system deinitialized");
}

void system_log_print(log_level_t level, const char *fmt, ...) {
  /* 检查日志级别 */
  if (!g_log_initialized || level > g_current_log_level) {
    return;
  }

  /* 检查参数有效性 */
  if (!fmt) {
    return;
  }

  /* 格式化消息 */
  char buffer[CONFIG_LOG_MAX_MESSAGE_LENGTH];
  va_list args;
  va_start(args, fmt);
  int len = vsnprintf(buffer, sizeof(buffer), fmt, args);
  va_end(args);

  /* 检查格式化是否成功 */
  if (len < 0 || len >= (int)sizeof(buffer)) {
    return;
  }

  /* 输出日志前缀 */
  const char *color = get_level_color(level);
  const char *level_str = system_log_level_to_string(level);

  /* 构建完整消息：[LEVEL] message\n */
  char prefix[32];
  int prefix_len = snprintf(prefix, sizeof(prefix), "%s[%s]%s ", color,
                            level_str, COLOR_RESET);

  /* 输出前缀 */
  system_log_platform_putstr(prefix, (uint16_t)prefix_len);

  /* 输出消息内容 */
  system_log_platform_putstr(buffer, (uint16_t)len);

  /* 输出换行符 */
  system_log_platform_putchar('\n');
}

void system_log_set_level(log_level_t level) {
  if (level >= LOG_LEVEL_NONE && level <= LOG_LEVEL_DEBUG) {
    g_current_log_level = level;
    LOG_INFO("Log level changed to %s", system_log_level_to_string(level));
  }
}

log_level_t system_log_get_level(void) { return g_current_log_level; }
