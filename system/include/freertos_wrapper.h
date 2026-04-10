/**
 * @file freertos_wrapper.h
 * @brief FreeRTOS wrapper for system layer
 *
 * Provides FreeRTOS implementation for system layer interfaces.
 * This is a simulation - in real implementation, include FreeRTOS headers.
 */

#ifndef FREERTOS_WRAPPER_H
#define FREERTOS_WRAPPER_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initialize FreeRTOS wrapper
 * @return 0 on success, negative error code on failure
 */
int freertos_wrapper_init(void);

/**
 * @brief Deinitialize FreeRTOS wrapper
 */
void freertos_wrapper_deinit(void);

/**
 * @brief Get wrapper status
 * @return 1 if initialized, 0 otherwise
 */
int freertos_wrapper_is_initialized(void);

#ifdef __cplusplus
}
#endif

#endif /* FREERTOS_WRAPPER_H */
