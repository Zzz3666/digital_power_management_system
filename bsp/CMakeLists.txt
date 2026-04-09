# BSP 库 CMake 配置文件
cmake_minimum_required(VERSION 3.10)

# BSP 库选项
option(BSP_PLATFORM_GCC_SIMULATOR "使用 GCC Simulator 平台" ON)

# 收集 BSP 核心源文件
set(BSP_SOURCES
    src/bsp.c
)

# 收集 BSP 头文件目录
set(BSP_INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# 根据选择的平台添加平台特定文件
if(BSP_PLATFORM_GCC_SIMULATOR)
    message(STATUS "BSP: Using GCC Simulator platform")
    
    list(APPEND BSP_SOURCES
        platforms/gcc_simulator/src/bsp_platform.c
    )
    
    list(APPEND BSP_INCLUDE_DIRS
        ${CMAKE_CURRENT_SOURCE_DIR}/platforms/gcc_simulator/include
    )
    
    # 定义平台宏
    add_definitions(-DBSP_PLATFORM_GCC_SIMULATOR)
endif()

# 创建静态库
add_library(bsp STATIC ${BSP_SOURCES})

# 设置库的包含目录
target_include_directories(bsp
    PUBLIC
        ${BSP_INCLUDE_DIRS}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# 设置 C 标准
set_target_properties(bsp PROPERTIES
    C_STANDARD 11
    C_STANDARD_REQUIRED ON
)

# 导出库信息供父项目使用
set(BSP_LIBRARIES bsp PARENT_SCOPE)
set(BSP_INCLUDE_DIRS ${BSP_INCLUDE_DIRS} PARENT_SCOPE)

# 可选：构建 BSP 测试程序
option(BSP_BUILD_TESTS "构建 BSP 测试程序" OFF)
if(BSP_BUILD_TESTS)
    message(STATUS "BSP: Building test program")
    add_executable(bsp_test test_bsp.c)
    target_link_libraries(bsp_test PRIVATE bsp)
endif()

message(STATUS "BSP library configured successfully")
message(STATUS "  Sources: ${BSP_SOURCES}")
message(STATUS "  Include dirs: ${BSP_INCLUDE_DIRS}")
