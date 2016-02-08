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

enum class estadoPhantom : int {NORMAL,ACOJONADO,MUERTO};

class Phantom{

 private :

  float speed;
  float smart;
  int idOrigin;
  int startNode;
  int idDestiny;
  double distanceToDestiny;
  bool afraid;
  string orgMaterial;
  estadoPhantom estado;
  string name;

  RigidBody* body;
  CollisionShape* shape;
  void calculateNewDestiny();
  Vector3  getBulletPosition();
  
  void setNormal();
  void setMuerto();
  void setAfraid();

public :
  const Ogre::Vector3& getPosition();
  explicit Phantom(DynamicsWorld* _world,Vector3 position,string _name, float _speed, float _smart,string material,int origin);
  inline RigidBody* getBody(){return body;};
  inline btRigidBody* getBtRigidBody(){return body->getBulletRigidBody();};
  void checkMove(Ogre::Real deltaT);
  void reset();
  inline bool isAfraid(){return afraid;};
  //void setAfraid(bool _afraid);
  void changeStatePhantom(estadoPhantom estado);
  estadoPhantom getEstado();
  //  ~Phantom();
  //Phantom(const Phantom &phantom);

};

#endif 
