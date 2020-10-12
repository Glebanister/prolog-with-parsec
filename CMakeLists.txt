cmake_minimum_required(VERSION 3.0.0)
project(prolog-checker)

set(EXE ${CMAKE_PROJECT_NAME})

add_executable(${EXE} src/main.cpp)
target_include_directories(${EXE} PUBLIC extern/parsly/core/)
target_include_directories(${EXE} PUBLIC extern/parsly/Parsnip/)
target_include_directories(${EXE} PUBLIC extern/parsly/)
