cmake_minimum_required(VERSION 3.18)

project(splitSAM2amplicons
    VERSION 1.0.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


list(APPEND CMAKE_CXX_FLAGS
    "-Werror -Wall -pedantic"
)


include(CheckIPOSupported)
check_ipo_supported(RESULT ipo_supported)
if (ipo_supported)
    message(STATUS "IPO is supported")
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)
else()
    message(STATUS "IPO is not supported")
endif()

#--------------------
# Static Code Analysis
#--------------------
find_program(CLANG_TIDY clang-tidy)
if(${CLANG_TIDY} STREQUAL "CLANG_TIDY-NOTFOUND")
    message("clang-tidy not found. Cannot use target staticanalysis")
else()
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
    add_custom_target(staticanalysis
        COMMAND clang-tidy -p=${CMAKE_BINARY_DIR} -warnings-as-errors=* -checks=bugprone-*,clang-analyzer-*,cppcoreguidelines-*,modernize-*,readability-*,performance-*,portability-*,-cppcoreguidelines-pro-bounds-array-to-pointer-decay,-readability-uppercase-literal-suffix,-cppcoreguidelines-special-member-functions,-cppcoreguidelines-non-private-member-variables-in-classes,-cppcoreguidelines-pro-bounds-constant-array-index `find ${CMAKE_SOURCE_DIR}/src -iname \"*.[ch]pp\"` -extra-arg=-std=c++20)
endif()

add_executable(${CMAKE_PROJECT_NAME} src/main.cpp)

add_subdirectory(src)

