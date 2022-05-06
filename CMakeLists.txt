cmake_minimum_required(VERSION 3.18)

project(comp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT MSVC)
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g -ftrapv")
set(CMAKE_C_FLAGS_DEBUG "-O0 -g -ftrapv")
else()
set(CMAKE_CXX_FLAGS_DEBUG "/DEBUG /Zi")
set(CMAKE_C_FLAGS_DEBUG "/DEBUG /Zi")
endif()

add_subdirectory(src)
# enable_testing()
# add_subdirectory(tests)
