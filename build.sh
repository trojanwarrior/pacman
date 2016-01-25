export MYGUI_INSTALL="/home/twsh/desarrollo/mygui"
export MYGUI_SOURCE_DIR="/home/twsh/desarrollo/mygui-src"
export OGRE_INCLUDE_DIR="/usr/include/OGRE"
export OIS_INCLUDE_DIR="/usr/include/OIS"

make

cp -r ./mygui/Media/PACMAN ${MYGUI_SOURCE_DIR}/Media
cp -r ./mygui/Media/MyGUI_Media/* ${MYGUI_SOURCE_DIR}/Media/MyGUI_Media
