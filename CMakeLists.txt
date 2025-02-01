cmake_minimum_required(VERSION 3.23)

project(
    YaYa
    VERSION 1.0
    DESCRIPTION "Yet another Yet Another Programming Language"
    LANGUAGES CXX C)

find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)
find_package(LLVM REQUIRED CONFIG)

set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED TRUE)
set(CMAKE_C_EXTENSIONS OFF)

option(BUILD_TESTING "Build the tests" TRUE)
option(BUILD_LINTING "Run the linting utilities when building" TRUE)

if (BUILD_LINTING)
    find_program(CLANG_TIDY NAMES "clang-tidy")
    if(CLANG_TIDY)
        set(CLANG_TIDY_COMMAND "${CLANG_TIDY}" "-p" "${CMAKE_BINARY_DIR}")
    endif()

    find_program(CPPCHECK NAMES "cppcheck")
    if(CPPCHECK)
      set(
          CPPCHECK_COMMAND
          "${CPPCHECK}"
          "--enable=warning,style,performance")
    endif()
endif()

add_subdirectory(src)
