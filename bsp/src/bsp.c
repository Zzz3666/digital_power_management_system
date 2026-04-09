#include "bsp.h"
#include "bsp_platform.h"
#include <stdio.h>

int init_bsp(void) {
  printf("Initializing BSP...\n");

  /* 调用平台特定的初始化 */
  if (bsp_platform_init() != 0) {
    printf("BSP platform initialization failed!\n");
    return -1;
  }

  printf("BSP initialized successfully.\n");
  return 0;
}

void deinit_bsp(void) {
  printf("Deinitializing BSP...\n");
  bsp_platform_deinit();
  printf("BSP deinitialized.\n");
}
