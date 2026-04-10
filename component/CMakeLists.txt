# Component Library CMake Configuration
# Provides middleware encapsulation for third-party software
# Component layer depends only on Driver layer

cmake_minimum_required(VERSION 3.15)
project(component_lib VERSION 1.0.0 LANGUAGES C)

# Options for enabling/disabling specific components
option(COMPONENT_ENABLE_JSON_CODEC "Enable JSON codec component" ON)
option(COMPONENT_ENABLE_MODBUS "Enable Modbus protocol component" ON)
option(COMPONENT_ENABLE_FILESYSTEM "Enable Filesystem component" OFF)
option(COMPONENT_ENABLE_CRYPTO "Enable Cryptography component" OFF)

# Collect all source files
set(COMPONENT_SOURCES
    src/component.c
)

# Add middleware components based on options
if(COMPONENT_ENABLE_JSON_CODEC)
    list(APPEND COMPONENT_SOURCES middleware/json_codec.c)
    message(STATUS "Component: JSON codec enabled")
endif()

if(COMPONENT_ENABLE_MODBUS)
    list(APPEND COMPONENT_SOURCES middleware/modbus_protocol.c)
    message(STATUS "Component: Modbus protocol enabled")
endif()

if(COMPONENT_ENABLE_FILESYSTEM)
    # list(APPEND COMPONENT_SOURCES middleware/filesystem.c)
    message(STATUS "Component: Filesystem enabled (stub)")
endif()

if(COMPONENT_ENABLE_CRYPTO)
    # list(APPEND COMPONENT_SOURCES middleware/crypto.c)
    message(STATUS "Component: Cryptography enabled (stub)")
endif()

# Create static library
add_library(component STATIC ${COMPONENT_SOURCES})

# Set include directories
target_include_directories(component
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Link with Driver library (Component depends on Driver)
if(TARGET driver)
    target_link_libraries(component PRIVATE driver)
    message(STATUS "Component: Linked with Driver library")
else()
    message(WARNING "Component: Driver library not found, some features may not work")
endif()

# Set C standard
target_compile_features(component PUBLIC c_std_11)

# Compiler warnings
if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(component PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Export variables for parent scope
set(COMPONENT_LIBRARIES component PARENT_SCOPE)

# Print configuration summary
message(STATUS "")
message(STATUS "=== Component Library Configuration ===")
message(STATUS "Version: ${PROJECT_VERSION}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Enabled components:")
if(COMPONENT_ENABLE_JSON_CODEC)
    message(STATUS "  - JSON codec")
endif()
if(COMPONENT_ENABLE_MODBUS)
    message(STATUS "  - Modbus protocol")
endif()
if(COMPONENT_ENABLE_FILESYSTEM)
    message(STATUS "  - Filesystem")
endif()
if(COMPONENT_ENABLE_CRYPTO)
    message(STATUS "  - Cryptography")
endif()
message(STATUS "=========================================")
message(STATUS "")

# Example programs (optional)
if(BUILD_EXAMPLES)
    add_executable(component_demo examples/component_demo.c)
    target_link_libraries(component_demo PRIVATE component driver hal bsp)
endif()
