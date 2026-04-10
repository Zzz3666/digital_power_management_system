/**
 * @file driver_demo.c
 * @brief Driver layer demonstration
 *
 * Shows how to use the driver framework with POSIX-like interface.
 */

#include "bsp.h"
#include "driver.h"
#include "gpio_driver.h"
#include "hal.h"
#include "uart_driver.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>


int main(void) {
  printf("========================================\n");
  printf("  Driver Layer Demonstration\n");
  printf("========================================\n\n");

  printf("Architecture:\n");
  printf("  Application -> Driver -> HAL -> BSP -> Hardware\n\n");

  printf("Dependency Chain:\n");
  printf("  Driver depends on HAL only\n");
  printf("  HAL depends on BSP\n");
  printf("  BSP is platform-specific\n\n");

  /* Initialize layers in order */
  printf("=== Initialization ===\n");
  init_bsp();
  hal_init();
  driver_init();
  printf("\n");

  /* Initialize drivers */
  gpio_driver_init();
  uart_driver_init();
  printf("\n");

  /* Register GPIO device */
  printf("=== Registering Devices ===\n");
  gpio_drv_config_t gpio_config;
  gpio_config.port = 0;
  gpio_config.pin = 5;
  gpio_config.mode = GPIO_DRV_MODE_OUTPUT;
  gpio_config.pull = GPIO_DRV_PULL_NONE;

  gpio_device_register("led1", &gpio_config);

  /* Register UART device */
  uart_drv_config_t uart_config;
  uart_config.baudrate = 115200;
  uart_config.parity = UART_DRV_PARITY_NONE;
  uart_config.stop_bits = UART_DRV_STOP_1BIT;
  uart_config.data_bits = 8;

  uart_device_register("console", &uart_config);
  printf("\n");

  /* Use devices through driver interface */
  printf("=== Using Devices (POSIX-like Interface) ===\n");

  /* Open LED device */
  printf("\n--- LED Control ---\n");
  driver_handle_t led_handle = driver_open("led1", DEVICE_FLAG_WRONLY);
  if (led_handle != DRIVER_INVALID_HANDLE) {
    /* Write to LED (turn on) */
    uint8_t value = 1;
    ssize_t written = driver_write(led_handle, &value, 1);
    printf("Wrote %zd bytes to LED (ON)\n", written);

    /* Read LED state */
    uint8_t read_value = 0;
    ssize_t read_bytes = driver_read(led_handle, &read_value, 1);
    printf("Read %zd bytes from LED: %d\n", read_bytes, read_value);

    /* Control LED */
    driver_control(led_handle, GPIO_CMD_SET_MODE, NULL);

    /* Close device */
    driver_close(led_handle);
    printf("LED device closed\n");
  }

  /* Open UART device */
  printf("\n--- UART Communication ---\n");
  driver_handle_t uart_handle = driver_open("console", DEVICE_FLAG_RDWR);
  if (uart_handle != DRIVER_INVALID_HANDLE) {
    /* Write to UART */
    const char *message = "Hello from Driver Layer!\n";
    ssize_t written = driver_write(uart_handle, message, strlen(message));
    printf("Wrote %zd bytes to UART\n", written);

    /* Control UART */
    driver_control(uart_handle, UART_CMD_FLUSH, NULL);

    /* Close device */
    driver_close(uart_handle);
    printf("UART device closed\n");
  }

  printf("\n=== Cleanup ===\n");
  driver_deinit();
  hal_deinit();
  deinit_bsp();

  printf("\n========================================\n");
  printf("  Demo completed successfully!\n");
  printf("========================================\n");

  return 0;
}
