/**
 * @file hal.c
 * @brief HAL 层核心实现
 */

#include "hal.h"
#include <stdint.h>
#include <stdio.h>
#include <time.h>


#define HAL_VERSION "1.0.0"

hal_status_t hal_init(void) {
  printf("HAL layer initializing...\n");

  /* 在这里可以添加各模块的初始化 */

  printf("HAL layer initialized successfully.\n");
  return HAL_OK;
}

void hal_deinit(void) {
  printf("HAL layer deinitializing...\n");

  /* 在这里可以添加各模块的反初始化 */

  printf("HAL layer deinitialized.\n");
}

const char *hal_get_version(void) { return HAL_VERSION; }

/* 毫秒延时实现 (平台相关，这里是模拟器实现) */
void hal_delay_ms(uint32_t delay_ms) {
  clock_t start = clock();
  clock_t end = start + (delay_ms * CLOCKS_PER_SEC / 1000);

  while (clock() < end) {
    /* 忙等待 */
  }
}

/* 微秒延时实现 */
void hal_delay_us(uint32_t delay_us) {
  /* 在模拟器中，微秒延时简化为毫秒延时 */
  if (delay_us >= 1000) {
    hal_delay_ms(delay_us / 1000);
  }
}
