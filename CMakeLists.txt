cmake_minimum_required(VERSION 3.23)

project(
    YaYa
    VERSION 1.0
    DESCRIPTION "Yet another Yet Another Programming Language"
    LANGUAGES CXX)

find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)

set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

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
