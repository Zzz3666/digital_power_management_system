cmake_minimum_required(VERSION 3.10)
project(DigitalPowerManagementSystem C)

set(CMAKE_C_STANDARD 11)

# 生成 compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(${PROJECT_NAME} main.c)
