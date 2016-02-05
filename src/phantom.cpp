#include "phantom.h"
#include "Shapes/OgreBulletCollisionsSphereShape.h"
#include <string>


using namespace Ogre;
using namespace std;
using namespace OgreBulletDynamics;
using namespace OgreBulletCollisions;


Phantom::Phantom(const Phantom& phantom){
  name = phantom.name;
  speed = phantom.speed;
  smart = phantom.smart;
  body = phantom.body;
  shape = phantom.shape;


}
/*
 *Constructor
 */
Phantom::Phantom(DynamicsWorld *_world, Vector3 position,string _name, float _speed,float _smart,string material) {
  
  smart = _smart;
  name = _name;
  speed = _speed;
  SceneManager* _sceneMgr = Root::getSingleton().
                            getSceneManager("SceneManager");

  Entity *ghostEnt = _sceneMgr->createEntity(name, "ghost.mesh");
  ghostEnt->setMaterialName(material);
  ghostEnt->setCastShadows(true);
  
  SceneNode *nodeGhost = _sceneMgr->createSceneNode("nodeGhost"+name);
  nodeGhost->attachObject(ghostEnt);
  nodeGhost->scale(0.2, 0.2, 0.2);
  _sceneMgr->getRootSceneNode()->addChild(nodeGhost);
  Entity *ojosEnt = _sceneMgr->createEntity("ojo"+name, "ojosGhost.mesh");
  SceneNode *nodeOjos = _sceneMgr->createSceneNode("nodeOjos"+name);
  nodeOjos->attachObject(ojosEnt);
  nodeGhost->addChild(nodeOjos);
  nodeOjos->setInheritOrientation(false);

  body = new  RigidBody(_name, _world);
  shape = new SphereCollisionShape(0.2);
  body->setShape(nodeGhost,
                        shape,
                        0.0,
                        0.0,
                        10,
                        Vector3::ZERO,
                        Quaternion::IDENTITY);
  body->enableActiveState();
  
  btTransform transform; //Declaration of the btTransform
  transform.setIdentity(); //This function put the variable of the object to default. The ctor of btTransform doesnt do it.
  transform.setOrigin(OgreBulletCollisions::OgreBtConverter::to(position)); //Set the new position/origin
  body->getBulletRigidBody()->setWorldTransform(transform); //Apply the btTransform to the body*/


  
}


/*
 * Destructor
 */
Phantom::~Phantom() {
  delete body;
  delete shape;
}

/*
 * Get  position
 */
const Vector3& Phantom::getPosition() {
  return this->body->getSceneNode()->getPosition();
}

/*
 * Move phantom in one direction
 */
void Phantom::move(int direction) {
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


