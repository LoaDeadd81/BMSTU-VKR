cmake_minimum_required(VERSION 3.5)

project(vkr VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_PREFIX_PATH "/usr/lib/qt6")

#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")

add_subdirectory(petri_net/drag)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

include_directories(petri_net anim_elements)

add_executable(vkr
        petri_net/PetriNet.cpp
        petri_net/PetriNet.h
        petri_net/Error.h
        petri_net/Distributions.h
        petri_net/Delft.cpp
        petri_net/Delft.h
        petri_net/PetriNetDrawer.cpp
        petri_net/PetriNetDrawer.h
        petri_net/PetriNetBuilder.cpp
        petri_net/PetriNetBuilder.h
        petri_net/Stats.cpp
        petri_net/Stats.h

        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        group.ui
        group.h
        group.cpp
        reception.ui
        reception.h
        reception.cpp
        windows.ui
        windows.cpp
        windows.h
        result.ui
        result.cpp
        result.h
        anim.ui
        animation.cpp
        animation.h
        DistributionsCFG.h
        anim_elements/BaseAnimItem.h
        anim_elements/EnterAnimItem.cpp
        anim_elements/EnterAnimItem.h
        anim_elements/QueueAnimItem.cpp
        anim_elements/QueueAnimItem.h
        anim_elements/WorkerAnimItem.cpp
        anim_elements/WorkerAnimItem.h
        anim_elements/ReceptionAnimItem.cpp
        anim_elements/ReceptionAnimItem.h
        anim_elements/WindowsGroupAnimItem.cpp
        anim_elements/WindowsGroupAnimItem.h
        anim_elements/ArrowAnimItem.cpp
        anim_elements/ArrowAnimItem.h
        anim_elements/TransactAnimItem.cpp
        anim_elements/TransactAnimItem.h
        anim_elements/Colors.h
)

target_link_libraries(vkr
        Qt::Core
        Qt::Gui
        Qt::Widgets
        drag
)
