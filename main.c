#include "bsp.h"
#include <stdio.h>


int main() {
  /* 初始化 BSP */
  if (init_bsp() != 0) {
    printf("Failed to initialize BSP!\n");
    return -1;
  }

  printf("Hello, Digital Power Management System!\n");

  /* 反初始化 BSP */
  deinit_bsp();

  return 0;
}
