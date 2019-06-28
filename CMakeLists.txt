cmake_minimum_required(VERSION 3.14)
project(labnovo)

set(CMAKE_CXX_STANDARD 11)

add_executable(lab01
        instance.h
        main.cpp
        main.h
        point.h
        solver.cpp
        solver.h)