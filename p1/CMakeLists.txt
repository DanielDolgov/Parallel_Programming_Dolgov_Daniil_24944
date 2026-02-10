cmake_minimum_required(VERSION 3.14 FATAL_ERROR)
project(p1_proj VERSION 0.0.0 LANGUAGES CXX)


option(FLOAT "Use vector<float>" OFF)
option(DOUBLE "Use vector<double>" OFF)
if(FLOAT)
    add_definitions(-DFLOAT)
elseif (DOUBLE)
    add_definitions(-DDOUBLE)
endif()


add_executable(out p1.cpp)