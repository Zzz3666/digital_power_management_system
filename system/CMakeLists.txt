# System Library CMake Configuration
# Provides OS abstraction for third-party RTOS
# System layer depends only on Component layer

cmake_minimum_required(VERSION 3.15)
project(system_lib VERSION 1.0.0 LANGUAGES C)

# Options for selecting RTOS implementation
option(SYSTEM_USE_FREERTOS "Use FreeRTOS as underlying RTOS" ON)
option(SYSTEM_USE_THREADX "Use ThreadX as underlying RTOS" OFF)
option(SYSTEM_USE_RTT "Use RT-Thread as underlying RTOS" OFF)
option(SYSTEM_USE_NONE "Use no RTOS (bare metal)" OFF)

# Collect all source files
set(SYSTEM_SOURCES
    src/system.c
)

# Add OS wrapper based on selection
if(SYSTEM_USE_FREERTOS)
    list(APPEND SYSTEM_SOURCES os/freertos_wrapper.c)
    message(STATUS "System: Using FreeRTOS wrapper")
elseif(SYSTEM_USE_THREADX)
    list(APPEND SYSTEM_SOURCES os/threadx_wrapper.c)
    message(STATUS "System: Using ThreadX wrapper")
elseif(SYSTEM_USE_RTT)
    # list(APPEND SYSTEM_SOURCES os/rtt_wrapper.c)
    message(STATUS "System: Using RT-Thread wrapper (stub)")
elseif(SYSTEM_USE_NONE)
    message(STATUS "System: Bare metal mode (no RTOS)")
else()
    # Default to FreeRTOS
    list(APPEND SYSTEM_SOURCES os/freertos_wrapper.c)
    message(STATUS "System: Using FreeRTOS wrapper (default)")
endif()

# Create static library
add_library(system STATIC ${SYSTEM_SOURCES})

# Set include directories
target_include_directories(system
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Link with Component library (System depends on Component)
if(TARGET component)
    target_link_libraries(system PRIVATE component)
    message(STATUS "System: Linked with Component library")
else()
    message(WARNING "System: Component library not found, some features may not work")
endif()

# Set C standard
target_compile_features(system PUBLIC c_std_11)

# Compiler warnings
if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(system PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Export variables for parent scope
set(SYSTEM_LIBRARIES system PARENT_SCOPE)

# Print configuration summary
message(STATUS "")
message(STATUS "=== System Library Configuration ===")
message(STATUS "Version: ${PROJECT_VERSION}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "RTOS Selection:")
if(SYSTEM_USE_FREERTOS)
    message(STATUS "  - FreeRTOS (selected)")
endif()
if(SYSTEM_USE_THREADX)
    message(STATUS "  - ThreadX (selected)")
endif()
if(SYSTEM_USE_RTT)
    message(STATUS "  - RT-Thread (selected)")
endif()
if(SYSTEM_USE_NONE)
    message(STATUS "  - Bare Metal (selected)")
endif()
message(STATUS "=====================================")
message(STATUS "")

# Example programs (optional)
if(BUILD_EXAMPLES)
    add_executable(system_demo examples/system_demo.c)
    target_link_libraries(system_demo PRIVATE system component driver hal bsp)
endif()
