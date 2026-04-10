/**
 * @file system.h
 * @brief System layer core interface
 *
 * System layer provides OS abstraction for third-party RTOS,
 * making it easier to port to different operating systems.
 */

#ifndef SYSTEM_H
#define SYSTEM_H

#include <stddef.h>
#include <stdint.h>


#ifdef __cplusplus
extern "C" {
#endif

/* Task handle type */
typedef void *task_handle_t;

/* Invalid task handle */
#define SYSTEM_INVALID_TASK ((task_handle_t)0)

/* Mutex handle type */
typedef void *mutex_handle_t;

/* Semaphore handle type */
typedef void *semaphore_handle_t;

/* Queue handle type */
typedef void *queue_handle_t;

/* Task priority levels */
typedef enum {
  TASK_PRIORITY_IDLE = 0,
  TASK_PRIORITY_LOW,
  TASK_PRIORITY_MEDIUM,
  TASK_PRIORITY_HIGH,
  TASK_PRIORITY_REALTIME,
} task_priority_t;

/* Task state */
typedef enum {
  TASK_STATE_RUNNING = 0,
  TASK_STATE_READY,
  TASK_STATE_BLOCKED,
  TASK_STATE_SUSPENDED,
  TASK_STATE_DELETED,
} task_state_t;

/* Task function type */
typedef void (*task_function_t)(void *argument);

/* System configuration */
typedef struct {
  uint32_t tick_rate_hz;     /* System tick rate (Hz) */
  size_t min_stack_size;     /* Minimum stack size (bytes) */
  size_t default_stack_size; /* Default stack size (bytes) */
} system_config_t;

/* ========== System Core API ========== */

/**
 * @brief Initialize system layer
 * @param config System configuration
 * @return 0 on success, negative error code on failure
 */
int system_init(const system_config_t *config);

/**
 * @brief Deinitialize system layer
 */
void system_deinit(void);

/**
 * @brief Start the scheduler
 */
void system_start(void);

/**
 * @brief Get system tick count
 * @return Current tick count
 */
uint32_t system_get_tick(void);

/**
 * @brief Get system uptime in milliseconds
 * @return Uptime in milliseconds
 */
uint32_t system_get_uptime_ms(void);

/* ========== Task Management API ========== */

/**
 * @brief Create a new task
 * @param name Task name
 * @param func Task function
 * @param argument Task argument
 * @param stack_size Stack size in bytes
 * @param priority Task priority
 * @return Task handle on success, SYSTEM_INVALID_TASK on failure
 */
task_handle_t system_task_create(const char *name, task_function_t func,
                                 void *argument, size_t stack_size,
                                 task_priority_t priority);

/**
 * @brief Delete a task
 * @param task Task handle
 * @return 0 on success, negative error code on failure
 */
int system_task_delete(task_handle_t task);

/**
 * @brief Suspend a task
 * @param task Task handle
 * @return 0 on success, negative error code on failure
 */
int system_task_suspend(task_handle_t task);

/**
 * @brief Resume a task
 * @param task Task handle
 * @return 0 on success, negative error code on failure
 */
int system_task_resume(task_handle_t task);

/**
 * @brief Get current task handle
 * @return Current task handle
 */
task_handle_t system_task_current(void);

/**
 * @brief Get task state
 * @param task Task handle
 * @return Task state
 */
task_state_t system_task_get_state(task_handle_t task);

/**
 * @brief Delay current task
 * @param delay_ms Delay time in milliseconds
 */
void system_delay_ms(uint32_t delay_ms);

/* ========== Mutex API ========== */

/**
 * @brief Create a mutex
 * @param name Mutex name
 * @return Mutex handle on success, NULL on failure
 */
mutex_handle_t system_mutex_create(const char *name);

/**
 * @brief Delete a mutex
 * @param mutex Mutex handle
 * @return 0 on success, negative error code on failure
 */
int system_mutex_delete(mutex_handle_t mutex);

/**
 * @brief Lock a mutex
 * @param mutex Mutex handle
 * @param timeout_ms Timeout in milliseconds (0 = no wait, UINT32_MAX = wait
 * forever)
 * @return 0 on success, negative error code on failure
 */
int system_mutex_lock(mutex_handle_t mutex, uint32_t timeout_ms);

/**
 * @brief Unlock a mutex
 * @param mutex Mutex handle
 * @return 0 on success, negative error code on failure
 */
int system_mutex_unlock(mutex_handle_t mutex);

/* ========== Semaphore API ========== */

/**
 * @brief Create a binary semaphore
 * @param name Semaphore name
 * @return Semaphore handle on success, NULL on failure
 */
semaphore_handle_t system_semaphore_create_binary(const char *name);

/**
 * @brief Create a counting semaphore
 * @param name Semaphore name
 * @param max_count Maximum count
 * @param initial_count Initial count
 * @return Semaphore handle on success, NULL on failure
 */
semaphore_handle_t system_semaphore_create_counting(const char *name,
                                                    uint32_t max_count,
                                                    uint32_t initial_count);

/**
 * @brief Delete a semaphore
 * @param sem Semaphore handle
 * @return 0 on success, negative error code on failure
 */
int system_semaphore_delete(semaphore_handle_t sem);

/**
 * @brief Take a semaphore
 * @param sem Semaphore handle
 * @param timeout_ms Timeout in milliseconds
 * @return 0 on success, negative error code on failure
 */
int system_semaphore_take(semaphore_handle_t sem, uint32_t timeout_ms);

/**
 * @brief Give a semaphore
 * @param sem Semaphore handle
 * @return 0 on success, negative error code on failure
 */
int system_semaphore_give(semaphore_handle_t sem);

/* ========== Queue API ========== */

/**
 * @brief Create a queue
 * @param name Queue name
 * @param item_size Size of each item in bytes
 * @param length Number of items
 * @return Queue handle on success, NULL on failure
 */
queue_handle_t system_queue_create(const char *name, size_t item_size,
                                   uint32_t length);

/**
 * @brief Delete a queue
 * @param queue Queue handle
 * @return 0 on success, negative error code on failure
 */
int system_queue_delete(queue_handle_t queue);

/**
 * @brief Send to queue
 * @param queue Queue handle
 * @param item Item to send
 * @param timeout_ms Timeout in milliseconds
 * @return 0 on success, negative error code on failure
 */
int system_queue_send(queue_handle_t queue, const void *item,
                      uint32_t timeout_ms);

/**
 * @brief Receive from queue
 * @param queue Queue handle
 * @param buffer Buffer to receive item
 * @param timeout_ms Timeout in milliseconds
 * @return 0 on success, negative error code on failure
 */
int system_queue_receive(queue_handle_t queue, void *buffer,
                         uint32_t timeout_ms);

#ifdef __cplusplus
}
#endif

#endif /* SYSTEM_H */
