/**
 * @file system.c
 * @brief System layer implementation
 *
 * Implements the system layer with OS abstraction.
 */

#include "system.h"
#include "freertos_wrapper.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* System state */
typedef struct {
  system_config_t config;
  int initialized;
  uint32_t start_tick;
} system_state_t;

static system_state_t sys_state = {0};

int system_init(const system_config_t *config) {
  printf("[SYSTEM] Initializing system layer...\n");

  if (config) {
    sys_state.config = *config;
  } else {
    /* Default configuration */
    sys_state.config.tick_rate_hz = 1000;
    sys_state.config.min_stack_size = 512;
    sys_state.config.default_stack_size = 2048;
  }

  /* Initialize FreeRTOS wrapper */
  int ret = freertos_wrapper_init();
  if (ret != 0) {
    printf("[SYSTEM] Error: Failed to initialize FreeRTOS wrapper\n");
    return ret;
  }

  sys_state.initialized = 1;
  sys_state.start_tick = (uint32_t)clock();

  printf("[SYSTEM] System layer initialized (tick_rate=%u Hz)\n",
         sys_state.config.tick_rate_hz);
  return 0;
}

void system_deinit(void) {
  printf("[SYSTEM] Deinitializing system layer...\n");

  freertos_wrapper_deinit();
  sys_state.initialized = 0;
}

void system_start(void) {
  printf("[SYSTEM] Starting scheduler...\n");
  printf("[SYSTEM] Note: In simulation mode, scheduler runs in main thread\n");
}

uint32_t system_get_tick(void) {
  if (!sys_state.initialized) {
    return 0;
  }

  uint32_t current = (uint32_t)clock();
  return current - sys_state.start_tick;
}

uint32_t system_get_uptime_ms(void) {
  if (!sys_state.initialized) {
    return 0;
  }

  uint32_t ticks = system_get_tick();
  return (ticks * 1000) / sys_state.config.tick_rate_hz;
}

/* ========== Task Management Implementation ========== */

task_handle_t system_task_create(const char *name, task_function_t func,
                                 void *argument, size_t stack_size,
                                 task_priority_t priority) {
  (void)argument; // Suppress unused parameter warning
  if (!sys_state.initialized || !name || !func) {
    return SYSTEM_INVALID_TASK;
  }

  printf("[SYSTEM-TASK] Creating task '%s' (priority=%d, stack=%zu)\n", name,
         priority, stack_size);

  /* Simulate task creation */
  task_handle_t task = malloc(64); /* Simulated task control block */
  if (task) {
    memset(task, 0, 64);
    printf("[SYSTEM-TASK] Task '%s' created successfully\n", name);
  }

  return task;
}

int system_task_delete(task_handle_t task) {
  if (!task) {
    return -1;
  }

  printf("[SYSTEM-TASK] Deleting task\n");
  free(task);
  return 0;
}

int system_task_suspend(task_handle_t task) {
  if (!task) {
    return -1;
  }

  printf("[SYSTEM-TASK] Suspending task\n");
  return 0;
}

int system_task_resume(task_handle_t task) {
  if (!task) {
    return -1;
  }

  printf("[SYSTEM-TASK] Resuming task\n");
  return 0;
}

task_handle_t system_task_current(void) {
  printf("[SYSTEM-TASK] Getting current task\n");
  return (task_handle_t)0x12345678; /* Simulated current task */
}

task_state_t system_task_get_state(task_handle_t task) {
  if (!task) {
    return TASK_STATE_DELETED;
  }

  return TASK_STATE_RUNNING;
}

void system_delay_ms(uint32_t delay_ms) {
  printf("[SYSTEM-TASK] Delaying for %u ms\n", delay_ms);
  /* In real implementation, this would call RTOS delay function */
  /* For simulation, we just print */
}

/* ========== Mutex Implementation ========== */

mutex_handle_t system_mutex_create(const char *name) {
  if (!sys_state.initialized || !name) {
    return NULL;
  }

  printf("[SYSTEM-MUTEX] Creating mutex '%s'\n", name);

  /* Simulate mutex creation */
  mutex_handle_t mutex = malloc(32);
  if (mutex) {
    memset(mutex, 0, 32);
    printf("[SYSTEM-MUTEX] Mutex '%s' created\n", name);
  }

  return mutex;
}

int system_mutex_delete(mutex_handle_t mutex) {
  if (!mutex) {
    return -1;
  }

  printf("[SYSTEM-MUTEX] Deleting mutex\n");
  free(mutex);
  return 0;
}

int system_mutex_lock(mutex_handle_t mutex, uint32_t timeout_ms) {
  if (!mutex) {
    return -1;
  }

  printf("[SYSTEM-MUTEX] Locking mutex (timeout=%u ms)\n", timeout_ms);
  return 0;
}

int system_mutex_unlock(mutex_handle_t mutex) {
  if (!mutex) {
    return -1;
  }

  printf("[SYSTEM-MUTEX] Unlocking mutex\n");
  return 0;
}

/* ========== Semaphore Implementation ========== */

semaphore_handle_t system_semaphore_create_binary(const char *name) {
  if (!sys_state.initialized || !name) {
    return NULL;
  }

  printf("[SYSTEM-SEM] Creating binary semaphore '%s'\n", name);

  semaphore_handle_t sem = malloc(32);
  if (sem) {
    memset(sem, 0, 32);
    printf("[SYSTEM-SEM] Binary semaphore '%s' created\n", name);
  }

  return sem;
}

semaphore_handle_t system_semaphore_create_counting(const char *name,
                                                    uint32_t max_count,
                                                    uint32_t initial_count) {
  if (!sys_state.initialized || !name) {
    return NULL;
  }

  printf("[SYSTEM-SEM] Creating counting semaphore '%s' (max=%u, init=%u)\n",
         name, max_count, initial_count);

  semaphore_handle_t sem = malloc(32);
  if (sem) {
    memset(sem, 0, 32);
    printf("[SYSTEM-SEM] Counting semaphore '%s' created\n", name);
  }

  return sem;
}

int system_semaphore_delete(semaphore_handle_t sem) {
  if (!sem) {
    return -1;
  }

  printf("[SYSTEM-SEM] Deleting semaphore\n");
  free(sem);
  return 0;
}

int system_semaphore_take(semaphore_handle_t sem, uint32_t timeout_ms) {
  if (!sem) {
    return -1;
  }

  printf("[SYSTEM-SEM] Taking semaphore (timeout=%u ms)\n", timeout_ms);
  return 0;
}

int system_semaphore_give(semaphore_handle_t sem) {
  if (!sem) {
    return -1;
  }

  printf("[SYSTEM-SEM] Giving semaphore\n");
  return 0;
}

/* ========== Queue Implementation ========== */

queue_handle_t system_queue_create(const char *name, size_t item_size,
                                   uint32_t length) {
  if (!sys_state.initialized || !name || item_size == 0 || length == 0) {
    return NULL;
  }

  printf("[SYSTEM-QUEUE] Creating queue '%s' (item_size=%zu, length=%u)\n",
         name, item_size, length);

  queue_handle_t queue = malloc(64);
  if (queue) {
    memset(queue, 0, 64);
    printf("[SYSTEM-QUEUE] Queue '%s' created\n", name);
  }

  return queue;
}

int system_queue_delete(queue_handle_t queue) {
  if (!queue) {
    return -1;
  }

  printf("[SYSTEM-QUEUE] Deleting queue\n");
  free(queue);
  return 0;
}

int system_queue_send(queue_handle_t queue, const void *item,
                      uint32_t timeout_ms) {
  if (!queue || !item) {
    return -1;
  }

  printf("[SYSTEM-QUEUE] Sending to queue (timeout=%u ms)\n", timeout_ms);
  return 0;
}

int system_queue_receive(queue_handle_t queue, void *buffer,
                         uint32_t timeout_ms) {
  if (!queue || !buffer) {
    return -1;
  }

  printf("[SYSTEM-QUEUE] Receiving from queue (timeout=%u ms)\n", timeout_ms);
  return 0;
}
