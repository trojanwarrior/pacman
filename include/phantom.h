/*
 * Phantom Class, it stores lifes
 *
 *
 *
 */
#ifndef PHANTOM_H
#define PHANTOM_H
#define UP_DIR 1
#define DOWN_DIR 2
#define LEFT_DIR 3
#define RIGHT_DIR 4
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

  string name;

  RigidBody* body;
  CollisionShape* shape;


 public :
  const Ogre::Vector3& getPosition();
  explicit Phantom(DynamicsWorld* _world,Vector3 position,string _name, float _speed, float _smart,string material);
  ~Phantom();
  Phantom(const Phantom &phantom);
  void move(int direction);

};

#endif 
