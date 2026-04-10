/**
 * @file component.h
 * @brief Component layer core interface
 *
 * Component layer provides middleware encapsulation for third-party software,
 * making it easier for upper layers to integrate various components.
 */

#ifndef COMPONENT_H
#define COMPONENT_H

#include <stddef.h>
#include <stdint.h>


#ifdef __cplusplus
extern "C" {
#endif

/* Component handle type */
typedef int32_t component_handle_t;

/* Invalid component handle */
#define COMPONENT_INVALID_HANDLE (-1)

/* Component types */
typedef enum {
  COMPONENT_TYPE_PROTOCOL = 0, /* Protocol stack (Modbus, MQTT, etc.) */
  COMPONENT_TYPE_FILESYSTEM,   /* File system (FATFS, LittleFS, etc.) */
  COMPONENT_TYPE_CRYPTO,       /* Cryptography (AES, RSA, etc.) */
  COMPONENT_TYPE_CODEC,        /* Codec (JSON, XML, etc.) */
  COMPONENT_TYPE_ALGORITHM,    /* Algorithm (PID, Filter, etc.) */
  COMPONENT_TYPE_CUSTOM,       /* Custom component */
} component_type_t;

/* Component status */
typedef enum {
  COMPONENT_STATUS_UNINIT = 0,
  COMPONENT_STATUS_READY,
  COMPONENT_STATUS_RUNNING,
  COMPONENT_STATUS_ERROR,
} component_status_t;

/* Component information */
typedef struct {
  const char *name;      /* Component name */
  component_type_t type; /* Component type */
  const char *version;   /* Component version */
  void *private_data;    /* Private data */
} component_info_t;

/* Component API functions */

/**
 * @brief Initialize component layer
 * @return 0 on success, negative error code on failure
 */
int component_init(void);

/**
 * @brief Deinitialize component layer
 * @return 0 on success, negative error code on failure
 */
int component_deinit(void);

/**
 * @brief Register a component
 * @param info Component information
 * @return 0 on success, negative error code on failure
 */
int component_register(const component_info_t *info);

/**
 * @brief Unregister a component
 * @param name Component name
 * @return 0 on success, negative error code on failure
 */
int component_unregister(const char *name);

/**
 * @brief Find component by name
 * @param name Component name
 * @return Component info pointer, NULL if not found
 */
const component_info_t *component_find(const char *name);

/**
 * @brief Get component status
 * @param name Component name
 * @return Component status
 */
component_status_t component_get_status(const char *name);

#ifdef __cplusplus
}
#endif

#endif /* COMPONENT_H */
