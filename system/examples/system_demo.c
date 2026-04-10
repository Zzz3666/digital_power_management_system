/**
 * @file system_demo.c
 * @brief System layer demonstration
 *
 * Shows how to use the system layer with RTOS abstraction.
 */

#include "bsp.h"
#include "component.h"
#include "driver.h"
#include "hal.h"
#include "json_codec.h"
#include "system.h"
#include <stdio.h>
#include <string.h>


/* Example task function */
void example_task(void *argument) {
  const char *name = (const char *)argument;
  printf("[TASK-%s] Task started\n", name);

  for (int i = 0; i < 3; i++) {
    printf("[TASK-%s] Running iteration %d\n", name, i);
    system_delay_ms(100);
  }

  printf("[TASK-%s] Task completed\n", name);
}

int main(void) {
  printf("========================================\n");
  printf("  System Layer Demonstration\n");
  printf("========================================\n\n");

  printf("Architecture:\n");
  printf("  Application -> System -> Component -> Driver -> HAL -> BSP -> "
         "Hardware\n\n");

  printf("Dependency Chain:\n");
  printf("  System depends on Component only\n");
  printf("  Component depends on Driver\n");
  printf("  Driver depends on HAL\n");
  printf("  HAL depends on BSP\n");
  printf("  BSP is platform-specific\n\n");

  /* Initialize all layers */
  printf("=== Initialization ===\n");
  init_bsp();
  hal_init();
  driver_init();
  component_init();

  system_config_t sys_config;
  sys_config.tick_rate_hz = 1000;
  sys_config.min_stack_size = 512;
  sys_config.default_stack_size = 2048;
  system_init(&sys_config);
  printf("\n");

  /* ========== Task Management Demo ========== */
  printf("=== Task Management ===\n");

  /* Create tasks */
  task_handle_t task1 = system_task_create(
      "Task1", example_task, (void *)"Task1", 1024, TASK_PRIORITY_MEDIUM);
  task_handle_t task2 = system_task_create(
      "Task2", example_task, (void *)"Task2", 1024, TASK_PRIORITY_HIGH);

  if (task1 && task2) {
    printf("Tasks created successfully\n");

    /* Get current task */
    task_handle_t current = system_task_current();
    printf("Current task: %p\n", (void *)current);

    /* Get task state */
    task_state_t state = system_task_get_state(task1);
    printf("Task1 state: %d (0=RUNNING, 1=READY, 2=BLOCKED, 3=SUSPENDED)\n",
           state);

    /* Suspend and resume */
    system_task_suspend(task1);
    printf("Task1 suspended\n");
    system_task_resume(task1);
    printf("Task1 resumed\n");

    /* Delay */
    printf("Main task delaying...\n");
    system_delay_ms(50);

    /* Delete tasks */
    system_task_delete(task1);
    system_task_delete(task2);
    printf("Tasks deleted\n");
  }
  printf("\n");

  /* ========== Mutex Demo ========== */
  printf("=== Mutex Management ===\n");

  mutex_handle_t mutex = system_mutex_create("SharedResource");
  if (mutex) {
    /* Lock mutex */
    int ret = system_mutex_lock(mutex, 1000);
    if (ret == 0) {
      printf("Mutex locked successfully\n");

      /* Critical section would be here */
      printf("Accessing shared resource...\n");

      /* Unlock mutex */
      system_mutex_unlock(mutex);
      printf("Mutex unlocked\n");
    }

    /* Delete mutex */
    system_mutex_delete(mutex);
    printf("Mutex deleted\n");
  }
  printf("\n");

  /* ========== Semaphore Demo ========== */
  printf("=== Semaphore Management ===\n");

  /* Binary semaphore */
  semaphore_handle_t bin_sem = system_semaphore_create_binary("BinarySem");
  if (bin_sem) {
    printf("Binary semaphore created\n");

    /* Give semaphore */
    system_semaphore_give(bin_sem);
    printf("Semaphore given\n");

    /* Take semaphore */
    int ret = system_semaphore_take(bin_sem, 1000);
    if (ret == 0) {
      printf("Semaphore taken successfully\n");
    }

    system_semaphore_delete(bin_sem);
    printf("Binary semaphore deleted\n");
  }

  /* Counting semaphore */
  semaphore_handle_t count_sem =
      system_semaphore_create_counting("CountingSem", 10, 5);
  if (count_sem) {
    printf("Counting semaphore created (max=10, initial=5)\n");
    system_semaphore_delete(count_sem);
    printf("Counting semaphore deleted\n");
  }
  printf("\n");

  /* ========== Queue Demo ========== */
  printf("=== Queue Management ===\n");

  queue_handle_t queue = system_queue_create("DataQueue", sizeof(int), 10);
  if (queue) {
    printf("Queue created (item_size=%zu, length=%u)\n", sizeof(int), 10);

    /* Send to queue */
    int data = 42;
    int ret = system_queue_send(queue, &data, 1000);
    if (ret == 0) {
      printf("Data sent to queue: %d\n", data);
    }

    /* Receive from queue */
    int received = 0;
    ret = system_queue_receive(queue, &received, 1000);
    if (ret == 0) {
      printf("Data received from queue: %d\n", received);
    }

    /* Delete queue */
    system_queue_delete(queue);
    printf("Queue deleted\n");
  }
  printf("\n");

  /* ========== System Info Demo ========== */
  printf("=== System Information ===\n");

  uint32_t tick = system_get_tick();
  uint32_t uptime = system_get_uptime_ms();
  printf("System tick: %u\n", tick);
  printf("System uptime: %u ms\n", uptime);
  printf("\n");

  /* ========== Integration with Component Layer ========== */
  printf("=== Integration with Component Layer ===\n");

  /* Use JSON codec through component layer */
  json_codec_config_t json_config;
  json_config.max_depth = 10;
  json_config.max_size = 4096;
  json_codec_init(&json_config);

  /* Create task status report as JSON */
  json_object_t report = json_object_create();
  json_object_add_string(report, "system", "power_management");
  json_object_add_int(report, "uptime_ms", uptime);
  json_object_add_int(report, "tick_count", tick);
  json_object_add_bool(report, "rtos_enabled", 1);

  char json_buffer[512];
  json_object_encode(report, json_buffer, sizeof(json_buffer));
  printf("Task status report: %s\n", json_buffer);

  json_object_delete(report);
  json_codec_deinit();
  printf("\n");

  /* Cleanup */
  printf("=== Cleanup ===\n");
  system_deinit();
  component_deinit();
  driver_deinit();
  hal_deinit();
  deinit_bsp();

  printf("\n========================================\n");
  printf("  Demo completed successfully!\n");
  printf("========================================\n");

  return 0;
}
