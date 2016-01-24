# --------------------------------------------------------------------
# Makefile Genérico :: Módulo 2. Curso Experto Desarrollo Videojuegos
# Carlos González Morcillo     Escuela Superior de Informática (UCLM)
# --------------------------------------------------------------------
EXEC := pacman

DIRSRC := src/
DIROBJ := obj/
DIRHEA := include/
#Añadido por mi para compilar en Fedora
#DIRHEACEGUI1 := /usr/include/cegui-0/CEGUI
#DIRHEACEGUI2 := /usr/include/cegui-0
#LIBCEGUIBASE := CEGUIBase-0
#LIBCEGUIOGRERENDERER := CEGUIOgreRenderer-0    #-lCEGUIBase-0 -lCEGUIOgreRenderer-0

CXX := g++

#START MYGUI
MYGUI_INC :=-I$(MYGUI_SOURCE_DIR)/MyGUIEngine/include -I$(MYGUI_INSTALL)/Common/Base/Ogre -I$(MYGUI_SOURCE_DIR)/Common/ -I$(MYGUI_SOURCE_DIR)/Platforms/Ogre/OgrePlatform/include -I$(MYGUI_INSTALL)/Common/Input/OIS -I$(OGRE_INCLUDE_DIR) -I$(OIS_INCLUDE_DIR) -DMYGUI_OGRE_PLATFORM
MYGUI_LINK=-L/usr/lib/i386-linux-gnu/ -lOgreMain -L$(MYGUI_INSTALL)/lib -lboost_system -lCommon -lX11 -lMyGUIEngine -lOIS -lMyGUI.OgrePlatform
#SDL_LINK=-lSDL -lSDL_image -lSDL_ttf -lSDL_mixer -lmxml -lpthread
SDL_LINK=-lSDL -lSDL_mixer -lmxml -lpthread
#END MYGUI
# Flags de compilación -----------------------------------------------
#CXXFLAGS := -I$(DIRHEA) -I$(DIRHEACEGUI1) -I$(DIRHEACEGUI2) -Wall `pkg-config --cflags OIS OGRE`  
CXXFLAGS := -I$(DIRHEA) -Wall `pkg-config --cflags OIS OGRE OGRE-Overlay` $(MYGUI_INC)

# Flags del linker ---------------------------------------------------
LDFLAGS := `pkg-config --libs-only-L OGRE`
#LDLIBS := `pkg-config --libs-only-l gl OIS OGRE xerces-c` -lstdc++ -lboost_system -l$(LIBCEGUIBASE) -l$(LIBCEGUIOGRERENDERER)
LDLIBS := `pkg-config --libs-only-l gl OIS OGRE OGRE-Overlay xerces-c` -lstdc++ -lboost_system $(MYGUI_LINK) $(SDL_LINK)

# Modo de compilación (-mode=release -mode=debug) --------------------
ifeq ($(mode), release) 
	CXXFLAGS += -O2 -D_RELEASE
else 
	CXXFLAGS += -g -D_DEBUG
	mode := debug
endif

CXXFLAGS += -Wno-deprecated -Wno-deprecated-declarations

# Obtención automática de la lista de objetos a compilar -------------
OBJS := $(subst $(DIRSRC), $(DIROBJ), \
	$(patsubst %.cpp, %.o, $(wildcard $(DIRSRC)*.cpp)))

.PHONY: all clean

all: info dirs $(EXEC)

info:
	@echo '------------------------------------------------------'
	@echo '>>> Using mode $(mode)'
	@echo '    (Please, call "make" with [mode=debug|release])  '
	@echo '------------------------------------------------------'

dirs:
	mkdir -p $(DIROBJ)

# Enlazado -----------------------------------------------------------
$(EXEC): $(OBJS)
	$(CXX) $(LDFLAGS) -o $@ $^ $(LDLIBS)

# Compilación --------------------------------------------------------
$(DIROBJ)%.o: $(DIRSRC)%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Limpieza de temporales ---------------------------------------------
clean:
	rm -f *.log $(EXEC) *~ $(DIRSRC)*~ $(DIRHEA)*~ 
	rm -rf $(DIROBJ)
