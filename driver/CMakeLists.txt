# Driver Library CMake Configuration
# Provides standard device driver interface (POSIX-like)
# Driver layer depends only on HAL layer

cmake_minimum_required(VERSION 3.15)
project(driver_lib VERSION 1.0.0 LANGUAGES C)

# Options for enabling/disabling specific drivers
option(DRIVER_ENABLE_GPIO "Enable GPIO driver" ON)
option(DRIVER_ENABLE_UART "Enable UART driver" ON)
option(DRIVER_ENABLE_SPI "Enable SPI driver" OFF)
option(DRIVER_ENABLE_I2C "Enable I2C driver" OFF)
option(DRIVER_ENABLE_ADC "Enable ADC driver" OFF)
option(DRIVER_ENABLE_TIMER "Enable Timer driver" OFF)
option(DRIVER_ENABLE_PWM "Enable PWM driver" OFF)

# Collect all source files
set(DRIVER_SOURCES
    src/driver.c
)

# Add device drivers based on options
if(DRIVER_ENABLE_GPIO)
    list(APPEND DRIVER_SOURCES devices/gpio_driver.c)
    message(STATUS "Driver: GPIO driver enabled")
endif()

if(DRIVER_ENABLE_UART)
    list(APPEND DRIVER_SOURCES devices/uart_driver.c)
    message(STATUS "Driver: UART driver enabled")
endif()

if(DRIVER_ENABLE_SPI)
    # list(APPEND DRIVER_SOURCES devices/spi_driver.c)
    message(STATUS "Driver: SPI driver enabled (stub)")
endif()

if(DRIVER_ENABLE_I2C)
    # list(APPEND DRIVER_SOURCES devices/i2c_driver.c)
    message(STATUS "Driver: I2C driver enabled (stub)")
endif()

if(DRIVER_ENABLE_ADC)
    # list(APPEND DRIVER_SOURCES devices/adc_driver.c)
    message(STATUS "Driver: ADC driver enabled (stub)")
endif()

if(DRIVER_ENABLE_TIMER)
    # list(APPEND DRIVER_SOURCES devices/timer_driver.c)
    message(STATUS "Driver: Timer driver enabled (stub)")
endif()

if(DRIVER_ENABLE_PWM)
    # list(APPEND DRIVER_SOURCES devices/pwm_driver.c)
    message(STATUS "Driver: PWM driver enabled (stub)")
endif()

# Create static library
add_library(driver STATIC ${DRIVER_SOURCES})

# Set include directories
target_include_directories(driver
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Link with HAL library (Driver depends on HAL)
if(TARGET hal)
    target_link_libraries(driver PRIVATE hal)
    message(STATUS "Driver: Linked with HAL library")
else()
    message(WARNING "Driver: HAL library not found, some features may not work")
endif()

# Set C standard
target_compile_features(driver PUBLIC c_std_11)

# Compiler warnings
if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(driver PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Export variables for parent scope
set(DRIVER_LIBRARIES driver PARENT_SCOPE)

# Print configuration summary
message(STATUS "")
message(STATUS "=== Driver Library Configuration ===")
message(STATUS "Version: ${PROJECT_VERSION}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Enabled drivers:")
if(DRIVER_ENABLE_GPIO)
    message(STATUS "  - GPIO driver")
endif()
if(DRIVER_ENABLE_UART)
    message(STATUS "  - UART driver")
endif()
if(DRIVER_ENABLE_SPI)
    message(STATUS "  - SPI driver")
endif()
if(DRIVER_ENABLE_I2C)
    message(STATUS "  - I2C driver")
endif()
if(DRIVER_ENABLE_ADC)
    message(STATUS "  - ADC driver")
endif()
if(DRIVER_ENABLE_TIMER)
    message(STATUS "  - Timer driver")
endif()
if(DRIVER_ENABLE_PWM)
    message(STATUS "  - PWM driver")
endif()
message(STATUS "=====================================")
message(STATUS "")

# Example programs (optional)
if(BUILD_EXAMPLES)
    add_executable(driver_demo examples/driver_demo.c)
    target_link_libraries(driver_demo PRIVATE driver hal bsp)
endif()
