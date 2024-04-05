cmake_minimum_required(VERSION 3.26.4)
project(project_DA)

set(CMAKE_CXX_STANDARD 14)

include_directories(src)

add_executable(project_DA
        data_structures/Graph.h
        src/main.cpp
        src/Interface.cpp
        src/Interface.h
        src/Printer.h
        src/City.h
        src/City.cpp
        src/Reservoir.cpp
        src/Reservoir.h
        src/Station.h
        src/Station.cpp
        src/FileReader.cpp
        src/FileReader.h
        src/Management.h
        src/Management.cpp)
