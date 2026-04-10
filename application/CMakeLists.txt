# Application Layer CMake Configuration
# This layer depends on System layer

cmake_minimum_required(VERSION 3.15)

# Collect all source files
set(APPLICATION_SOURCES
    src/application.c
)

# Collect all header files for installation
set(APPLICATION_HEADERS
    include/application.h
)

# Create application library
add_library(application STATIC ${APPLICATION_SOURCES})

# Set include directories
target_include_directories(application
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Link with System library (Application depends on System)
if(TARGET system)
    target_link_libraries(application PRIVATE system)
    message(STATUS "Application: Linked with System library")
else()
    message(WARNING "Application: System library not found")
endif()

# Set compile options
target_compile_options(application PRIVATE
    $<$<CONFIG:Debug>:-g -O0>
    $<$<CONFIG:Release>:-O2>
)

# Set C standard
set_target_properties(application PROPERTIES
    C_STANDARD 11
    C_STANDARD_REQUIRED ON
    POSITION_INDEPENDENT_CODE ON
)

# Export variables for parent scope
set(APPLICATION_LIBRARIES application PARENT_SCOPE)
set(APPLICATION_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/include PARENT_SCOPE)

# Installation rules
install(TARGETS application
    EXPORT DigitalPowerManagementSystemTargets
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)

install(FILES ${APPLICATION_HEADERS}
    DESTINATION include/application
)

message(STATUS "Application layer configured successfully")

# Build examples if enabled
if(BUILD_EXAMPLES)
    # Application demo executable
    add_executable(app_demo examples/app_demo.c)
    target_link_libraries(app_demo PRIVATE application system component driver hal bsp)
    
    # Add component layer include directory for app_demo
    target_include_directories(app_demo PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/../component/include
    )
    
    set_target_properties(app_demo PROPERTIES
        OUTPUT_NAME "app_demo"
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/application
    )
    message(STATUS "Application: Built app_demo example")
endif()
