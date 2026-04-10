#include "application.h"
#include <stdio.h>

int main() {
  printf("========================================\n");
  printf("  Digital Power Management System\n");
  printf("========================================\n\n");

  /* 配置应用 */
  app_config_t config = {.app_name = "DigitalPowerManager",
                         .version_major = 1,
                         .version_minor = 0,
                         .version_patch = 0,
                         .heap_size = 8192,
                         .task_stack_size = 2048};

  /* 初始化应用层 */
  if (app_init(&config) != 0) {
    printf("Failed to initialize application!\n");
    return -1;
  }

  /* 启动应用 */
  if (app_start() != 0) {
    printf("Failed to start application!\n");
    app_deinit();
    return -1;
  }

  printf("\nApplication is running...\n");
  printf("Press Ctrl+C to stop.\n\n");

  /* 模拟应用运行（在实际系统中，这里会是RTOS调度器） */
  /* 由于我们使用模拟器，这里简单演示应用功能 */

  /* 设置电源模式 */
  app_set_power_mode(POWER_MODE_NORMAL);

  /* 使能通道 */
  app_enable_channel(1);
  app_enable_channel(2);

  /* 获取通道信息 */
  power_channel_info_t channel_info;
  if (app_get_channel_info(1, &channel_info) == 0) {
    printf("Channel 1: Status=%s, Voltage=%.1fmV, Current=%.1fmA\n",
           channel_info.status == CHANNEL_STATUS_ON ? "ON" : "OFF",
           channel_info.voltage_mv, channel_info.current_ma);
  }

  /* 生成状态报告 */
  char report[1024];
  int len = app_generate_status_report(report, sizeof(report));
  if (len > 0) {
    printf("\nStatus Report:\n%s\n", report);
  }

  /* 停止应用 */
  app_stop();

  /* 反初始化应用 */
  app_deinit();

  printf("\n========================================\n");
  printf("  Application exited successfully\n");
  printf("========================================\n");

  return 0;
}
