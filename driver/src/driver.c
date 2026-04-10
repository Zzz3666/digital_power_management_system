/**
 * @file driver.c
 * @brief Driver framework implementation
 * 
 * Implements the core driver framework with device registration and management.
 */

#include "driver.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* Maximum number of devices */
#define MAX_DEVICES     32

/* Device table */
static device_info_t device_table[MAX_DEVICES];
static int device_count = 0;

/* Handle to device index mapping */
typedef struct {
    driver_handle_t handle;
    int device_index;
    uint32_t flags;
    int in_use;
} handle_mapping_t;

static handle_mapping_t handle_table[MAX_DEVICES];
static int next_handle = 0;

int driver_init(void)
{
    printf("[DRIVER] Initializing driver framework...\n");
    
    memset(device_table, 0, sizeof(device_table));
    memset(handle_table, 0, sizeof(handle_table));
    device_count = 0;
    next_handle = 0;
    
    printf("[DRIVER] Driver framework initialized\n");
    return 0;
}

int driver_deinit(void)
{
    printf("[DRIVER] Deinitializing driver framework...\n");
    
    /* Close all open handles */
    for (int i = 0; i < MAX_DEVICES; i++) {
        if (handle_table[i].in_use) {
            driver_close(handle_table[i].handle);
        }
    }
    
    /* Unregister all devices */
    for (int i = 0; i < device_count; i++) {
        if (device_table[i].name != NULL) {
            driver_unregister(device_table[i].name);
        }
    }
    
    printf("[DRIVER] Driver framework deinitialized\n");
    return 0;
}

int driver_register(const device_info_t *info)
{
    if (!info || !info->name || !info->ops) {
        return -1;
    }
    
    if (device_count >= MAX_DEVICES) {
        printf("[DRIVER] Error: Device table full\n");
        return -2;
    }
    
    /* Check if device already exists */
    for (int i = 0; i < device_count; i++) {
        if (strcmp(device_table[i].name, info->name) == 0) {
            printf("[DRIVER] Error: Device '%s' already registered\n", info->name);
            return -3;
        }
    }
    
    /* Register device */
    device_table[device_count].name = info->name;
    device_table[device_count].type = info->type;
    device_table[device_count].ops = info->ops;
    device_table[device_count].private_data = info->private_data;
    device_count++;
    
    printf("[DRIVER] Device '%s' registered successfully\n", info->name);
    return 0;
}

int driver_unregister(const char *name)
{
    if (!name) {
        return -1;
    }
    
    for (int i = 0; i < device_count; i++) {
        if (strcmp(device_table[i].name, name) == 0) {
            /* Shift remaining devices */
            for (int j = i; j < device_count - 1; j++) {
                device_table[j] = device_table[j + 1];
            }
            device_count--;
            printf("[DRIVER] Device '%s' unregistered\n", name);
            return 0;
        }
    }
    
    printf("[DRIVER] Error: Device '%s' not found\n", name);
    return -2;
}

const device_info_t* driver_find_device(const char *name)
{
    if (!name) {
        return NULL;
    }
    
    for (int i = 0; i < device_count; i++) {
        if (strcmp(device_table[i].name, name) == 0) {
            return &device_table[i];
        }
    }
    
    return NULL;
}

driver_handle_t driver_open(const char *path, uint32_t flags)
{
    const device_info_t *device = driver_find_device(path);
    if (!device) {
        printf("[DRIVER] Error: Device '%s' not found\n", path);
        return DRIVER_INVALID_HANDLE;
    }
    
    if (!device->ops || !device->ops->open) {
        printf("[DRIVER] Error: Device '%s' has no open operation\n", path);
        return DRIVER_INVALID_HANDLE;
    }
    
    /* Call device-specific open */
    driver_handle_t handle = device->ops->open(path, flags);
    if (handle < 0) {
        return handle;
    }
    
    /* Allocate handle mapping */
    for (int i = 0; i < MAX_DEVICES; i++) {
        if (!handle_table[i].in_use) {
            handle_table[i].handle = handle;
            handle_table[i].device_index = (int)(device - device_table);
            handle_table[i].flags = flags;
            handle_table[i].in_use = 1;
            return handle;
        }
    }
    
    printf("[DRIVER] Error: Handle table full\n");
    return DRIVER_INVALID_HANDLE;
}

int driver_close(driver_handle_t handle)
{
    if (handle == DRIVER_INVALID_HANDLE) {
        return -1;
    }
    
    /* Find handle mapping */
    for (int i = 0; i < MAX_DEVICES; i++) {
        if (handle_table[i].in_use && handle_table[i].handle == handle) {
            int dev_idx = handle_table[i].device_index;
            
            if (dev_idx >= 0 && dev_idx < device_count) {
                const device_info_t *device = &device_table[dev_idx];
                if (device->ops && device->ops->close) {
                    device->ops->close(handle);
                }
            }
            
            handle_table[i].in_use = 0;
            return 0;
        }
    }
    
    printf("[DRIVER] Error: Invalid handle %d\n", handle);
    return -2;
}

ssize_t driver_read(driver_handle_t handle, void *buffer, size_t size)
{
    if (handle == DRIVER_INVALID_HANDLE || !buffer) {
        return -1;
    }
    
    /* Find handle mapping */
    for (int i = 0; i < MAX_DEVICES; i++) {
        if (handle_table[i].in_use && handle_table[i].handle == handle) {
            int dev_idx = handle_table[i].device_index;
            
            if (dev_idx >= 0 && dev_idx < device_count) {
                const device_info_t *device = &device_table[dev_idx];
                if (device->ops && device->ops->read) {
                    return device->ops->read(handle, buffer, size);
                }
            }
            break;
        }
    }
    
    printf("[DRIVER] Error: Invalid handle %d for read\n", handle);
    return -2;
}

ssize_t driver_write(driver_handle_t handle, const void *buffer, size_t size)
{
    if (handle == DRIVER_INVALID_HANDLE || !buffer) {
        return -1;
    }
    
    /* Find handle mapping */
    for (int i = 0; i < MAX_DEVICES; i++) {
        if (handle_table[i].in_use && handle_table[i].handle == handle) {
            int dev_idx = handle_table[i].device_index;
            
            if (dev_idx >= 0 && dev_idx < device_count) {
                const device_info_t *device = &device_table[dev_idx];
                if (device->ops && device->ops->write) {
                    return device->ops->write(handle, buffer, size);
                }
            }
            break;
        }
    }
    
    printf("[DRIVER] Error: Invalid handle %d for write\n", handle);
    return -2;
}

int driver_control(driver_handle_t handle, int cmd, void *arg)
{
    if (handle == DRIVER_INVALID_HANDLE) {
        return -1;
    }
    
    /* Find handle mapping */
    for (int i = 0; i < MAX_DEVICES; i++) {
        if (handle_table[i].in_use && handle_table[i].handle == handle) {
            int dev_idx = handle_table[i].device_index;
            
            if (dev_idx >= 0 && dev_idx < device_count) {
                const device_info_t *device = &device_table[dev_idx];
                if (device->ops && device->ops->control) {
                    return device->ops->control(handle, cmd, arg);
                }
            }
            break;
        }
    }
    
    printf("[DRIVER] Error: Invalid handle %d for control\n", handle);
    return -2;
}
