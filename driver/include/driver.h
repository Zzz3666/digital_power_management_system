/**
 * @file driver.h
 * @brief Driver framework core interface
 * 
 * Provides standard device driver interface (POSIX-like):
 * - open: Open device
 * - close: Close device
 * - read: Read data from device
 * - write: Write data to device
 * - control: Control device (ioctl)
 */

#ifndef DRIVER_H
#define DRIVER_H

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Driver handle type */
typedef int32_t driver_handle_t;

/* Invalid handle */
#define DRIVER_INVALID_HANDLE   (-1)

/* Device types */
typedef enum {
    DEVICE_TYPE_GPIO = 0,
    DEVICE_TYPE_UART,
    DEVICE_TYPE_SPI,
    DEVICE_TYPE_I2C,
    DEVICE_TYPE_ADC,
    DEVICE_TYPE_TIMER,
    DEVICE_TYPE_PWM,
    DEVICE_TYPE_CUSTOM,
} device_type_t;

/* Device flags */
#define DEVICE_FLAG_RDONLY      0x0001  /* Read only */
#define DEVICE_FLAG_WRONLY      0x0002  /* Write only */
#define DEVICE_FLAG_RDWR        0x0004  /* Read and write */
#define DEVICE_FLAG_NONBLOCK    0x0008  /* Non-blocking mode */
#define DEVICE_FLAG_EXCLUSIVE   0x0010  /* Exclusive access */

/* Device operations structure */
typedef struct {
    /**
     * @brief Open device
     * @param path Device path/name
     * @param flags Open flags
     * @return Handle on success, negative error code on failure
     */
    driver_handle_t (*open)(const char *path, uint32_t flags);
    
    /**
     * @brief Close device
     * @param handle Device handle
     * @return 0 on success, negative error code on failure
     */
    int (*close)(driver_handle_t handle);
    
    /**
     * @brief Read data from device
     * @param handle Device handle
     * @param buffer Data buffer
     * @param size Number of bytes to read
     * @return Number of bytes read, negative error code on failure
     */
    ssize_t (*read)(driver_handle_t handle, void *buffer, size_t size);
    
    /**
     * @brief Write data to device
     * @param handle Device handle
     * @param buffer Data buffer
     * @param size Number of bytes to write
     * @return Number of bytes written, negative error code on failure
     */
    ssize_t (*write)(driver_handle_t handle, const void *buffer, size_t size);
    
    /**
     * @brief Control device (ioctl)
     * @param handle Device handle
     * @param cmd Control command
     * @param arg Command argument
     * @return 0 on success, negative error code on failure
     */
    int (*control)(driver_handle_t handle, int cmd, void *arg);
} driver_ops_t;

/* Device registration information */
typedef struct {
    const char *name;           /* Device name */
    device_type_t type;         /* Device type */
    const driver_ops_t *ops;    /* Device operations */
    void *private_data;         /* Private data for device */
} device_info_t;

/* Driver API functions */

/**
 * @brief Initialize driver framework
 * @return 0 on success, negative error code on failure
 */
int driver_init(void);

/**
 * @brief Deinitialize driver framework
 * @return 0 on success, negative error code on failure
 */
int driver_deinit(void);

/**
 * @brief Register a device
 * @param info Device information
 * @return 0 on success, negative error code on failure
 */
int driver_register(const device_info_t *info);

/**
 * @brief Unregister a device
 * @param name Device name
 * @return 0 on success, negative error code on failure
 */
int driver_unregister(const char *name);

/**
 * @brief Open a device
 * @param path Device path/name
 * @param flags Open flags
 * @return Device handle on success, DRIVER_INVALID_HANDLE on failure
 */
driver_handle_t driver_open(const char *path, uint32_t flags);

/**
 * @brief Close a device
 * @param handle Device handle
 * @return 0 on success, negative error code on failure
 */
int driver_close(driver_handle_t handle);

/**
 * @brief Read data from device
 * @param handle Device handle
 * @param buffer Data buffer
 * @param size Number of bytes to read
 * @return Number of bytes read, negative error code on failure
 */
ssize_t driver_read(driver_handle_t handle, void *buffer, size_t size);

/**
 * @brief Write data to device
 * @param handle Device handle
 * @param buffer Data buffer
 * @param size Number of bytes to write
 * @return Number of bytes written, negative error code on failure
 */
ssize_t driver_write(driver_handle_t handle, const void *buffer, size_t size);

/**
 * @brief Control device
 * @param handle Device handle
 * @param cmd Control command
 * @param arg Command argument
 * @return 0 on success, negative error code on failure
 */
int driver_control(driver_handle_t handle, int cmd, void *arg);

/**
 * @brief Find device by name
 * @param name Device name
 * @return Device info pointer, NULL if not found
 */
const device_info_t* driver_find_device(const char *name);

#ifdef __cplusplus
}
#endif

#endif /* DRIVER_H */
