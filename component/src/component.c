/**
 * @file component.c
 * @brief Component layer implementation
 *
 * Implements the component management framework.
 */

#include "component.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/* Maximum number of components */
#define MAX_COMPONENTS 16

/* Component table */
typedef struct {
  component_info_t info;
  component_status_t status;
  int registered;
} component_entry_t;

static component_entry_t component_table[MAX_COMPONENTS];
static int component_count = 0;
static int initialized = 0;

int component_init(void) {
  printf("[COMPONENT] Initializing component layer...\n");

  memset(component_table, 0, sizeof(component_table));
  component_count = 0;
  initialized = 1;

  printf("[COMPONENT] Component layer initialized\n");
  return 0;
}

int component_deinit(void) {
  printf("[COMPONENT] Deinitializing component layer...\n");

  /* Unregister all components */
  for (int i = 0; i < component_count; i++) {
    if (component_table[i].registered) {
      component_unregister(component_table[i].info.name);
    }
  }

  initialized = 0;
  printf("[COMPONENT] Component layer deinitialized\n");
  return 0;
}

int component_register(const component_info_t *info) {
  if (!info || !info->name) {
    return -1;
  }

  if (!initialized) {
    printf("[COMPONENT] Error: Component layer not initialized\n");
    return -2;
  }

  if (component_count >= MAX_COMPONENTS) {
    printf("[COMPONENT] Error: Component table full\n");
    return -3;
  }

  /* Check if component already exists */
  for (int i = 0; i < component_count; i++) {
    if (strcmp(component_table[i].info.name, info->name) == 0) {
      printf("[COMPONENT] Error: Component '%s' already registered\n",
             info->name);
      return -4;
    }
  }

  /* Register component */
  component_table[component_count].info.name = info->name;
  component_table[component_count].info.type = info->type;
  component_table[component_count].info.version =
      info->version ? info->version : "unknown";
  component_table[component_count].info.private_data = info->private_data;
  component_table[component_count].status = COMPONENT_STATUS_READY;
  component_table[component_count].registered = 1;
  component_count++;

  printf("[COMPONENT] Component '%s' (v%s) registered successfully\n",
         info->name, info->version ? info->version : "unknown");
  return 0;
}

int component_unregister(const char *name) {
  if (!name) {
    return -1;
  }

  for (int i = 0; i < component_count; i++) {
    if (strcmp(component_table[i].info.name, name) == 0) {
      /* Shift remaining components */
      for (int j = i; j < component_count - 1; j++) {
        component_table[j] = component_table[j + 1];
      }
      component_count--;
      printf("[COMPONENT] Component '%s' unregistered\n", name);
      return 0;
    }
  }

  printf("[COMPONENT] Error: Component '%s' not found\n", name);
  return -2;
}

const component_info_t *component_find(const char *name) {
  if (!name || !initialized) {
    return NULL;
  }

  for (int i = 0; i < component_count; i++) {
    if (strcmp(component_table[i].info.name, name) == 0) {
      return &component_table[i].info;
    }
  }

  return NULL;
}

component_status_t component_get_status(const char *name) {
  if (!name || !initialized) {
    return COMPONENT_STATUS_UNINIT;
  }

  for (int i = 0; i < component_count; i++) {
    if (strcmp(component_table[i].info.name, name) == 0) {
      return component_table[i].status;
    }
  }

  return COMPONENT_STATUS_UNINIT;
}
