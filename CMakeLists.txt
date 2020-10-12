cmake_minimum_required(VERSION 3.0.0)
project(prolog-checker)

set(EXE ${CMAKE_PROJECT_NAME})
set(Parsly extern/Parsnip_0.23/)

add_executable(${EXE} src/main.cpp)
target_include_directories(${EXE} PUBLIC extern/Parsnip_0.23/)
target_include_directories(${EXE} PUBLIC extern/cxxopts/include/)
