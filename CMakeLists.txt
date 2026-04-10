cmake_minimum_required(VERSION 3.10)
project(DigitalPowerManagementSystem C)

set(CMAKE_C_STANDARD 11)

# Enable building examples
option(BUILD_EXAMPLES "Build example programs" ON)

# Code coverage option
option(ENABLE_COVERAGE "Enable code coverage testing" OFF)

# 生成 compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 代码覆盖率配置
if(ENABLE_COVERAGE)
    message(STATUS "Code coverage enabled")
    
    # 检查编译器是否支持覆盖率
    if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
        # 添加覆盖率编译选项
        set(COVERAGE_FLAGS "--coverage -fprofile-arcs -ftest-coverage")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${COVERAGE_FLAGS}")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${COVERAGE_FLAGS}")
        
        # 查找 gcovr 工具
        find_program(GCOVR_PATH gcovr)
        if(NOT GCOVR_PATH)
            # 尝试通过 python 模块调用
            find_package(Python3 COMPONENTS Interpreter QUIET)
            if(Python3_Interpreter_FOUND)
                set(GCOVR_PATH ${Python3_EXECUTABLE} -m gcovr)
                message(STATUS "Found gcovr via Python: ${GCOVR_PATH}")
            else()
                message(WARNING "gcovr not found. Install it for coverage reports: pip install gcovr")
            endif()
        else()
            message(STATUS "Found gcovr: ${GCOVR_PATH}")
        endif()
    else()
        message(WARNING "Code coverage is only supported with GCC or Clang")
    endif()
endif()

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

# 主可执行文件 - 只依赖应用层
# 应用层会通过传递性依赖自动链接所有底层库
add_executable(${PROJECT_NAME} main.c)
target_link_libraries(${PROJECT_NAME} PRIVATE ${APPLICATION_LIBRARIES})

# 代码覆盖率测试目标
if(ENABLE_COVERAGE)
    # 创建覆盖率报告目录
    file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/coverage)
    
    # 添加自定义目标用于运行测试并生成覆盖率报告
    add_custom_target(coverage
        COMMAND ${CMAKE_COMMAND} -E echo "Running coverage tests..."
        # 运行主程序（作为示例测试）
        COMMAND ${CMAKE_BINARY_DIR}/${PROJECT_NAME}
        # 运行示例程序
        COMMAND ${CMAKE_BINARY_DIR}/application/app_demo
        COMMAND ${CMAKE_BINARY_DIR}/driver/driver_demo
        COMMAND ${CMAKE_BINARY_DIR}/component/component_demo
        COMMAND ${CMAKE_BINARY_DIR}/system/system_demo
        # 生成 HTML 覆盖率报告
        COMMAND ${GCOVR_PATH}
            --root ${CMAKE_SOURCE_DIR}
            --html-details
            --output ${CMAKE_BINARY_DIR}/coverage/index.html
            --exclude-directories 'build/*'
            --exclude-directories 'examples/*'
            ${CMAKE_BINARY_DIR}
        COMMAND ${CMAKE_COMMAND} -E echo "Coverage report generated: ${CMAKE_BINARY_DIR}/coverage/index.html"
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMENT "Generating code coverage report..."
        VERBATIM
    )
    
    # 添加自定义目标用于生成文本格式的覆盖率报告
    add_custom_target(coverage-text
        COMMAND ${CMAKE_COMMAND} -E echo "Running coverage tests (text report)..."
        COMMAND ${CMAKE_BINARY_DIR}/${PROJECT_NAME}
        COMMAND ${GCOVR_PATH}
            --root ${CMAKE_SOURCE_DIR}
            --txt
            --output ${CMAKE_BINARY_DIR}/coverage/coverage.txt
            --exclude-directories 'build/*'
            --exclude-directories 'examples/*'
            ${CMAKE_BINARY_DIR}
        COMMAND ${CMAKE_COMMAND} -E cat ${CMAKE_BINARY_DIR}/coverage/coverage.txt
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMENT "Generating text coverage report..."
        VERBATIM
    )
    
    message(STATUS "Coverage targets added: 'coverage' (HTML), 'coverage-text' (TXT)")
endif()
