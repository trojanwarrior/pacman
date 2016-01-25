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


class Pacman{
 private :

  float speed;
  int lifes;
  Ogre::SceneNode* node;

 public :
  const Ogre::Vector3& getPosition();
  Pacman();
  ~Pacman();
  void move(int direction  , double deltaTime);
  int kill();
  int getLifes();
};

#endif 