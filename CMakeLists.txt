cmake_minimum_required(VERSION 3.10)
project(DigitalPowerManagementSystem C)

set(CMAKE_C_STANDARD 11)

# Enable building examples
option(BUILD_EXAMPLES "Build example programs" ON)

# 生成 compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 添加 BSP 子目录（板级支持包 - 底层实现）
add_subdirectory(bsp)

# 添加 HAL 子目录 (硬件抽象层 - 封装 BSP，依赖 BSP)
add_subdirectory(hal)

# 添加 Driver 子目录 (驱动层 - 只依赖 HAL)
add_subdirectory(driver)

# 添加 Component 子目录 (组件层 - 只依赖 Driver)
add_subdirectory(component)

# 添加 System 子目录 (系统层 - 只依赖 Component)
add_subdirectory(system)

# 添加 Application 子目录 (应用层 - 只依赖 System)
add_subdirectory(application)

# 主可执行文件
add_executable(${PROJECT_NAME} main.c)
target_link_libraries(${PROJECT_NAME} PRIVATE ${APPLICATION_LIBRARIES} ${SYSTEM_LIBRARIES} ${COMPONENT_LIBRARIES} ${DRIVER_LIBRARIES} ${HAL_LIBRARIES} ${BSP_LIBRARIES})
