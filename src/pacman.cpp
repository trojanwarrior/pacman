#include "pacman.h"
#include "Shapes/OgreBulletCollisionsSphereShape.h"


using namespace Ogre;
using namespace OgreBulletDynamics;
using namespace OgreBulletCollisions;


/*
 *Constructor
 * Set lifes to 3 and speed to 1. also create Scene Node for pacman if it doesn't exists.
 */
Pacman::Pacman(DynamicsWorld *_world, Vector3 position) {
  SceneManager* _sceneMgr = Root::getSingleton().
                            getSceneManager("SceneManager");
  try  {
    this->node = _sceneMgr->getSceneNode(PACMAN_NODE);

  }
  catch (Exception ex)  {
      this->node = _sceneMgr->createSceneNode(PACMAN_NODE);
       _sceneMgr->getRootSceneNode()->addChild(this->node);

  }
  Entity* pacmanEnt = _sceneMgr->createEntity("pacman.mesh");
  pacmanEnt->setCastShadows(true);
  this->node->attachObject(pacmanEnt);
  this->node->scale(0.2, 0.2, 0.2);

  body = new  RigidBody("pacman", _world);
  shape = new SphereCollisionShape(0.2);
  body->setShape(this->node,
                        shape,
                        0.0,
                        0.0,
                        10,
                        Vector3::ZERO,
                        Quaternion::IDENTITY);
  body->enableActiveState();

  std::cout << position << std::endl;

  btTransform transform; //Declaration of the btTransform
  transform.setIdentity(); //This function put the variable of the object to default. The ctor of btTransform doesnt do it.
  transform.setOrigin(OgreBulletCollisions::OgreBtConverter::to(position)); //Set the new position/origin
  body->getBulletRigidBody()->setWorldTransform(transform); //Apply the btTransform to the body

  this->lifes = 3;
  this->speed = 2;
}


/*
 * Destructor
 */
Pacman::~Pacman() {
  delete body;
  delete shape;
}

/*
 * Get pacman position
 */
const Vector3& Pacman::getPosition() {
  return this->node->getPosition();
}

/*
 * Move pacman in one direction
 */
void Pacman::move(int direction  , double deltaTime) {
  float x = 0,  z = 0;
  float spaceTranslated = this->speed;
  switch (direction) {
    case UP_DIR: z+= spaceTranslated;
      break;
    case DOWN_DIR: z-= spaceTranslated;
      break;
    case LEFT_DIR: x+= spaceTranslated;
      break;
    case RIGHT_DIR: x-= spaceTranslated;
      break;
  }



  body->setLinearVelocity( Ogre::Vector3(x,0,z ));

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

