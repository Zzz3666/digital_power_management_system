/**
 * @file gpio_driver.h
 * @brief GPIO device driver interface
 * 
 * Provides standard POSIX-like interface for GPIO devices:
 * - open/close/read/write/control
 */

#ifndef GPIO_DRIVER_H
#define GPIO_DRIVER_H

#include "driver.h"

#ifdef __cplusplus
extern "C" {
#endif

/* GPIO control commands */
#define GPIO_CMD_SET_MODE       0x01    /* Set GPIO mode */
#define GPIO_CMD_GET_MODE       0x02    /* Get GPIO mode */
#define GPIO_CMD_SET_PULL       0x03    /* Set pull-up/down */
#define GPIO_CMD_GET_PULL       0x04    /* Get pull-up/down */
#define GPIO_CMD_SET_SPEED      0x05    /* Set speed */
#define GPIO_CMD_INTERRUPT_EN   0x10    /* Enable interrupt */
#define GPIO_CMD_INTERRUPT_DIS  0x11    /* Disable interrupt */

/* GPIO modes */
typedef enum {
    GPIO_DRV_MODE_INPUT = 0,
    GPIO_DRV_MODE_OUTPUT,
    GPIO_DRV_MODE_ALT_FUNC,
    GPIO_DRV_MODE_ANALOG,
} gpio_drv_mode_t;

/* GPIO pull configuration */
typedef enum {
    GPIO_DRV_PULL_NONE = 0,
    GPIO_DRV_PULL_UP,
    GPIO_DRV_PULL_DOWN,
} gpio_drv_pull_t;

/* GPIO device configuration */
typedef struct {
    uint8_t port;               /* GPIO port */
    uint8_t pin;                /* GPIO pin */
    gpio_drv_mode_t mode;       /* GPIO mode */
    gpio_drv_pull_t pull;       /* Pull configuration */
} gpio_drv_config_t;

/* GPIO driver API */

/**
 * @brief Initialize GPIO driver
 * @return 0 on success, negative error code on failure
 */
int gpio_driver_init(void);

/**
 * @brief Register GPIO device
 * @param name Device name (e.g., "gpio0", "led1")
 * @param config GPIO configuration
 * @return 0 on success, negative error code on failure
 */
int gpio_device_register(const char *name, const gpio_drv_config_t *config);

#ifdef __cplusplus
}
#endif

#endif /* GPIO_DRIVER_H */
