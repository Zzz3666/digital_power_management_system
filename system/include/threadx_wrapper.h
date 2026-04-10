/**
 * @file threadx_wrapper.h
 * @brief ThreadX wrapper for system layer
 *
 * Provides ThreadX implementation for system layer interfaces.
 * This is a simulation - in real implementation, include ThreadX headers.
 */

#ifndef THREADX_WRAPPER_H
#define THREADX_WRAPPER_H

#include "system.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initialize ThreadX wrapper
 * @return 0 on success, negative error code on failure
 */
int threadx_wrapper_init(void);

/**
 * @brief Deinitialize ThreadX wrapper
 */
void threadx_wrapper_deinit(void);

/**
 * @brief Get wrapper status
 * @return 1 if initialized, 0 otherwise
 */
int threadx_wrapper_is_initialized(void);

#ifdef __cplusplus
}
#endif

#endif /* THREADX_WRAPPER_H */
