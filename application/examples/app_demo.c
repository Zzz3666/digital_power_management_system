/**
 * @file app_demo.c
 * @brief Application Layer Demonstration
 *
 * This demonstrates the complete application layer functionality,
 * showing power management control, monitoring, and diagnostics.
 */

#include "application.h"
#include "component.h"
#include "json_codec.h"
#include <stdio.h>
#include <stdlib.h>


/* ============================================================================
 * Callback Functions
 * ============================================================================
 */

static void on_power_mode_changed(power_mode_t old_mode,
                                  power_mode_t new_mode) {
  printf("[CALLBACK] Power mode changed: %d -> %d\n", old_mode, new_mode);
}

static void on_channel_status_changed(uint8_t channel_id,
                                      channel_status_t old_status,
                                      channel_status_t new_status) {
  printf("[CALLBACK] Channel %d status: %d -> %d\n", channel_id, old_status,
         new_status);
}

/* ============================================================================
 * Demo Functions
 * ============================================================================
 */

static void demo_application_lifecycle(void) {
  printf("\n=== Application Lifecycle Demo ===\n");

  /* Initialize application */
  app_config_t config;
  config.app_name = "DigitalPowerManager";
  config.version_major = 1;
  config.version_minor = 0;
  config.version_patch = 0;
  config.heap_size = 65536;
  config.task_stack_size = 2048;

  int ret = app_init(&config);
  if (ret != 0) {
    printf("Error: Failed to initialize application\n");
    return;
  }

  printf("Application initialized: %s\n", app_is_initialized() ? "YES" : "NO");

  /* Start application */
  ret = app_start();
  if (ret != 0) {
    printf("Error: Failed to start application\n");
    return;
  }

  printf("Application running: %s\n", app_is_running() ? "YES" : "NO");

  printf("Application uptime: %u ms\n", app_get_uptime_ms());
}

static void demo_power_management(void) {
  printf("\n=== Power Management Demo ===\n");

  /* Register callbacks */
  app_register_power_mode_callback(on_power_mode_changed);
  app_register_channel_status_callback(on_channel_status_changed);

  /* Set power mode */
  printf("\nSetting power modes...\n");
  app_set_power_mode(POWER_MODE_ECO);
  printf("Current mode: %d\n", app_get_power_mode());

  app_set_power_mode(POWER_MODE_PERFORMANCE);
  printf("Current mode: %d\n", app_get_power_mode());

  app_set_power_mode(POWER_MODE_NORMAL);
  printf("Current mode: %d\n", app_get_power_mode());

  /* Enable/disable channels */
  printf("\nEnabling channels...\n");
  app_enable_channel(1);
  app_enable_channel(2);
  app_enable_channel(3);

  printf("\nDisabling channel 2...\n");
  app_disable_channel(2);

  /* Get channel information */
  printf("\nChannel Information:\n");
  for (int i = 1; i <= 4; i++) {
    power_channel_info_t info;
    if (app_get_channel_info(i, &info) == 0) {
      printf("  Channel %d: Status=%s, V=%.1fV, I=%.1fmA, P=%.1fmW\n",
             info.channel_id,
             (info.status == CHANNEL_STATUS_ON)    ? "ON"
             : (info.status == CHANNEL_STATUS_OFF) ? "OFF"
                                                   : "FAULT",
             info.voltage_mv / 1000.0f, info.current_ma, info.power_mw);
    }
  }

  /* Get all channels */
  printf("\nAll Channels:\n");
  power_channel_info_t all_channels[4];
  int count = app_get_all_channels(all_channels, 4);
  printf("Retrieved %d channels\n", count);
}

static void demo_monitoring_diagnostics(void) {
  printf("\n=== Monitoring & Diagnostics Demo ===\n");

  /* Get system health */
  system_health_t health;
  if (app_get_system_health(&health) == 0) {
    printf("\nSystem Health:\n");
    printf("  CPU Load: %.1f%%\n", health.cpu_load_percent);
    printf("  Memory Usage: %.1f%%\n", health.memory_usage_percent);
    printf("  Uptime: %u seconds\n", health.total_uptime_seconds);
    printf("  Errors: %u\n", health.error_count);
    printf("  Warnings: %u\n", health.warning_count);
  }

  /* Generate status report */
  printf("\nGenerating status report...\n");
  char report[2048];
  int len = app_generate_status_report(report, sizeof(report));
  if (len > 0) {
    printf("\nStatus Report (%d bytes):\n%s\n", len, report);
  }

  /* Logging */
  printf("\nLogging Demo:\n");
  app_log(0, "This is a DEBUG message");
  app_log(1, "This is an INFO message");
  app_log(2, "This is a WARNING message");
  app_log(3, "This is an ERROR message");
}

static void demo_integration_with_component_layer(void) {
  printf("\n=== Integration with Component Layer Demo ===\n");

  /* Use JSON codec from Component layer */
  json_codec_config_t json_config;
  json_config.max_depth = 10;
  json_config.max_size = 2048;

  json_codec_init(&json_config);

  /* Create a comprehensive system report */
  json_object_t report = json_object_create();

  /* Add application info */
  json_object_add_string(report, "app_name", "DigitalPowerManager");
  json_object_add_string(report, "version", "1.0.0");

  /* Add power mode */
  power_mode_t mode = app_get_power_mode();
  const char *mode_str = (mode == POWER_MODE_NORMAL)        ? "NORMAL"
                         : (mode == POWER_MODE_ECO)         ? "ECO"
                         : (mode == POWER_MODE_PERFORMANCE) ? "PERFORMANCE"
                                                            : "UNKNOWN";
  json_object_add_string(report, "power_mode", mode_str);

  /* Add uptime */
  uint32_t uptime = app_get_uptime_ms();
  json_object_add_int(report, "uptime_ms", (int)uptime);

  /* Add channel information */
  for (int i = 1; i <= 4; i++) {
    power_channel_info_t info;
    if (app_get_channel_info(i, &info) == 0) {
      char key[32];
      snprintf(key, sizeof(key), "ch%d_status", i);
      const char *status_str =
          (info.status == CHANNEL_STATUS_ON) ? "ON" : "OFF";
      json_object_add_string(report, key, status_str);

      /* Add voltage as integer (mV) */
      snprintf(key, sizeof(key), "ch%d_voltage_mv", i);
      json_object_add_int(report, key, (int)info.voltage_mv);
    }
  }

  /* Encode to JSON string */
  char json_buffer[2048];
  int json_len = json_object_encode(report, json_buffer, sizeof(json_buffer));
  if (json_len > 0) {
    printf("\nComprehensive System Report:\n%s\n", json_buffer);
  }

  /* Cleanup */
  json_object_delete(report);
  json_codec_deinit();
}

/* ============================================================================
 * Main Function
 * ============================================================================
 */

int main(void) {
  printf("========================================\n");
  printf("  Application Layer Demonstration\n");
  printf("========================================\n\n");

  printf("Architecture:\n");
  printf("  Application -> System -> Component -> Driver -> HAL -> BSP -> "
         "Hardware\n\n");

  /* Demo 1: Application Lifecycle */
  demo_application_lifecycle();

  /* Demo 2: Power Management */
  demo_power_management();

  /* Demo 3: Monitoring & Diagnostics */
  demo_monitoring_diagnostics();

  /* Demo 4: Integration with Component Layer */
  demo_integration_with_component_layer();

  /* Stop and cleanup */
  printf("\n=== Application Shutdown ===\n");
  app_stop();
  app_deinit();

  printf("\n========================================\n");
  printf("  Demo completed successfully!\n");
  printf("========================================\n");

  return 0;
}
