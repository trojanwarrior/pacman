/*
 * Phantom Class, it stores lifes
 *
 *
 *
 */
#ifndef PHANTOM_H
#define PHANTOM_H

#include "Ogre.h"
#include "OgreBulletDynamicsRigidBody.h"
#include "OgreBulletDynamicsWorld.h"
#include <string>

using namespace std;
using namespace Ogre;
using namespace OgreBulletDynamics;
using namespace OgreBulletCollisions;

class Phantom{
 private :

  float speed;
  float smart;
  int idOrigin;

  string name;

  RigidBody* body;
  CollisionShape* shape;


 public :
  const Ogre::Vector3& getPosition();
  explicit Phantom(DynamicsWorld* _world,Vector3 position,string _name, float _speed, float _smart,string material,int origin);
  //  ~Phantom();
  //Phantom(const Phantom &phantom);
  void move(int direction);

};

#endif 
