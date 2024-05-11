#cmake_minimum_required(VERSION 3.27)
#project(src)
#
#set(CMAKE_CXX_STANDARD 20)
#
#add_subdirectory(drag)
#
#add_executable(petri_nets INTERFACE
#        main.cpp
#        PetriNet.cpp
#        PetriNet.h
#        Error.h
#        Distributions.h
#        Delft.cpp
#        Delft.h
#        PetriNetDrawer.cpp
#        PetriNetDrawer.h
#        PetriNetBuilder.cpp
#        PetriNetBuilder.h
#        Stats.cpp
#        Stats.h
#)
#
##add_library(petri_nets INTERFACE
##        PetriNet.cpp
##        PetriNet.h
##        Error.h
##        Distributions.h
##        Delft.cpp
##        Delft.h
##        PetriNetDrawer.cpp
##        PetriNetDrawer.h
##        PetriNetBuilder.cpp
##        PetriNetBuilder.h
##        Stats.cpp
##        Stats.h
##)
#
#target_link_libraries(petri_nets INTERFACE drag)
