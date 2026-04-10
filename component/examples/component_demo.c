/**
 * @file component_demo.c
 * @brief Component layer demonstration
 *
 * Shows how to use the component framework with middleware components.
 */

#include "bsp.h"
#include "component.h"
#include "driver.h"
#include "hal.h"
#include "json_codec.h"
#include "modbus_protocol.h"
#include <stdio.h>
#include <string.h>


int main(void) {
  printf("========================================\n");
  printf("  Component Layer Demonstration\n");
  printf("========================================\n\n");

  printf("Architecture:\n");
  printf("  Application -> Component -> Driver -> HAL -> BSP -> Hardware\n\n");

  printf("Dependency Chain:\n");
  printf("  Component depends on Driver only\n");
  printf("  Driver depends on HAL\n");
  printf("  HAL depends on BSP\n");
  printf("  BSP is platform-specific\n\n");

  /* Initialize all layers */
  printf("=== Initialization ===\n");
  init_bsp();
  hal_init();
  driver_init();
  component_init();
  printf("\n");

  /* ========== JSON Codec Demo ========== */
  printf("=== JSON Codec Component ===\n");

  json_codec_config_t json_config;
  json_config.max_depth = 10;
  json_config.max_size = 4096;
  json_codec_init(&json_config);

  /* Create and encode JSON */
  printf("\n--- Encoding JSON ---\n");
  json_object_t obj = json_object_create();
  json_object_add_string(obj, "device", "power_meter");
  json_object_add_int(obj, "voltage", 220);
  json_object_add_int(obj, "current", 10);
  json_object_add_bool(obj, "enabled", 1);

  char json_buffer[512];
  int len = json_object_encode(obj, json_buffer, sizeof(json_buffer));
  printf("Encoded JSON (%d bytes): %s\n", len, json_buffer);

  /* Decode JSON */
  printf("\n--- Decoding JSON ---\n");
  json_object_t decoded = json_object_decode(json_buffer);
  if (decoded) {
    int voltage;
    char device[64];
    json_object_get_int(decoded, "voltage", &voltage);
    json_object_get_string(decoded, "device", device, sizeof(device));
    printf("Decoded: device=%s, voltage=%d\n", device, voltage);
    json_object_delete(decoded);
  }

  json_object_delete(obj);
  printf("\n");

  /* ========== Modbus Protocol Demo ========== */
  printf("=== Modbus Protocol Component ===\n");

  modbus_config_t modbus_config;
  modbus_config.mode = MODBUS_MODE_RTU;
  modbus_config.role = MODBUS_ROLE_MASTER;
  modbus_config.slave_id = 1;
  modbus_config.port_name = "console"; // Use registered UART device
  modbus_config.baudrate = 9600;

  modbus_protocol_init(&modbus_config);

  /* Read holding registers */
  printf("\n--- Modbus Operations ---\n");
  uint16_t registers[10];
  int count = modbus_read_holding_registers(1, 0, 10, registers);
  if (count > 0) {
    printf("Read %d registers: ", count);
    for (int i = 0; i < count; i++) {
      printf("%d ", registers[i]);
    }
    printf("\n");
  }

  /* Write single register */
  modbus_write_single_register(1, 100, 1234);

  /* Write single coil */
  modbus_write_single_coil(1, 0, 1);

  /* Check status */
  component_status_t status = modbus_get_status();
  printf("Modbus status: %d (0=UNINIT, 1=READY, 2=RUNNING, 3=ERROR)\n", status);
  printf("\n");

  /* ========== Component Management ========== */
  printf("=== Component Management ===\n");

  const component_info_t *json_info = component_find("json_codec");
  if (json_info) {
    printf("Found component: %s (v%s, type=%d)\n", json_info->name,
           json_info->version, json_info->type);
  }

  const component_info_t *modbus_info = component_find("modbus_protocol");
  if (modbus_info) {
    printf("Found component: %s (v%s, type=%d)\n", modbus_info->name,
           modbus_info->version, modbus_info->type);
  }

  printf("\n=== Cleanup ===\n");
  modbus_protocol_deinit();
  json_codec_deinit();
  component_deinit();
  driver_deinit();
  hal_deinit();
  deinit_bsp();

  printf("\n========================================\n");
  printf("  Demo completed successfully!\n");
  printf("========================================\n");

  return 0;
}
