cmake_minimum_required(VERSION 3.3)
project(pacman)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ftrack-macro-expansion=0 -std=c++11")

#add_custom_target(pacman2 COMMAND make -C ${pacman_SOURCE_DIR}
#        CLION_EXE_DIR=${PROJECT_BINARY_DIR})

######################## AÑADIDO POR MI: varias variables de entorno para pasarle al compilador/linkador
set(MYGUI_SOURCE_DIR "/home/twsh/desarrollo/mygui")
set(MYGUI_INSTALL "/home/twsh/desarrollo/mygui")
set(MYGUI_ENGINE_INC "${MYGUI_SOURCE_DIR}/MyGUIEngine/include")
set(MYGUI_COMMON_INC "${MYGUI_SOURCE_DIR}/Common")
set(MYGUI_OGREPLATFORM_INC "${MYGUI_SOURCE_DIR}/Platforms/Ogre/OgrePlatform/include")
set(MYGUI_OGREPLATFORM_ARGS "-DMYGUI_OGRE_PLATFORM")
set(MYGUI_COMMON_BASE_OGRE_INC "${MYGUI_COMMON_INC}/Base/Ogre")
set(MYGUI_INPUT_OIS "${MYGUI_COMMON_INC}/Input/OIS")
set(SDL_INC "/usr/include/SDL")
set(SDL_ARGS "-D_GNU_SOURCE=1 -D_REENTRANT")
set(MXML_INC "/usr/local/include")
set(MXML_ARGS "-D_THREAD_SAFE -D_REENTRANT")
set(OIS_INC "/usr/include/OIS")
set(OGRE_INC "/usr/include/OGRE")
set(OVERLAY_INC "/usr/include/OGRE/Overlay")
set(BULLET_INC "/usr/include/bullet")
set(OGREBULLET_DIN_INC "/usr/local/include/OgreBullet/Dynamics")
set(OGREBULLET_COL_INC "/usr/local/include/OgreBullet/Collisions")


#set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -D_DEBUG -std=c++11 -pthread -Wno-deprecated -Wno-deprecated-declarations \
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -D_DEBUG -std=c++11 -pthread -Wno-deprecated -Wno-deprecated-declarations \
        -I${OIS_INC} \
        -I${OGRE_INC} \
        -I${OVERLAY_INC} \
        -I${MYGUI_ENGINE_INC} \
        -I${MYGUI_COMMON_BASE_OGRE_INC} \
        -I${MYGUI_COMMON_INC} \
        -I${MYGUI_OGREPLATFORM_INC} ${MYGUI_OGREPLATFORM_ARGS} \
        -I${SDL_INC} ${SDL_ARGS} \
        -I${MXML_INC} ${MXML_ARGS} \
        -I${MYGUI_INPUT_OIS} \
        -I${BULLET_INC} \
        -I${OGREBULLET_DIN_INC} \
        -I${OGREBULLET_COL_INC}")

#set(LDLIBS "-lGL -lOIS -lOgreOverlay -lOgreMain -lboost_system -lxerces-c -lstdc++ \
#    -lboost_graph -lSDL -lSDL_mixer -lmxml -lpthread \
#    -L/usr/local/lib/ -lOgreMain -L${MYGUI_INSTALL}/lib -lCommon -lX11 -lMyGUIEngine -lMyGUI.OgrePlatform ")

#set(LDLIBS "-lOgreOverlay -lOgreMain -lpthread -lboost_system -lMyGUIEngine -lmxml -lSDL -lpthread -lSDL_mixer -lboost_system \
#-lboost_graph -lOIS -lGL -lstdc++ -lMyGUI.OgrePlatform -lBulletSoftBody -lBulletDynamics -lBulletCollision -lLinearMath \
#-lOgreBulletCollisions -lOgreBulletDynamics")

set(LDLIBS "-lOgreOverlay -lOgreMain -lpthread -lboost_system -lMyGUIEngine -lmxml -lSDL -lpthread -lSDL_mixer \
            -lboost_system -lboost_graph -lOIS -lGL -lMyGUI.OgrePlatform  -lBulletSoftBody -lBulletDynamics -lBulletCollision \
            -lLinearMath -lOgreBulletCollisions -lOgreBulletDynamics")

set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${LDLIBS}")
set(CMAKE_EXE_LINKER_FLAGS "${LDLIBS}")
set(CMAKE_MODULE_LINKER_FLAGS_DEBUG "${LDLIBS}")
set(CMAKE_MODULE_LINKER_FLAGS_DEBUG "${LDLIBS}")


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
        include/pacman.h
        include/records.h
        include/phantom.h
        include/phantomFactory.h
        include/OgreUtil.h
        include/pill.h
        include/FruitFactory.h
        include/Fruit.h
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
        src/sounds.cpp
        src/records.cpp
        src/pacman.cpp
        src/phantom.cpp
        src/phantomFactory.cpp
        src/FruitFactory.cpp
        src/Fruit.cpp
        src/ogreUtil.cpp
        src/pill.cpp)

include_directories(.)
include_directories(include)
#include_directories(src)
include_directories(${OIS_INC})
include_directories(${OGRE_INC})
include_directories(${OVERLAY_INC})
include_directories(${MYGUI_ENGINE_INC})
include_directories(${MYGUI_COMMON_INC})
include_directories(${MYGUI_OGREPLATFORM_INC})
include_directories(${MYGUI_COMMON_BASE_OGRE_INC})
include_directories(${SDL_INC})
include_directories(${MXML_INC})
include_directories(BULLET_INC)
include_directories(OGREBULLET_DIN_INC)
include_directories(OGREBULLET_COL_INC)



add_executable(pacman ${SOURCE_FILES})

TARGET_LINK_LIBRARIES(pacman -lOgreOverlay -lOgreMain -lpthread -lboost_system -lMyGUIEngine -lmxml -lSDL -lpthread -lSDL_mixer
                      -lboost_system -lboost_graph -lOIS -lGL -lMyGUI.OgrePlatform  -lBulletSoftBody -lBulletDynamics -lBulletCollision
                      -lLinearMath -lOgreBulletCollisions -lOgreBulletDynamics)