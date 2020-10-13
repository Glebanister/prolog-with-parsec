cmake_minimum_required(VERSION 3.0.0)
project(prolog-checker)

set(EXE ${CMAKE_PROJECT_NAME})
set(Parsly extern/Parsnip_0.23/)

add_executable(${EXE} src/main.cpp
                      src/PrologChecker.cpp
                      src/ArgumentParser.cpp
                      src/PrologObject.cpp
                      src/PrologObjectPrinter.cpp)

add_executable(test-${EXE} src/TestPrologChecker.cpp
                           src/PrologChecker.cpp
                           src/ArgumentParser.cpp
                           src/PrologObject.cpp
                           src/PrologObjectPrinter.cpp)

target_include_directories(${EXE} PUBLIC include/)
target_include_directories(${EXE} PUBLIC extern/Parsnip_0.23/)
target_include_directories(${EXE} PUBLIC extern/cxxopts/include/)

target_include_directories(test-${EXE} PUBLIC include/)
target_include_directories(test-${EXE} PUBLIC extern/Parsnip_0.23/)
target_include_directories(test-${EXE} PUBLIC extern/cxxopts/include/)

set_property(TARGET ${EXE}
             PROPERTY CXX_STANDARD 17)
set_property(TARGET test-${EXE}
             PROPERTY CXX_STANDARD 17)
