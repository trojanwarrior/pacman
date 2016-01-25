cmake_minimum_required(VERSION 3.3)
project(pacman)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

######################## AÑADIDO POR MI: varias variables de entorno para pasarle al compilador/linkador
set(MYGUI_SOURCE_DIR "/home/twsh/desarrollo/mygui-src")
set(MYGUI_INSTALL "home/twsh/desarrollo/mygui")
set(MYGUI_ENGINE_INC "${MYGUI_SOURCE_DIR}/MyGUIEngine/include")
set(MYGUI_COMMON_INC "${MYGUI_SOURCE_DIR}/Common")
set(MYGUI_OGREPLATFORM_INC "${MYGUI_SOURCE_DIR}/Platforms/Ogre/OgrePlatform/include")
set(MYGUI_COMMON_BASE_OGRE_INC "${MYGUI_COMMON}/Base/Ogre")
set(OIS_INC "/usr/include/OIS")
set(OGRE_INC "/usr/include/OGRE")
set(OVERLAY_INC "/usr/include/OGRE/Overlay")
#set(MYGUI_INC "-I$(MYGUI_SOURCE_DIR)//MyGUIEngine/include -I$(MYGUI_SOURCE_DIR)/Common -I$(MYGUI_SOURCE_DIR)/Platforms/Ogre/OgrePlatform/include -I$(MYGUI_INSTALL)/Common/Base/Ogre -I$(MYGUI_INSTALL)/Common/Input/OIS -I$(OGRE_INCLUDE_DIR) -I$(OIS_INCLUDE_DIR) -DMYGUI_OGRE_PLATFORM")

#set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -D_DEBUG -pthread -I/usr/include/OIS -I/usr/include/OGRE -I/usr/include/OGRE/Overlay")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -D_DEBUG -pthread -I${OIS_INC} -I${OGRE_INC} -I${OVERLAY_INC} -I${MYGUI_ENGINE_INC} -I${MYGUI_COMMON_BASE_OGRE_INC} -I${MYGUI_COMMON_INC} -I${MYGUI_OGREPLATFORM_INC}")


####################################

set(SOURCE_FILES
        include/GameManager.h
        include/GameState.h
#        include/InputManager.h
        include/IntroState.h
        include/PauseState.h
        include/PlayState.h
        include/graphml_boost.h
        include/ControlsState.h
        include/InputManager_.h
        include/MenuState.h
        include/RecordsState.h
        include/sounds.h
        src/GameManager.cpp
        src/InputManager.cpp
        src/IntroState.cpp
        src/PauseState.cpp
        src/PlayState.cpp
        src/graphml_boost.cpp
        src/main.cpp
        src/ControlsState.cpp
        src/MenuState.cpp
        src/RecordsState.cpp
        src/sounds.cpp)

include_directories(.)
include_directories(include)
include_directories(src)
#include_directories(/usr/include/OIS)
#include_directories(/usr/include/OGRE)
include_directories(${OIS_INC})
include_directories(${OGRE_INC})
include_directories(${OVERLAY_INC})
include_directories(${MYGUI_ENGINE_INC})
include_directories(${MYGUI_COMMON_INC})
include_directories(${MYGUI_OGREPLATFORM_INC})
include_directories(${MYGUI_COMMON_BASE_OGRE_INC})




add_executable(pacman ${SOURCE_FILES})
