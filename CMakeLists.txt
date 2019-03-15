project (cbig)
cmake_minimum_required(VERSION 3.11.4)

set(TARGET_GROUP main CACHE STRING "Group to build")

find_program(
    CMAKE_C_CPPCHECK
    NAMES "cppcheck"
    DOC "Path to cppcheck executable"
)
if(NOT CMAKE_C_CPPCHECK)
    message(FATAL_ERROR "cppcheck not found.")
else()
    message(STATUS "cppcheck found: ${CMAKE_C_CPPCHECK}")
    set(CMAKE_C_CPPCHECK "${CMAKE_C_CPPCHECK}" --enable=warning,performance,portability,style --error-exitcode=1 --force --quiet)
endif()

add_compile_options(-DuECC_VLI_NATIVE_LITTLE_ENDIAN=0 -std=c99 -Wall -Wextra  -O0 -g --coverage -fsanitize=address -fno-omit-frame-pointer)
SET(CMAKE_EXE_LINKER_FLAGS "-DuECC_VLI_NATIVE_LITTLE_ENDIAN=0 -std=c99 -Wall -Wextra  -O0 -g --coverage -fsanitize=address -fno-omit-frame-pointer ${CMAKE_EXE_LINKER_FLAGS}")
add_subdirectory(src)    
include(CTest)
add_subdirectory(tests)
