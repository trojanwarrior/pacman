#include "pacman.h"

/*
 *Constructor
 * Set lifes to 3 and speed to 1. also create Scene Node for pacman if it doesn't exists.
 */
Pacman::Pacman() {
  Ogre::SceneManager* _sceneMgr = Ogre::Root::getSingleton().
                                 getSceneManager("SceneManager");
  try  {
    this->node = _sceneMgr->getSceneNode(PACMAN_NODE);
  }
  catch (Ogre::Exception ex)  {
      this->node = _sceneMgr->createSceneNode(PACMAN_NODE);
       _sceneMgr->getRootSceneNode()->addChild(this->node);
  }


  Ogre::Entity* pacmanEnt = _sceneMgr->createEntity("pacman.mesh");
  this->node->attachObject(pacmanEnt);
  this->node->scale(0.3, 0.3, 0.3);


  this->lifes = 3;
  this->speed = 1.0;
}


/*
 * Destructor
 */
Pacman::~Pacman(){


}

/*
 * Get pacman position
 */
const Ogre::Vector3& Pacman::getPosition() {
  return this->node->getPosition();
}

/*
 * Move pacman in one direction
 */
void Pacman::move(int direction  , double deltaTime) {
  float x = 0, y = 0, z = 0;
  float spaceTranslated = deltaTime * this->speed;
  switch (direction) {
    case UP_DIR: z+= spaceTranslated;
      break;
    case DOWN_DIR: z-= spaceTranslated;
      break;
    case LEFT_DIR: x-= spaceTranslated;
      break;
    case RIGHT_DIR: x-= spaceTranslated;
      break;
  }
  this->node->translate(x, y, z);
}

/*
 *Kill one pacman's life. return reamining lifes.
 */
int Pacman::kill() {
  this->lifes--;
  return this->lifes;
}

/*
 * Return pacman's remaining lifes
 */
int Pacman::getLifes() {
  return this->lifes;
}

