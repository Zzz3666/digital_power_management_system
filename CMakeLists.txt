cmake_minimum_required(VERSION 3.10)
project(DigitalPowerManagementSystem C)

set(CMAKE_C_STANDARD 11)

# 生成 compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 添加 BSP 子目录（使用独立的 CMake 配置）
add_subdirectory(bsp)

# 主可执行文件
add_executable(${PROJECT_NAME} main.c)
target_link_libraries(${PROJECT_NAME} PRIVATE ${BSP_LIBRARIES})
