cmake_minimum_required(VERSION 3.10)
project(DigitalPowerManagementSystem C)

set(CMAKE_C_STANDARD 11)

# 生成 compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 添加 BSP 子目录（板级支持包 - 底层实现）
add_subdirectory(bsp)

# 添加 HAL 子目录 (硬件抽象层 - 封装 BSP，依赖 BSP)
add_subdirectory(hal)

# 主可执行文件
add_executable(${PROJECT_NAME} main.c)
target_link_libraries(${PROJECT_NAME} PRIVATE ${HAL_LIBRARIES} ${BSP_LIBRARIES})
