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
  int startNode;
  int idDestiny;
  double distanceToDestiny;


  string name;

  RigidBody* body;
  CollisionShape* shape;
  void calculateNewDestiny();


 public :
  const Ogre::Vector3& getPosition();
  explicit Phantom(DynamicsWorld* _world,Vector3 position,string _name, float _speed, float _smart,string material,int origin);
  inline RigidBody* getBody(){return body;};
  void checkMove();
  //  ~Phantom();
  //Phantom(const Phantom &phantom);


};

#endif 
