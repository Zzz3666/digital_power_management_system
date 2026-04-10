# HAL (Hardware Abstraction Layer) 库 CMake 配置文件
cmake_minimum_required(VERSION 3.10)

# HAL 库选项
option(HAL_ENABLE_GPIO "启用 GPIO 模块" ON)
option(HAL_ENABLE_UART "启用 UART 模块" ON)
option(HAL_ENABLE_SPI "启用 SPI 模块" ON)
option(HAL_ENABLE_I2C "启用 I2C 模块" ON)
option(HAL_ENABLE_ADC "启用 ADC 模块" ON)
option(HAL_ENABLE_TIMER "启用 Timer 模块" ON)
option(HAL_ENABLE_PWM "启用 PWM 模块" ON)

# 收集 HAL 核心源文件
set(HAL_SOURCES
    src/hal.c
    src/hal_gpio.c
    src/hal_uart.c
)

# 收集 HAL 头文件目录
set(HAL_INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# 根据选项添加各模块驱动文件
if(HAL_ENABLE_GPIO)
    message(STATUS "HAL: GPIO module enabled")
    file(GLOB GPIO_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/drivers/gpio/*.c")
    list(APPEND HAL_SOURCES ${GPIO_SOURCES})
endif()

if(HAL_ENABLE_UART)
    message(STATUS "HAL: UART module enabled")
    file(GLOB UART_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/drivers/uart/*.c")
    list(APPEND HAL_SOURCES ${UART_SOURCES})
endif()

if(HAL_ENABLE_SPI)
    message(STATUS "HAL: SPI module enabled")
    file(GLOB SPI_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/drivers/spi/*.c")
    list(APPEND HAL_SOURCES ${SPI_SOURCES})
endif()

if(HAL_ENABLE_I2C)
    message(STATUS "HAL: I2C module enabled")
    file(GLOB I2C_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/drivers/i2c/*.c")
    list(APPEND HAL_SOURCES ${I2C_SOURCES})
endif()

if(HAL_ENABLE_ADC)
    message(STATUS "HAL: ADC module enabled")
    file(GLOB ADC_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/drivers/adc/*.c")
    list(APPEND HAL_SOURCES ${ADC_SOURCES})
endif()

if(HAL_ENABLE_TIMER)
    message(STATUS "HAL: Timer module enabled")
    file(GLOB TIMER_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/drivers/timer/*.c")
    list(APPEND HAL_SOURCES ${TIMER_SOURCES})
endif()

if(HAL_ENABLE_PWM)
    message(STATUS "HAL: PWM module enabled")
    file(GLOB PWM_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/drivers/pwm/*.c")
    list(APPEND HAL_SOURCES ${PWM_SOURCES})
endif()

# 创建静态库
add_library(hal STATIC ${HAL_SOURCES})

# 设置库的包含目录
target_include_directories(hal
    PUBLIC
        ${HAL_INCLUDE_DIRS}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/drivers
)

# 链接 BSP 库 (HAL 依赖 BSP)
if(TARGET bsp)
    target_link_libraries(hal PRIVATE bsp)
    message(STATUS "HAL: Linked with BSP library")
endif()

# 设置 C 标准
set_target_properties(hal PROPERTIES
    C_STANDARD 11
    C_STANDARD_REQUIRED ON
)

# 导出库信息供父项目使用
set(HAL_LIBRARIES hal PARENT_SCOPE)
set(HAL_INCLUDE_DIRS ${HAL_INCLUDE_DIRS} PARENT_SCOPE)

# 可选：构建 HAL 示例程序
option(HAL_BUILD_EXAMPLES "构建 HAL 示例程序" OFF)
if(HAL_BUILD_EXAMPLES)
    message(STATUS "HAL: Building example programs")
    
    add_executable(hal_demo examples/hal_demo.c)
    target_link_libraries(hal_demo PRIVATE hal bsp)
    
    add_executable(hal_bsp_demo examples/hal_bsp_demo.c)
    target_link_libraries(hal_bsp_demo PRIVATE hal bsp)
endif()

message(STATUS "HAL library configured successfully")
message(STATUS "  Sources: ${HAL_SOURCES}")
message(STATUS "  Include dirs: ${HAL_INCLUDE_DIRS}")
