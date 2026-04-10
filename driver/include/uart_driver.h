/**
 * @file uart_driver.h
 * @brief UART device driver interface
 *
 * Provides standard POSIX-like interface for UART devices:
 * - open/close/read/write/control
 */

#ifndef UART_DRIVER_H
#define UART_DRIVER_H

#include "driver.h"

#ifdef __cplusplus
extern "C" {
#endif

/* UART control commands */
#define UART_CMD_SET_BAUDRATE 0x01 /* Set baudrate */
#define UART_CMD_GET_BAUDRATE 0x02 /* Get baudrate */
#define UART_CMD_SET_PARITY 0x03   /* Set parity */
#define UART_CMD_GET_PARITY 0x04   /* Get parity */
#define UART_CMD_SET_STOPBITS 0x05 /* Set stop bits */
#define UART_CMD_FLUSH 0x10        /* Flush buffer */
#define UART_CMD_GET_STATUS 0x20   /* Get status */

/* Parity types */
typedef enum {
  UART_DRV_PARITY_NONE = 0,
  UART_DRV_PARITY_EVEN,
  UART_DRV_PARITY_ODD,
} uart_parity_t;

/* Stop bits */
typedef enum {
  UART_DRV_STOP_1BIT = 0,
  UART_DRV_STOP_2BITS,
} uart_stopbits_t;

/* UART device configuration */
typedef struct {
  uint32_t baudrate;         /* Baudrate */
  uart_parity_t parity;      /* Parity */
  uart_stopbits_t stop_bits; /* Stop bits */
  uint8_t data_bits;         /* Data bits (typically 8) */
} uart_drv_config_t;

/* UART driver API */

/**
 * @brief Initialize UART driver
 * @return 0 on success, negative error code on failure
 */
int uart_driver_init(void);

/**
 * @brief Register UART device
 * @param name Device name (e.g., "uart0", "console")
 * @param config UART configuration
 * @return 0 on success, negative error code on failure
 */
int uart_device_register(const char *name, const uart_drv_config_t *config);

#ifdef __cplusplus
}
#endif

#endif /* UART_DRIVER_H */
