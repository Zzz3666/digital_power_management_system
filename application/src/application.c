/**
 * @file application.c
 * @brief Application Layer Implementation
 */

#include "application.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


/* ============================================================================
 * Internal State
 * ============================================================================
 */

static app_state_t g_app_state = {0};
static power_mode_t g_current_power_mode = POWER_MODE_NORMAL;
static power_channel_info_t g_channels[4] = {0};
static power_mode_callback_t g_power_mode_callback = NULL;
static channel_status_callback_t g_channel_status_callback = NULL;

/* ============================================================================
 * Helper Functions
 * ============================================================================
 */

static void initialize_channels(void) {
  for (int i = 0; i < 4; i++) {
    g_channels[i].channel_id = i + 1;
    g_channels[i].voltage_mv = 0.0f;
    g_channels[i].current_ma = 0.0f;
    g_channels[i].power_mw = 0.0f;
    g_channels[i].temperature_c = 25.0f;
    g_channels[i].status = CHANNEL_STATUS_OFF;
    g_channels[i].uptime_seconds = 0;
  }
}

static const char *power_mode_to_string(power_mode_t mode) {
  switch (mode) {
  case POWER_MODE_NORMAL:
    return "NORMAL";
  case POWER_MODE_ECO:
    return "ECO";
  case POWER_MODE_PERFORMANCE:
    return "PERFORMANCE";
  case POWER_MODE_STANDBY:
    return "STANDBY";
  case POWER_MODE_SHUTDOWN:
    return "SHUTDOWN";
  default:
    return "UNKNOWN";
  }
}

static const char *channel_status_to_string(channel_status_t status) {
  switch (status) {
  case CHANNEL_STATUS_OFF:
    return "OFF";
  case CHANNEL_STATUS_ON:
    return "ON";
  case CHANNEL_STATUS_FAULT:
    return "FAULT";
  case CHANNEL_STATUS_OVERLOAD:
    return "OVERLOAD";
  default:
    return "UNKNOWN";
  }
}

/* ============================================================================
 * Application Lifecycle Implementation
 * ============================================================================
 */

int app_init(const app_config_t *config) {
  printf("[APP] Initializing application...\n");

  if (!config || !config->app_name) {
    printf("[APP] Error: Invalid configuration\n");
    return -1;
  }

  /* Save configuration */
  g_app_state.config = *config;
  g_app_state.initialized = 1;
  g_app_state.running = 0;
  g_app_state.start_time = (uint32_t)clock();

  /* Initialize power channels */
  initialize_channels();

  printf("[APP] Application '%s' v%u.%u.%u initialized\n", config->app_name,
         config->version_major, config->version_minor, config->version_patch);

  return 0;
}

int app_start(void) {
  if (!g_app_state.initialized) {
    printf("[APP] Error: Application not initialized\n");
    return -1;
  }

  printf("[APP] Starting application...\n");
  g_app_state.running = 1;

  /* Create application tasks */
  task_handle_t monitor_task = app_create_monitor_task();
  task_handle_t control_task = app_create_control_task();
  task_handle_t comm_task = app_create_communication_task();

  if (monitor_task && control_task && comm_task) {
    printf("[APP] All application tasks created successfully\n");
  } else {
    printf("[APP] Warning: Some tasks failed to create\n");
  }

  printf("[APP] Application started\n");
  return 0;
}

int app_stop(void) {
  if (!g_app_state.running) {
    printf("[APP] Warning: Application is not running\n");
    return -1;
  }

  printf("[APP] Stopping application...\n");
  g_app_state.running = 0;

  /* Cleanup tasks would go here */

  printf("[APP] Application stopped\n");
  return 0;
}

int app_deinit(void) {
  if (g_app_state.running) {
    app_stop();
  }

  printf("[APP] Deinitializing application...\n");

  /* Reset state */
  memset(&g_app_state, 0, sizeof(app_state_t));
  g_current_power_mode = POWER_MODE_NORMAL;
  g_power_mode_callback = NULL;
  g_channel_status_callback = NULL;

  printf("[APP] Application deinitialized\n");
  return 0;
}

int app_is_initialized(void) { return g_app_state.initialized; }

int app_is_running(void) { return g_app_state.running; }

/* ============================================================================
 * Power Management Control Implementation
 * ============================================================================
 */

int app_set_power_mode(power_mode_t mode) {
  if (!g_app_state.initialized) {
    return -1;
  }

  power_mode_t old_mode = g_current_power_mode;
  g_current_power_mode = mode;

  printf("[APP-POWER] Mode changed: %s -> %s\n", power_mode_to_string(old_mode),
         power_mode_to_string(mode));

  /* Call registered callback */
  if (g_power_mode_callback) {
    g_power_mode_callback(old_mode, mode);
  }

  return 0;
}

power_mode_t app_get_power_mode(void) { return g_current_power_mode; }

int app_enable_channel(uint8_t channel_id) {
  if (!g_app_state.initialized || channel_id < 1 || channel_id > 4) {
    return -1;
  }

  uint8_t idx = channel_id - 1;
  channel_status_t old_status = g_channels[idx].status;

  g_channels[idx].status = CHANNEL_STATUS_ON;
  g_channels[idx].voltage_mv = 12000.0f; /* 12V */
  g_channels[idx].current_ma = 500.0f;   /* 500mA */
  g_channels[idx].power_mw = 6000.0f;    /* 6W */

  printf("[APP-CHANNEL] Channel %d enabled\n", channel_id);

  /* Call registered callback */
  if (g_channel_status_callback) {
    g_channel_status_callback(channel_id, old_status, CHANNEL_STATUS_ON);
  }

  return 0;
}

int app_disable_channel(uint8_t channel_id) {
  if (!g_app_state.initialized || channel_id < 1 || channel_id > 4) {
    return -1;
  }

  uint8_t idx = channel_id - 1;
  channel_status_t old_status = g_channels[idx].status;

  g_channels[idx].status = CHANNEL_STATUS_OFF;
  g_channels[idx].voltage_mv = 0.0f;
  g_channels[idx].current_ma = 0.0f;
  g_channels[idx].power_mw = 0.0f;

  printf("[APP-CHANNEL] Channel %d disabled\n", channel_id);

  /* Call registered callback */
  if (g_channel_status_callback) {
    g_channel_status_callback(channel_id, old_status, CHANNEL_STATUS_OFF);
  }

  return 0;
}

int app_get_channel_info(uint8_t channel_id, power_channel_info_t *info) {
  if (!g_app_state.initialized || !info || channel_id < 1 || channel_id > 4) {
    return -1;
  }

  *info = g_channels[channel_id - 1];
  return 0;
}

int app_get_all_channels(power_channel_info_t *info, uint8_t max_channels) {
  if (!g_app_state.initialized || !info || max_channels == 0) {
    return -1;
  }

  uint8_t count = (max_channels < 4) ? max_channels : 4;
  memcpy(info, g_channels, count * sizeof(power_channel_info_t));

  return count;
}

/* ============================================================================
 * Monitoring and Diagnostics Implementation
 * ============================================================================
 */

int app_get_system_health(system_health_t *health) {
  if (!g_app_state.initialized || !health) {
    return -1;
  }

  health->cpu_load_percent = 15.5f;
  health->memory_usage_percent = 42.3f;
  health->total_uptime_seconds = app_get_uptime_ms() / 1000;
  health->error_count = 0;
  health->warning_count = 2;

  return 0;
}

uint32_t app_get_uptime_ms(void) {
  if (!g_app_state.initialized) {
    return 0;
  }

  return (uint32_t)clock() - g_app_state.start_time;
}

int app_generate_status_report(char *buffer, size_t buffer_size) {
  if (!g_app_state.initialized || !buffer || buffer_size == 0) {
    return -1;
  }

  int offset = 0;
  offset += snprintf(buffer + offset, buffer_size - offset, "{\n");
  offset +=
      snprintf(buffer + offset, buffer_size - offset,
               "  \"application\": \"%s\",\n", g_app_state.config.app_name);
  offset += snprintf(
      buffer + offset, buffer_size - offset, "  \"version\": \"%u.%u.%u\",\n",
      g_app_state.config.version_major, g_app_state.config.version_minor,
      g_app_state.config.version_patch);
  offset += snprintf(buffer + offset, buffer_size - offset,
                     "  \"power_mode\": \"%s\",\n",
                     power_mode_to_string(g_current_power_mode));
  offset += snprintf(buffer + offset, buffer_size - offset,
                     "  \"uptime_ms\": %u,\n", app_get_uptime_ms());
  offset +=
      snprintf(buffer + offset, buffer_size - offset, "  \"channels\": [\n");

  for (int i = 0; i < 4; i++) {
    offset += snprintf(buffer + offset, buffer_size - offset, "    {\n");
    offset += snprintf(buffer + offset, buffer_size - offset,
                       "      \"id\": %d,\n", g_channels[i].channel_id);
    offset += snprintf(buffer + offset, buffer_size - offset,
                       "      \"status\": \"%s\",\n",
                       channel_status_to_string(g_channels[i].status));
    offset +=
        snprintf(buffer + offset, buffer_size - offset,
                 "      \"voltage_mv\": %.1f,\n", g_channels[i].voltage_mv);
    offset +=
        snprintf(buffer + offset, buffer_size - offset,
                 "      \"current_ma\": %.1f,\n", g_channels[i].current_ma);
    offset += snprintf(buffer + offset, buffer_size - offset,
                       "      \"power_mw\": %.1f\n", g_channels[i].power_mw);
    offset += snprintf(buffer + offset, buffer_size - offset, "    }%s\n",
                       (i < 3) ? "," : "");
  }

  offset += snprintf(buffer + offset, buffer_size - offset, "  ]\n");
  offset += snprintf(buffer + offset, buffer_size - offset, "}\n");

  return offset;
}

void app_log(int level, const char *format, ...) {
  if (!g_app_state.initialized || !format) {
    return;
  }

  const char *level_str[] = {"DEBUG", "INFO", "WARN", "ERROR"};
  if (level < 0 || level > 3) {
    level = 1;
  }

  printf("[%s] ", level_str[level]);

  va_list args;
  va_start(args, format);
  vprintf(format, args);
  va_end(args);

  printf("\n");
}

/* ============================================================================
 * Task Management Implementation
 * ============================================================================
 */

static void monitor_task_func(void *arg) {
  (void)arg;
  printf("[TASK-MONITOR] Monitoring task running\n");

  while (g_app_state.running) {
    /* Monitor system health */
    system_health_t health;
    if (app_get_system_health(&health) == 0) {
      printf("[TASK-MONITOR] CPU: %.1f%%, Memory: %.1f%%\n",
             health.cpu_load_percent, health.memory_usage_percent);
    }

    system_delay_ms(5000);
  }
}

static void control_task_func(void *arg) {
  (void)arg;
  printf("[TASK-CONTROL] Control task running\n");

  while (g_app_state.running) {
    /* Check power channels */
    for (int i = 1; i <= 4; i++) {
      power_channel_info_t info;
      if (app_get_channel_info(i, &info) == 0) {
        if (info.status == CHANNEL_STATUS_ON) {
          printf("[TASK-CONTROL] Channel %d: %.1fV, %.1fmA\n", info.channel_id,
                 info.voltage_mv / 1000.0f, info.current_ma);
        }
      }
    }

    system_delay_ms(2000);
  }
}

static void communication_task_func(void *arg) {
  (void)arg;
  printf("[TASK-COMM] Communication task running\n");

  while (g_app_state.running) {
    /* Generate and send status report */
    char report[1024];
    int len = app_generate_status_report(report, sizeof(report));
    if (len > 0) {
      printf("[TASK-COMM] Status report generated (%d bytes)\n", len);
    }

    system_delay_ms(10000);
  }
}

task_handle_t app_create_monitor_task(void) {
  return system_task_create("MonitorTask", monitor_task_func, NULL, 2048,
                            TASK_PRIORITY_MEDIUM);
}

task_handle_t app_create_control_task(void) {
  return system_task_create("ControlTask", control_task_func, NULL, 2048,
                            TASK_PRIORITY_HIGH);
}

task_handle_t app_create_communication_task(void) {
  return system_task_create("CommTask", communication_task_func, NULL, 2048,
                            TASK_PRIORITY_LOW);
}

/* ============================================================================
 * Callback Registration Implementation
 * ============================================================================
 */

int app_register_power_mode_callback(power_mode_callback_t callback) {
  if (!callback) {
    return -1;
  }

  g_power_mode_callback = callback;
  printf("[APP] Power mode callback registered\n");
  return 0;
}

int app_register_channel_status_callback(channel_status_callback_t callback) {
  if (!callback) {
    return -1;
  }

  g_channel_status_callback = callback;
  printf("[APP] Channel status callback registered\n");
  return 0;
}
