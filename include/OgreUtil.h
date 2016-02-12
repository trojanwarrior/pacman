#ifndef OGREUTIL_H
#define OGREUTIL_H

#include "Ogre.h"

using namespace Ogre;



enum collisiontypes {
    COL_NOTHING = 0, //<Collide with nothing
    COL_PACMAN = 2, //<Collide with PACMAN
    COL_PHANTOM =4, //<Collide with PHANTOM
    COL_PILL = 8, //<Collide with PILLS
    COL_WALL = 16, // COLLIDE WITH WALLS
    COL_FLOOR = 32,
    COL_FRUIT = 64 // COLLLIDE WITH FRUITS
};



class OgreUtil {



 public:

  static void destroyAllAttachedMovableObjects( SceneNode* node );
  static void destroySceneNode( Ogre::SceneNode* node );

};

#endif
