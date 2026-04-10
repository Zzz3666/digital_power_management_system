# HAL (Hardware Abstraction Layer) 库 CMake 配置文件
cmake_minimum_required(VERSION 3.10)

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

# 创建静态库
add_library(hal STATIC ${HAL_SOURCES})

# 设置库的包含目录
target_include_directories(hal
    PUBLIC
        ${HAL_INCLUDE_DIRS}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
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
