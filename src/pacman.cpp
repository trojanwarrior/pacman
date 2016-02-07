#include "pacman.h"
#include "Shapes/OgreBulletCollisionsSphereShape.h"
#include "PlayState.h"
#define PACMAN_EAT_ANIM "pakupaku"



using namespace Ogre;
using namespace OgreBulletDynamics;
using namespace OgreBulletCollisions;


static bool HandleContacts(btManifoldPoint& point, btCollisionObject* body0, btCollisionObject* body1){


  PlayState::getSingleton().handleCollision(body0,body1);

  
  return false;

}
extern ContactProcessedCallback gContactProcessedCallback = (ContactProcessedCallback) HandleContacts;
/*
 *Constructor
 * Set lifes to 3 and speed to 1. also create Scene Node for pacman if it doesn't exists.
 */
Pacman::Pacman(DynamicsWorld *_world, Vector3 position,int idNodeStart) {


  std::cout << "Nodo Start Pacman "<< idNodeStart << std::endl;
  SceneManager* _sceneMgr = Root::getSingleton().
                            getSceneManager("SceneManager");
  try  {
    this->node = _sceneMgr->getSceneNode(PACMAN_NODE);
  }
  catch (Exception ex)  {
      this->node = _sceneMgr->createSceneNode(PACMAN_NODE);
       _sceneMgr->getRootSceneNode()->addChild(this->node);
  }
  Entity* pacmanEnt = _sceneMgr->createEntity(PACMAN_NODE,"pacman.mesh");
  pacmanEnt->setCastShadows(true);
  this->node->attachObject(pacmanEnt);
  this->node->scale(0.2, 0.2, 0.2);
  AnimationState* anim = pacmanEnt->getAnimationState(PACMAN_EAT_ANIM);
  anim->setEnabled(true);
  anim->setLoop(true);
  anim->setTimePosition(0.0);


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
  currentNode = idNodeStart;



  btTransform transform; //Declaration of the btTransform
  transform.setIdentity(); //This function put the variable of the object to default. The ctor of btTransform doesnt do it.
  transform.setOrigin(OgreBulletCollisions::OgreBtConverter::to(position)); //Set the new position/origin
  body->getBulletRigidBody()->setWorldTransform(transform); //Apply the btTransform to the body
  body->getBulletRigidBody()->setCollisionFlags(body->getBulletRigidBody()->getCollisionFlags() |
  btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);

  gContactProcessedCallback = (ContactProcessedCallback)HandleContacts;

  this->lifes = 3;
  this->speed = 4;
  this->oldDir = LEFT_DIR;
}
void Pacman::updateAnim(Ogre::Real deltaT){
  Ogre::Entity* pacmanEnt = static_cast<Ogre::Entity*>( node->getAttachedObject(PACMAN_NODE));
  pacmanEnt->getAnimationState(PACMAN_EAT_ANIM)->addTime(deltaT*2);


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
  //Change Direction, rotate Pacman
  if (direction != oldDir) {
    int degrees = (direction - oldDir) *90;
    std::cout << " degrees " << degrees << " " << oldDir << "->"<<direction << std::endl;
    node->yaw(Ogre::Radian(Ogre::Degree(degrees)),Ogre::Node::TS_LOCAL);
    oldDir = direction;
  }



  body->setLinearVelocity( Ogre::Vector3(x,0,z ));

}




/*
 *Return pacman Bullet collision Object
 */
  
btRigidBody* Pacman::getBtRigidBody(){
  return body->getBulletRigidBody();
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

