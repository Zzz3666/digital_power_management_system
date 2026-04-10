/**
 * @file application.h
 * @brief Application Layer - Top level application framework
 * 
 * This layer provides the main application logic and user-facing features.
 * It depends on System layer for OS services and Component layer for utilities.
 * 
 * Architecture: Application -> System -> Component -> Driver -> HAL -> BSP
 */

#ifndef APPLICATION_H
#define APPLICATION_H

#include <stdint.h>
#include <stddef.h>
#include "system.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ============================================================================
 * Application Configuration
 * ============================================================================ */

/**
 * @brief Application configuration structure
 */
typedef struct {
    const char* app_name;              /**< Application name */
    uint32_t version_major;            /**< Major version number */
    uint32_t version_minor;            /**< Minor version number */
    uint32_t version_patch;            /**< Patch version number */
    size_t heap_size;                  /**< Heap size in bytes */
    uint32_t task_stack_size;          /**< Default task stack size */
} app_config_t;

/**
 * @brief Application state structure
 */
typedef struct {
    app_config_t config;               /**< Application configuration */
    int initialized;                   /**< Initialization flag */
    int running;                       /**< Running flag */
    uint32_t start_time;               /**< Application start time */
} app_state_t;

/* ============================================================================
 * Power Management Application Types
 * ============================================================================ */

/**
 * @brief Power management mode
 */
typedef enum {
    POWER_MODE_NORMAL = 0,             /**< Normal operation mode */
    POWER_MODE_ECO,                    /**< Energy saving mode */
    POWER_MODE_PERFORMANCE,            /**< High performance mode */
    POWER_MODE_STANDBY,                /**< Standby mode */
    POWER_MODE_SHUTDOWN,               /**< Shutdown mode */
} power_mode_t;

/**
 * @brief Power channel status
 */
typedef enum {
    CHANNEL_STATUS_OFF = 0,            /**< Channel is off */
    CHANNEL_STATUS_ON,                 /**< Channel is on */
    CHANNEL_STATUS_FAULT,              /**< Channel has fault */
    CHANNEL_STATUS_OVERLOAD,           /**< Channel is overloaded */
} channel_status_t;

/**
 * @brief Power channel information
 */
typedef struct {
    uint8_t channel_id;                /**< Channel ID (1-4) */
    float voltage_mv;                  /**< Voltage in millivolts */
    float current_ma;                  /**< Current in milliamps */
    float power_mw;                    /**< Power in milliwatts */
    float temperature_c;               /**< Temperature in Celsius */
    channel_status_t status;           /**< Channel status */
    uint32_t uptime_seconds;           /**< Channel uptime in seconds */
} power_channel_info_t;

/**
 * @brief System health status
 */
typedef struct {
    float cpu_load_percent;            /**< CPU load percentage */
    float memory_usage_percent;        /**< Memory usage percentage */
    uint32_t total_uptime_seconds;     /**< Total system uptime */
    uint32_t error_count;              /**< Error count */
    uint32_t warning_count;            /**< Warning count */
} system_health_t;

/* ============================================================================
 * Application Lifecycle API
 * ============================================================================ */

/**
 * @brief Initialize the application
 * @param config Application configuration
 * @return 0 on success, negative error code on failure
 */
int app_init(const app_config_t *config);

/**
 * @brief Start the application
 * @return 0 on success, negative error code on failure
 */
int app_start(void);

/**
 * @brief Stop the application
 * @return 0 on success, negative error code on failure
 */
int app_stop(void);

/**
 * @brief Deinitialize the application
 * @return 0 on success, negative error code on failure
 */
int app_deinit(void);

/**
 * @brief Check if application is initialized
 * @return 1 if initialized, 0 otherwise
 */
int app_is_initialized(void);

/**
 * @brief Check if application is running
 * @return 1 if running, 0 otherwise
 */
int app_is_running(void);

/* ============================================================================
 * Power Management Control API
 * ============================================================================ */

/**
 * @brief Set power management mode
 * @param mode Power mode to set
 * @return 0 on success, negative error code on failure
 */
int app_set_power_mode(power_mode_t mode);

/**
 * @brief Get current power management mode
 * @return Current power mode
 */
power_mode_t app_get_power_mode(void);

/**
 * @brief Enable a power channel
 * @param channel_id Channel ID (1-4)
 * @return 0 on success, negative error code on failure
 */
int app_enable_channel(uint8_t channel_id);

/**
 * @brief Disable a power channel
 * @param channel_id Channel ID (1-4)
 * @return 0 on success, negative error code on failure
 */
int app_disable_channel(uint8_t channel_id);

/**
 * @brief Get power channel information
 * @param channel_id Channel ID (1-4)
 * @param info Pointer to store channel information
 * @return 0 on success, negative error code on failure
 */
int app_get_channel_info(uint8_t channel_id, power_channel_info_t *info);

/**
 * @brief Get all power channels information
 * @param info Array to store channel information
 * @param max_channels Maximum number of channels
 * @return Number of channels retrieved, negative error code on failure
 */
int app_get_all_channels(power_channel_info_t *info, uint8_t max_channels);

/* ============================================================================
 * Monitoring and Diagnostics API
 * ============================================================================ */

/**
 * @brief Get system health status
 * @param health Pointer to store health status
 * @return 0 on success, negative error code on failure
 */
int app_get_system_health(system_health_t *health);

/**
 * @brief Get system uptime in milliseconds
 * @return Uptime in milliseconds
 */
uint32_t app_get_uptime_ms(void);

/**
 * @brief Generate system status report in JSON format
 * @param buffer Buffer to store JSON string
 * @param buffer_size Size of buffer
 * @return Length of JSON string, negative error code on failure
 */
int app_generate_status_report(char *buffer, size_t buffer_size);

/**
 * @brief Log application event
 * @param level Log level (0=DEBUG, 1=INFO, 2=WARN, 3=ERROR)
 * @param format Format string
 * @param ... Variable arguments
 */
void app_log(int level, const char *format, ...);

/* ============================================================================
 * Task Management API
 * ============================================================================ */

/**
 * @brief Create application monitoring task
 * @return Task handle, NULL on failure
 */
task_handle_t app_create_monitor_task(void);

/**
 * @brief Create application control task
 * @return Task handle, NULL on failure
 */
task_handle_t app_create_control_task(void);

/**
 * @brief Create application communication task
 * @return Task handle, NULL on failure
 */
task_handle_t app_create_communication_task(void);

/* ============================================================================
 * Callback Functions
 * ============================================================================ */

/**
 * @brief Power mode change callback type
 * @param old_mode Previous power mode
 * @param new_mode New power mode
 */
typedef void (*power_mode_callback_t)(power_mode_t old_mode, power_mode_t new_mode);

/**
 * @brief Register power mode change callback
 * @param callback Callback function
 * @return 0 on success, negative error code on failure
 */
int app_register_power_mode_callback(power_mode_callback_t callback);

/**
 * @brief Channel status change callback type
 * @param channel_id Channel ID
 * @param old_status Previous status
 * @param new_status New status
 */
typedef void (*channel_status_callback_t)(uint8_t channel_id, 
                                           channel_status_t old_status,
                                           channel_status_t new_status);

/**
 * @brief Register channel status change callback
 * @param callback Callback function
 * @return 0 on success, negative error code on failure
 */
int app_register_channel_status_callback(channel_status_callback_t callback);

#ifdef __cplusplus
}
#endif

#endif /* APPLICATION_H */
