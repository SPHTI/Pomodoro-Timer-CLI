cmake_minimum_required(VERSION 3.12)
project(Pomidor-cli)

if(MSVC)
    add_compile_options(/utf-8)
    add_compile_definitions(_WIN32_WINNT=0x0601)
endif()

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

add_executable(start main.cpp)

if(WIN32)
    target_link_libraries(start PRIVATE Kernel32.lib)
endif()