#ifndef PILL_H

#define PILL_H

#include "Ogre.h"

#include "OgreBulletDynamicsRigidBody.h"
#include "OgreBulletDynamicsWorld.h"

using namespace Ogre;
using namespace OgreBulletDynamics;
using namespace OgreBulletCollisions;
/*
enum collisiontypes {
    COL_NOTHING = 0, //<Collide with nothing
    COL_PACMAN = 2, //<Collide with PACMAN
    COL_PHANTOM =4, //<Collide with PHANTOM
    COL_PILL = 8, //<Collide with PILLS
    COL_WALL = 16 // COLLIDE WITH WALLS
};
*/

class Pill{
 private :

  bool _big;
  Ogre::SceneNode* _node;
  RigidBody* _body;
  CollisionShape* _shape;
  int _id;
  int _idNode;

 public :
  explicit Pill(DynamicsWorld* _world,Vector3 position, bool big, int id,int idNode);
  //~Pill();
  //  Pill(Pill& pill);
  //  Pill& operator =(const Pill& pill);
  inline bool isBig(){return _big;};
  inline int getId(){return _id;};
  btRigidBody* getBtRigidBody();
  inline SceneNode* getSceneNode(){return _node;};
  inline int getIdNode(){return _idNode;};

};

#endif 

