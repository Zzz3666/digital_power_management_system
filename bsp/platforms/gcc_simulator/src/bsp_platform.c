#include "bsp_platform.h"
#include <stdio.h>

int bsp_platform_init(void) {
  printf("GCC Simulator platform initialized.\n");
  /* 在这里添加平台特定的初始化代码 */
  return 0;
}

void bsp_platform_deinit(void) {
  printf("GCC Simulator platform deinitialized.\n");
  /* 在这里添加平台特定的清理代码 */
}
