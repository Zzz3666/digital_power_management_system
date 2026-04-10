/**
 * @file modbus_protocol.h
 * @brief Modbus protocol component
 *
 * Provides Modbus RTU/TCP protocol stack functionality.
 * This is a wrapper for third-party Modbus libraries.
 */

#ifndef MODBUS_PROTOCOL_H
#define MODBUS_PROTOCOL_H

#include "component.h"
#include <stddef.h>
#include <stdint.h>


#ifdef __cplusplus
extern "C" {
#endif

/* Modbus mode */
typedef enum {
  MODBUS_MODE_RTU = 0,
  MODBUS_MODE_TCP,
} modbus_mode_t;

/* Modbus role */
typedef enum {
  MODBUS_ROLE_MASTER = 0,
  MODBUS_ROLE_SLAVE,
} modbus_role_t;

/* Modbus configuration */
typedef struct {
  modbus_mode_t mode;    /* Communication mode */
  modbus_role_t role;    /* Master or Slave */
  uint8_t slave_id;      /* Slave ID (1-247) */
  const char *port_name; /* Device port name (e.g., "uart0") */
  uint32_t baudrate;     /* Baudrate (for RTU) */
} modbus_config_t;

/* Modbus function codes */
#define MODBUS_FUNC_READ_COILS 0x01
#define MODBUS_FUNC_READ_DISCRETE 0x02
#define MODBUS_FUNC_READ_HOLDING 0x03
#define MODBUS_FUNC_READ_INPUT 0x04
#define MODBUS_FUNC_WRITE_SINGLE_COIL 0x05
#define MODBUS_FUNC_WRITE_SINGLE_REG 0x06
#define MODBUS_FUNC_WRITE_MULTI_COILS 0x0F
#define MODBUS_FUNC_WRITE_MULTI_REGS 0x10

/* Modbus protocol API */

/**
 * @brief Initialize Modbus protocol component
 * @param config Modbus configuration
 * @return 0 on success, negative error code on failure
 */
int modbus_protocol_init(const modbus_config_t *config);

/**
 * @brief Deinitialize Modbus protocol component
 */
void modbus_protocol_deinit(void);

/**
 * @brief Read holding registers
 * @param slave_addr Slave address
 * @param start_addr Start register address
 * @param num_regs Number of registers to read
 * @param data Output data buffer
 * @return Number of registers read, negative error code on failure
 */
int modbus_read_holding_registers(uint8_t slave_addr, uint16_t start_addr,
                                  uint16_t num_regs, uint16_t *data);

/**
 * @brief Write single register
 * @param slave_addr Slave address
 * @param reg_addr Register address
 * @param value Value to write
 * @return 0 on success, negative error code on failure
 */
int modbus_write_single_register(uint8_t slave_addr, uint16_t reg_addr,
                                 uint16_t value);

/**
 * @brief Write multiple registers
 * @param slave_addr Slave address
 * @param start_addr Start register address
 * @param num_regs Number of registers to write
 * @param data Data to write
 * @return Number of registers written, negative error code on failure
 */
int modbus_write_multiple_registers(uint8_t slave_addr, uint16_t start_addr,
                                    uint16_t num_regs, const uint16_t *data);

/**
 * @brief Read coils
 * @param slave_addr Slave address
 * @param start_addr Start coil address
 * @param num_coils Number of coils to read
 * @param data Output data buffer (bit-packed)
 * @return Number of coils read, negative error code on failure
 */
int modbus_read_coils(uint8_t slave_addr, uint16_t start_addr,
                      uint16_t num_coils, uint8_t *data);

/**
 * @brief Write single coil
 * @param slave_addr Slave address
 * @param coil_addr Coil address
 * @param value Coil value (0 or 1)
 * @return 0 on success, negative error code on failure
 */
int modbus_write_single_coil(uint8_t slave_addr, uint16_t coil_addr,
                             uint8_t value);

/**
 * @brief Get protocol status
 * @return Protocol status
 */
component_status_t modbus_get_status(void);

#ifdef __cplusplus
}
#endif

#endif /* MODBUS_PROTOCOL_H */
