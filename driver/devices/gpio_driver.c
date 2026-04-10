/**
 * @file gpio_driver.c
 * @brief GPIO device driver implementation
 *
 * Implements GPIO driver using HAL layer.
 */

#include "gpio_driver.h"
#include "hal_gpio.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/* Maximum GPIO devices */
#define MAX_GPIO_DEVICES 16

/* GPIO device structure */
typedef struct {
  char name[32];
  uint8_t port;
  uint8_t pin;
  hal_gpio_config_t config;
  int initialized;
} gpio_device_t;

static gpio_device_t gpio_devices[MAX_GPIO_DEVICES];
static int gpio_device_count = 0;

/* Forward declarations */
static driver_handle_t gpio_open(const char *path, uint32_t flags);
static int gpio_close(driver_handle_t handle);
static ssize_t gpio_read(driver_handle_t handle, void *buffer, size_t size);
static ssize_t gpio_write(driver_handle_t handle, const void *buffer,
                          size_t size);
static int gpio_control(driver_handle_t handle, int cmd, void *arg);

/* GPIO device operations */
static const driver_ops_t gpio_ops = {
    .open = gpio_open,
    .close = gpio_close,
    .read = gpio_read,
    .write = gpio_write,
    .control = gpio_control,
};

int gpio_driver_init(void) {
  printf("[GPIO-DRV] Initializing GPIO driver...\n");
  memset(gpio_devices, 0, sizeof(gpio_devices));
  gpio_device_count = 0;
  printf("[GPIO-DRV] GPIO driver initialized\n");
  return 0;
}

int gpio_device_register(const char *name, const gpio_drv_config_t *config) {
  if (!name || !config || gpio_device_count >= MAX_GPIO_DEVICES) {
    return -1;
  }

  /* Initialize HAL GPIO */
  hal_gpio_config_t hal_config;
  memset(&hal_config, 0, sizeof(hal_config));
  hal_config.pin = config->pin;
  hal_config.mode = (config->mode == GPIO_DRV_MODE_OUTPUT) ? GPIO_MODE_OUTPUT
                                                           : GPIO_MODE_INPUT;
  hal_config.pull = GPIO_PULLUP; // Use existing HAL enum
  hal_config.speed = GPIO_SPEED_MEDIUM;

  int ret = hal_gpio_init(config->port, &hal_config);
  if (ret != 0) {
    printf("[GPIO-DRV] Error: Failed to init HAL GPIO for '%s'\n", name);
    return ret;
  }

  /* Register device */
  strncpy(gpio_devices[gpio_device_count].name, name,
          sizeof(gpio_devices[0].name) - 1);
  gpio_devices[gpio_device_count].port = config->port;
  gpio_devices[gpio_device_count].pin = config->pin;
  gpio_devices[gpio_device_count].config = hal_config;
  gpio_devices[gpio_device_count].initialized = 1;

  device_info_t dev_info;
  dev_info.name = name;
  dev_info.type = DEVICE_TYPE_GPIO;
  dev_info.ops = &gpio_ops;
  dev_info.private_data = &gpio_devices[gpio_device_count];

  ret = driver_register(&dev_info);
  if (ret != 0) {
    printf("[GPIO-DRV] Error: Failed to register device '%s'\n", name);
    return ret;
  }

  gpio_device_count++;
  printf("[GPIO-DRV] Device '%s' registered (Port=%d, Pin=%d)\n", name,
         config->port, config->pin);
  return 0;
}

static driver_handle_t gpio_open(const char *path, uint32_t flags) {
  printf("[GPIO-DRV] Opening device '%s' with flags=0x%04X\n", path, flags);

  /* Find device */
  for (int i = 0; i < gpio_device_count; i++) {
    if (strcmp(gpio_devices[i].name, path) == 0 &&
        gpio_devices[i].initialized) {
      return (driver_handle_t)i;
    }
  }

  printf("[GPIO-DRV] Error: Device '%s' not found or not initialized\n", path);
  return DRIVER_INVALID_HANDLE;
}

static int gpio_close(driver_handle_t handle) {
  if (handle < 0 || handle >= gpio_device_count) {
    return -1;
  }

  printf("[GPIO-DRV] Closing device index %d\n", (int)handle);
  return 0;
}

static ssize_t gpio_read(driver_handle_t handle, void *buffer, size_t size) {
  if (handle < 0 || handle >= gpio_device_count || !buffer || size < 1) {
    return -1;
  }

  gpio_device_t *dev = &gpio_devices[handle];
  uint8_t value = hal_gpio_read(dev->port, dev->pin);
  *(uint8_t *)buffer = value;

  printf("[GPIO-DRV] Read: Port=%d, Pin=%d, Value=%d\n", dev->port, dev->pin,
         value);
  return 1;
}

static ssize_t gpio_write(driver_handle_t handle, const void *buffer,
                          size_t size) {
  if (handle < 0 || handle >= gpio_device_count || !buffer || size < 1) {
    return -1;
  }

  gpio_device_t *dev = &gpio_devices[handle];
  uint8_t value = *(const uint8_t *)buffer;
  hal_gpio_write(dev->port, dev->pin, value);

  printf("[GPIO-DRV] Write: Port=%d, Pin=%d, Value=%d\n", dev->port, dev->pin,
         value);
  return 1;
}

static int gpio_control(driver_handle_t handle, int cmd, void *arg) {
  if (handle < 0 || handle >= gpio_device_count) {
    return -1;
  }

  gpio_device_t *dev = &gpio_devices[handle];

  switch (cmd) {
  case GPIO_CMD_SET_MODE:
    printf("[GPIO-DRV] Control: Set mode for '%s'\n", dev->name);
    break;
  case GPIO_CMD_GET_MODE:
    printf("[GPIO-DRV] Control: Get mode for '%s'\n", dev->name);
    break;
  default:
    printf("[GPIO-DRV] Error: Unknown command 0x%02X\n", cmd);
    return -2;
  }

  return 0;
}
