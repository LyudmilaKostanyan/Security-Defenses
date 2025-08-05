cmake_minimum_required(VERSION 3.14)
project(Security-Defenses)

set(CMAKE_CXX_STANDARD 17)

add_executable(dep dep.cpp)
add_executable(aslr aslr.cpp)