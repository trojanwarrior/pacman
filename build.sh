export MYGUI_INSTALL="/home/jalcolea/VIDEOJUEGOS/MYGUI/mygui"
export MYGUI_SOURCE_DIR="/home/jalcolea/VIDEOJUEGOS/MYGUI/mygui"
export OGRE_INCLUDE_DIR="/usr/include/OGRE"
export OIS_INCLUDE_DIR="/usr/include/ois"

make

cp -r ./mygui/Media/PACMAN ${MYGUI_SOURCE_DIR}/Media
cp -r ./mygui/Media/MyGUI_Media/* ${MYGUI_SOURCE_DIR}/Media/MyGUI_Media
