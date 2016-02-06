#ifndef OGREUTIL_H
#define OGREUTIL_H

#include "Ogre.h"

using namespace Ogre;



class OgreUtil {



 public:

  static void destroyAllAttachedMovableObjects( SceneNode* node );
  static void destroySceneNode( Ogre::SceneNode* node );

};

#endif
