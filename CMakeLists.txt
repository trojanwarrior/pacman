cmake_minimum_required(VERSION 3.3)
project(pacman)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
        include/GameManager.h
        include/GameState.h
        include/InputManager.h
        include/IntroState.h
        include/PauseState.h
        include/PlayState.h
        include/graphml_boost.h
        src/GameManager.cpp
        src/InputManager.cpp
        src/IntroState.cpp
        src/PauseState.cpp
        src/PlayState.cpp
        src/graphml_boost.cpp
        src/main.cpp)

include_directories(.)
include_directories(include)
include_directories(src)
include_directories(/usr/include/OIS)

add_executable(pacman ${SOURCE_FILES})
