/**
 * @file json_codec.h
 * @brief JSON codec component
 * 
 * Provides JSON encoding/decoding functionality.
 * This is a wrapper for third-party JSON libraries (e.g., cJSON).
 */

#ifndef JSON_CODEC_H
#define JSON_CODEC_H

#include "component.h"
#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/* JSON object handle */
typedef void* json_object_t;

/* JSON value types */
typedef enum {
    JSON_TYPE_NULL = 0,
    JSON_TYPE_BOOL,
    JSON_TYPE_NUMBER,
    JSON_TYPE_STRING,
    JSON_TYPE_ARRAY,
    JSON_TYPE_OBJECT,
} json_type_t;

/* JSON codec configuration */
typedef struct {
    size_t max_depth;       /* Maximum nesting depth */
    size_t max_size;        /* Maximum JSON string size */
} json_codec_config_t;

/* JSON codec API */

/**
 * @brief Initialize JSON codec component
 * @param config Configuration parameters
 * @return 0 on success, negative error code on failure
 */
int json_codec_init(const json_codec_config_t *config);

/**
 * @brief Deinitialize JSON codec component
 */
void json_codec_deinit(void);

/**
 * @brief Create a new JSON object
 * @return JSON object handle, NULL on failure
 */
json_object_t json_object_create(void);

/**
 * @brief Delete a JSON object
 * @param obj JSON object handle
 */
void json_object_delete(json_object_t obj);

/**
 * @brief Add integer to JSON object
 * @param obj JSON object handle
 * @param key Key name
 * @param value Integer value
 * @return 0 on success, negative error code on failure
 */
int json_object_add_int(json_object_t obj, const char *key, int value);

/**
 * @brief Add string to JSON object
 * @param obj JSON object handle
 * @param key Key name
 * @param value String value
 * @return 0 on success, negative error code on failure
 */
int json_object_add_string(json_object_t obj, const char *key, const char *value);

/**
 * @brief Add boolean to JSON object
 * @param obj JSON object handle
 * @param key Key name
 * @param value Boolean value
 * @return 0 on success, negative error code on failure
 */
int json_object_add_bool(json_object_t obj, const char *key, int value);

/**
 * @brief Encode JSON object to string
 * @param obj JSON object handle
 * @param buffer Output buffer
 * @param size Buffer size
 * @return Number of bytes written, negative error code on failure
 */
int json_object_encode(json_object_t obj, char *buffer, size_t size);

/**
 * @brief Decode JSON string to object
 * @param json_str JSON string
 * @return JSON object handle, NULL on failure
 */
json_object_t json_object_decode(const char *json_str);

/**
 * @brief Get integer value from JSON object
 * @param obj JSON object handle
 * @param key Key name
 * @param value Output value
 * @return 0 on success, negative error code on failure
 */
int json_object_get_int(json_object_t obj, const char *key, int *value);

/**
 * @brief Get string value from JSON object
 * @param obj JSON object handle
 * @param key Key name
 * @param value Output buffer
 * @param size Buffer size
 * @return 0 on success, negative error code on failure
 */
int json_object_get_string(json_object_t obj, const char *key, char *value, size_t size);

#ifdef __cplusplus
}
#endif

#endif /* JSON_CODEC_H */
