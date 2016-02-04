/*
 * Pacman Class, it stores lifes
 *
 *
 *
 */
#ifndef PACMAN_H
#define PACMAN_H
#define UP_DIR 1
#define DOWN_DIR 2
#define LEFT_DIR 3
#define RIGHT_DIR 4
#define PACMAN_NODE "pacman"
#include "Ogre.h"
#include "OgreBulletDynamicsRigidBody.h"
#include "OgreBulletDynamicsWorld.h"

using namespace Ogre;
using namespace OgreBulletDynamics;
using namespace OgreBulletCollisions;

class Pacman{
 private :

  float speed;
  int lifes;
  Ogre::SceneNode* node;
  RigidBody* body;
  CollisionShape* shape;
  void createFloor();
  void createLevel();
  void createLight();

 public :
  const Ogre::Vector3& getPosition();
  explicit Pacman(DynamicsWorld* _world,Vector3 position);
  ~Pacman();
  void move(int direction  , double deltaTime);
  int kill();
  int getLifes();
};

#endif 
