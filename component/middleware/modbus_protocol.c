/**
 * @file modbus_protocol.c
 * @brief Modbus protocol component implementation
 * 
 * Simulates Modbus protocol stack.
 * In real implementation, this would wrap libmodbus or similar library.
 */

#include "modbus_protocol.h"
#include "driver.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static modbus_config_t modbus_config = {0};
static component_status_t modbus_status = COMPONENT_STATUS_UNINIT;
static driver_handle_t uart_handle = DRIVER_INVALID_HANDLE;

int modbus_protocol_init(const modbus_config_t *config)
{
    if (!config) {
        return -1;
    }
    
    printf("[MODBUS] Initializing Modbus protocol component...\n");
    printf("[MODBUS] Mode: %s, Role: %s, Slave ID: %d\n",
           config->mode == MODBUS_MODE_RTU ? "RTU" : "TCP",
           config->role == MODBUS_ROLE_MASTER ? "Master" : "Slave",
           config->slave_id);
    
    memcpy(&modbus_config, config, sizeof(modbus_config_t));
    
    /* Open UART device for communication */
    if (config->mode == MODBUS_MODE_RTU && config->port_name) {
        uart_handle = driver_open(config->port_name, DEVICE_FLAG_RDWR);
        if (uart_handle == DRIVER_INVALID_HANDLE) {
            printf("[MODBUS] Error: Failed to open UART device '%s'\n", config->port_name);
            return -2;
        }
        printf("[MODBUS] UART device '%s' opened successfully\n", config->port_name);
    }
    
    modbus_status = COMPONENT_STATUS_READY;
    
    /* Register as component */
    component_info_t info;
    info.name = "modbus_protocol";
    info.type = COMPONENT_TYPE_PROTOCOL;
    info.version = "1.0.0";
    info.private_data = NULL;
    component_register(&info);
    
    printf("[MODBUS] Modbus protocol initialized\n");
    return 0;
}

void modbus_protocol_deinit(void)
{
    printf("[MODBUS] Deinitializing Modbus protocol component...\n");
    
    if (uart_handle != DRIVER_INVALID_HANDLE) {
        driver_close(uart_handle);
        uart_handle = DRIVER_INVALID_HANDLE;
    }
    
    modbus_status = COMPONENT_STATUS_UNINIT;
    component_unregister("modbus_protocol");
}

int modbus_read_holding_registers(uint8_t slave_addr, uint16_t start_addr, 
                                   uint16_t num_regs, uint16_t *data)
{
    if (modbus_status != COMPONENT_STATUS_READY || !data) {
        return -1;
    }
    
    printf("[MODBUS] Read Holding Registers: Slave=%d, Addr=%d, Count=%d\n",
           slave_addr, start_addr, num_regs);
    
    /* Simulate reading registers */
    for (int i = 0; i < num_regs; i++) {
        data[i] = (uint16_t)(start_addr + i);  /* Dummy data */
    }
    
    modbus_status = COMPONENT_STATUS_RUNNING;
    return num_regs;
}

int modbus_write_single_register(uint8_t slave_addr, uint16_t reg_addr, uint16_t value)
{
    if (modbus_status != COMPONENT_STATUS_READY) {
        return -1;
    }
    
    printf("[MODBUS] Write Single Register: Slave=%d, Addr=%d, Value=%d\n",
           slave_addr, reg_addr, value);
    
    /* Simulate writing to UART */
    if (uart_handle != DRIVER_INVALID_HANDLE) {
        uint8_t buffer[8];
        buffer[0] = slave_addr;
        buffer[1] = MODBUS_FUNC_WRITE_SINGLE_REG;
        buffer[2] = (reg_addr >> 8) & 0xFF;
        buffer[3] = reg_addr & 0xFF;
        buffer[4] = (value >> 8) & 0xFF;
        buffer[5] = value & 0xFF;
        
        ssize_t written = driver_write(uart_handle, buffer, 6);
        printf("[MODBUS] Sent %zd bytes via UART\n", written);
    }
    
    modbus_status = COMPONENT_STATUS_RUNNING;
    return 0;
}

int modbus_write_multiple_registers(uint8_t slave_addr, uint16_t start_addr,
                                     uint16_t num_regs, const uint16_t *data)
{
    if (modbus_status != COMPONENT_STATUS_READY || !data) {
        return -1;
    }
    
    printf("[MODBUS] Write Multiple Registers: Slave=%d, Addr=%d, Count=%d\n",
           slave_addr, start_addr, num_regs);
    
    modbus_status = COMPONENT_STATUS_RUNNING;
    return num_regs;
}

int modbus_read_coils(uint8_t slave_addr, uint16_t start_addr,
                      uint16_t num_coils, uint8_t *data)
{
    if (modbus_status != COMPONENT_STATUS_READY || !data) {
        return -1;
    }
    
    printf("[MODBUS] Read Coils: Slave=%d, Addr=%d, Count=%d\n",
           slave_addr, start_addr, num_coils);
    
    /* Simulate reading coils */
    memset(data, 0, (num_coils + 7) / 8);
    
    modbus_status = COMPONENT_STATUS_RUNNING;
    return num_coils;
}

int modbus_write_single_coil(uint8_t slave_addr, uint16_t coil_addr, uint8_t value)
{
    if (modbus_status != COMPONENT_STATUS_READY) {
        return -1;
    }
    
    printf("[MODBUS] Write Single Coil: Slave=%d, Addr=%d, Value=%d\n",
           slave_addr, coil_addr, value);
    
    modbus_status = COMPONENT_STATUS_RUNNING;
    return 0;
}

component_status_t modbus_get_status(void)
{
    return modbus_status;
}
