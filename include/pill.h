#ifndef PILL_H

#define PILL_H

#include "Ogre.h"
#include "OgreBulletDynamicsRigidBody.h"
#include "OgreBulletDynamicsWorld.h"

using namespace Ogre;
using namespace OgreBulletDynamics;
using namespace OgreBulletCollisions;


class Pill{
 private :

  bool _big;
  Ogre::SceneNode* _node;
  RigidBody* _body;
  CollisionShape* _shape;
  int _id;

 public :
  explicit Pill(DynamicsWorld* _world,Vector3 position, bool big, int id);
  //~Pill();
  //  Pill(Pill& pill);
  //  Pill& operator =(const Pill& pill);
  inline bool isBig(){return _big;};
  inline int getId(){return _id;};
  btRigidBody* getBtRigidBody();
  inline SceneNode* getSceneNode(){return _node;};
};

#endif 

