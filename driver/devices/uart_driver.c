/**
 * @file uart_driver.c
 * @brief UART device driver implementation
 * 
 * Implements UART driver using HAL layer.
 */

#include "uart_driver.h"
#include "hal_uart.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* Maximum UART devices */
#define MAX_UART_DEVICES    4

/* UART device structure */
typedef struct {
    char name[32];
    uint8_t instance;
    hal_uart_config_t config;
    int initialized;
} uart_device_t;

static uart_device_t uart_devices[MAX_UART_DEVICES];
static int uart_device_count = 0;

/* Forward declarations */
static driver_handle_t uart_open(const char *path, uint32_t flags);
static int uart_close(driver_handle_t handle);
static ssize_t uart_read(driver_handle_t handle, void *buffer, size_t size);
static ssize_t uart_write(driver_handle_t handle, const void *buffer, size_t size);
static int uart_control(driver_handle_t handle, int cmd, void *arg);

/* UART device operations */
static const driver_ops_t uart_ops = {
    .open = uart_open,
    .close = uart_close,
    .read = uart_read,
    .write = uart_write,
    .control = uart_control,
};

int uart_driver_init(void)
{
    printf("[UART-DRV] Initializing UART driver...\n");
    memset(uart_devices, 0, sizeof(uart_devices));
    uart_device_count = 0;
    printf("[UART-DRV] UART driver initialized\n");
    return 0;
}

int uart_device_register(const char *name, const uart_drv_config_t *config)
{
    if (!name || !config || uart_device_count >= MAX_UART_DEVICES) {
        return -1;
    }
    
    /* Initialize HAL UART */
    hal_uart_config_t hal_config;
    memset(&hal_config, 0, sizeof(hal_config));
    
    int ret = hal_uart_init(config->baudrate > 9600 ? 0 : 1, &hal_config);
    if (ret != 0) {
        printf("[UART-DRV] Error: Failed to init HAL UART for '%s'\n", name);
        return ret;
    }
    
    /* Register device */
    strncpy(uart_devices[uart_device_count].name, name, sizeof(uart_devices[0].name) - 1);
    uart_devices[uart_device_count].instance = (uint8_t)(config->baudrate > 9600 ? 0 : 1);
    uart_devices[uart_device_count].config = hal_config;
    uart_devices[uart_device_count].initialized = 1;
    
    device_info_t dev_info;
    dev_info.name = name;
    dev_info.type = DEVICE_TYPE_UART;
    dev_info.ops = &uart_ops;
    dev_info.private_data = &uart_devices[uart_device_count];
    
    ret = driver_register(&dev_info);
    if (ret != 0) {
        printf("[UART-DRV] Error: Failed to register device '%s'\n", name);
        return ret;
    }
    
    uart_device_count++;
    printf("[UART-DRV] Device '%s' registered (Baud=%d)\n", name, config->baudrate);
    return 0;
}

static driver_handle_t uart_open(const char *path, uint32_t flags)
{
    printf("[UART-DRV] Opening device '%s' with flags=0x%04X\n", path, flags);
    
    for (int i = 0; i < uart_device_count; i++) {
        if (strcmp(uart_devices[i].name, path) == 0 && uart_devices[i].initialized) {
            return (driver_handle_t)i;
        }
    }
    
    printf("[UART-DRV] Error: Device '%s' not found\n", path);
    return DRIVER_INVALID_HANDLE;
}

static int uart_close(driver_handle_t handle)
{
    if (handle < 0 || handle >= uart_device_count) {
        return -1;
    }
    
    printf("[UART-DRV] Closing device index %d\n", (int)handle);
    return 0;
}

static ssize_t uart_read(driver_handle_t handle, void *buffer, size_t size)
{
    if (handle < 0 || handle >= uart_device_count || !buffer || size == 0) {
        return -1;
    }
    
    uart_device_t *dev = &uart_devices[handle];
    uint32_t timeout = 1000;  // 1 second timeout
    return hal_uart_receive(dev->instance, buffer, size, timeout);
}

static ssize_t uart_write(driver_handle_t handle, const void *buffer, size_t size)
{
    if (handle < 0 || handle >= uart_device_count || !buffer || size == 0) {
        return -1;
    }
    
    uart_device_t *dev = &uart_devices[handle];
    uint32_t timeout = 1000;  // 1 second timeout
    return hal_uart_send(dev->instance, buffer, size, timeout);
}

static int uart_control(driver_handle_t handle, int cmd, void *arg)
{
    if (handle < 0 || handle >= uart_device_count) {
        return -1;
    }
    
    uart_device_t *dev = &uart_devices[handle];
    
    switch (cmd) {
        case UART_CMD_SET_BAUDRATE:
            printf("[UART-DRV] Control: Set baudrate for '%s'\n", dev->name);
            break;
        case UART_CMD_FLUSH:
            printf("[UART-DRV] Control: Flush buffer for '%s'\n", dev->name);
            break;
        default:
            printf("[UART-DRV] Error: Unknown command 0x%02X\n", cmd);
            return -2;
    }
    
    return 0;
}
