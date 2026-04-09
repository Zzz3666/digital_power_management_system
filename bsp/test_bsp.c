/**
 * BSP 库功能测试
 *
 * 这个文件演示如何使用 BSP 库的基本功能
 */

#include "bsp.h"
#include <stdio.h>


int main() {
  printf("=== BSP Library Test ===\n\n");

  // 测试 BSP 初始化
  printf("1. Testing BSP initialization...\n");
  int result = init_bsp();
  if (result == 0) {
    printf("   ✓ BSP initialized successfully\n");
  } else {
    printf("   ✗ BSP initialization failed with code: %d\n", result);
    return 1;
  }

  printf("\n2. BSP is running...\n");
  // 在这里可以添加更多的 BSP 功能测试

  printf("\n3. Testing BSP deinitialization...\n");
  deinit_bsp();
  printf("   ✓ BSP deinitialized successfully\n");

  printf("\n=== All tests passed! ===\n");
  return 0;
}
