cmake_minimum_required(VERSION 3.27)
project(src)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(drag)

add_executable(src main.cpp
        PetriNet.cpp
        PetriNet.h
        Error.h
        Distributions.h
        Delft.cpp
        Delft.h
        PetriNetDrawer.cpp
        PetriNetDrawer.h
        PetriNetConstructor.cpp
        PetriNetConstructor.h
)

target_link_libraries(src drag)
