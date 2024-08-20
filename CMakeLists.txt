cmake_minimum_required(VERSION 3.7)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/$<CONFIG>/${PROJECT_NAME})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/$<CONFIG>/${PROJECT_NAME})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/$<CONFIG>/${PROJECT_NAME})

project(TimerResolution VERSION 1.0)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)
set(CMAKE_C_EXTENSIONS OFF)

add_compile_definitions(_CRT_SECURE_NO_DEPRECATE)

add_executable(TimerResolution 
    src/main.c
    src/timer_helpers.c
)

target_include_directories(TimerResolution 
    PUBLIC src/
)

target_link_libraries(TimerResolution
    PUBLIC ntdll    
)
