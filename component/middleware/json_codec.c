/**
 * @file json_codec.c
 * @brief JSON codec component implementation
 *
 * Simulates JSON encoding/decoding functionality.
 * In real implementation, this would wrap cJSON or similar library.
 */

#include "json_codec.h"
#include "component.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Simple JSON object structure for simulation */
typedef struct {
  char key[64];
  char value[256];
  json_type_t type;
} json_pair_t;

typedef struct {
  json_pair_t pairs[32];
  int count;
} json_object_impl_t;

static json_codec_config_t codec_config = {0};
static int codec_initialized = 0;

int json_codec_init(const json_codec_config_t *config) {
  printf("[JSON-CODEC] Initializing JSON codec component...\n");

  if (config) {
    codec_config = *config;
  } else {
    codec_config.max_depth = 10;
    codec_config.max_size = 4096;
  }

  codec_initialized = 1;

  /* Register as component */
  component_info_t info;
  info.name = "json_codec";
  info.type = COMPONENT_TYPE_CODEC;
  info.version = "1.0.0";
  info.private_data = NULL;
  component_register(&info);

  printf("[JSON-CODEC] JSON codec initialized (max_depth=%zu, max_size=%zu)\n",
         codec_config.max_depth, codec_config.max_size);
  return 0;
}

void json_codec_deinit(void) {
  printf("[JSON-CODEC] Deinitializing JSON codec component...\n");
  codec_initialized = 0;
  component_unregister("json_codec");
}

json_object_t json_object_create(void) {
  if (!codec_initialized) {
    printf("[JSON-CODEC] Error: Codec not initialized\n");
    return NULL;
  }

  json_object_impl_t *obj =
      (json_object_impl_t *)malloc(sizeof(json_object_impl_t));
  if (obj) {
    memset(obj, 0, sizeof(json_object_impl_t));
    printf("[JSON-CODEC] Created new JSON object\n");
  }
  return (json_object_t)obj;
}

void json_object_delete(json_object_t obj) {
  if (obj) {
    free(obj);
    printf("[JSON-CODEC] Deleted JSON object\n");
  }
}

int json_object_add_int(json_object_t obj, const char *key, int value) {
  if (!obj || !key) {
    return -1;
  }

  json_object_impl_t *impl = (json_object_impl_t *)obj;
  if (impl->count >= 32) {
    return -2;
  }

  strncpy(impl->pairs[impl->count].key, key, sizeof(impl->pairs[0].key) - 1);
  snprintf(impl->pairs[impl->count].value, sizeof(impl->pairs[0].value), "%d",
           value);
  impl->pairs[impl->count].type = JSON_TYPE_NUMBER;
  impl->count++;

  printf("[JSON-CODEC] Added int: %s = %d\n", key, value);
  return 0;
}

int json_object_add_string(json_object_t obj, const char *key,
                           const char *value) {
  if (!obj || !key || !value) {
    return -1;
  }

  json_object_impl_t *impl = (json_object_impl_t *)obj;
  if (impl->count >= 32) {
    return -2;
  }

  strncpy(impl->pairs[impl->count].key, key, sizeof(impl->pairs[0].key) - 1);
  snprintf(impl->pairs[impl->count].value, sizeof(impl->pairs[0].value),
           "\"%s\"", value);
  impl->pairs[impl->count].type = JSON_TYPE_STRING;
  impl->count++;

  printf("[JSON-CODEC] Added string: %s = \"%s\"\n", key, value);
  return 0;
}

int json_object_add_bool(json_object_t obj, const char *key, int value) {
  if (!obj || !key) {
    return -1;
  }

  json_object_impl_t *impl = (json_object_impl_t *)obj;
  if (impl->count >= 32) {
    return -2;
  }

  strncpy(impl->pairs[impl->count].key, key, sizeof(impl->pairs[0].key) - 1);
  snprintf(impl->pairs[impl->count].value, sizeof(impl->pairs[0].value),
           value ? "true" : "false");
  impl->pairs[impl->count].type = JSON_TYPE_BOOL;
  impl->count++;

  printf("[JSON-CODEC] Added bool: %s = %s\n", key, value ? "true" : "false");
  return 0;
}

int json_object_encode(json_object_t obj, char *buffer, size_t size) {
  if (!obj || !buffer || size == 0) {
    return -1;
  }

  json_object_impl_t *impl = (json_object_impl_t *)obj;
  int offset = 0;

  offset += snprintf(buffer + offset, size - offset, "{");

  for (int i = 0; i < impl->count; i++) {
    if (i > 0) {
      offset += snprintf(buffer + offset, size - offset, ", ");
    }

    if (impl->pairs[i].type == JSON_TYPE_STRING) {
      offset += snprintf(buffer + offset, size - offset, "\"%s\": %s",
                         impl->pairs[i].key, impl->pairs[i].value);
    } else {
      offset += snprintf(buffer + offset, size - offset, "\"%s\": %s",
                         impl->pairs[i].key, impl->pairs[i].value);
    }
  }

  offset += snprintf(buffer + offset, size - offset, "}");

  printf("[JSON-CODEC] Encoded JSON: %s\n", buffer);
  return offset;
}

json_object_t json_object_decode(const char *json_str) {
  if (!json_str) {
    return NULL;
  }

  printf("[JSON-CODEC] Decoding JSON: %s\n", json_str);

  /* Simplified decoding - just create a dummy object */
  json_object_t obj = json_object_create();
  if (obj) {
    json_object_add_string(obj, "raw", json_str);
  }

  return obj;
}

int json_object_get_int(json_object_t obj, const char *key, int *value) {
  if (!obj || !key || !value) {
    return -1;
  }

  json_object_impl_t *impl = (json_object_impl_t *)obj;

  for (int i = 0; i < impl->count; i++) {
    if (strcmp(impl->pairs[i].key, key) == 0 &&
        impl->pairs[i].type == JSON_TYPE_NUMBER) {
      *value = atoi(impl->pairs[i].value);
      printf("[JSON-CODEC] Got int: %s = %d\n", key, *value);
      return 0;
    }
  }

  return -2;
}

int json_object_get_string(json_object_t obj, const char *key, char *value,
                           size_t size) {
  if (!obj || !key || !value || size == 0) {
    return -1;
  }

  json_object_impl_t *impl = (json_object_impl_t *)obj;

  for (int i = 0; i < impl->count; i++) {
    if (strcmp(impl->pairs[i].key, key) == 0) {
      strncpy(value, impl->pairs[i].value, size - 1);
      value[size - 1] = '\0';
      printf("[JSON-CODEC] Got string: %s = %s\n", key, value);
      return 0;
    }
  }

  return -2;
}
