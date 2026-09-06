cmake_minimum_required (VERSION 3.10)

project ("numeric-methods")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../dist/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../dist/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../dist/bin)

if (LINUX)
    list(APPEND COMMON_DEPS pthread m)
endif()

if (CMAKE_COMPILER_IS_GNUCXX)
    add_definitions (-Wall -pedantic)
endif()

if(MSVC)
    add_definitions (/W4)
    add_compile_definitions(_USE_MATH_DEFINES)
endif()

add_subdirectory ("errors")
