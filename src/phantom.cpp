#include "phantom.h"
#include "Shapes/OgreBulletCollisionsSphereShape.h"
#include <string>
#include "pacman.h"
#include "PlayState.h"


using namespace Ogre;
using namespace std;
using namespace OgreBulletDynamics;
using namespace OgreBulletCollisions;


/*Phantom::Phantom(const Phantom& phantom){
  name = phantom.name;
  speed = phantom.speed;
  smart = phantom.smart;
  body = phantom.body;
  shape = phantom.shape;


  }*/
/*
 *Constructor
 */
Phantom::Phantom(DynamicsWorld *_world, Vector3 position,string _name, float _speed,float _smart,string material,int origin) {
  
  smart = _smart;
  name = _name;
  speed = _speed;
  idOrigin = origin;
  startNode = origin;


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
  calculateNewDestiny();

 
  
}

void Phantom::calculateNewDestiny(){
  int idPacmanNode = PlayState::getSingleton().getPacman()->getCurrentNode();
  graphml_boost::ruta_t route = PlayState::getSingleton().calculateRoute(idOrigin, idPacmanNode);
  graphml_boost::nodo_props nodeDestiny = route.back();
  idDestiny = route.back().idBoost;
  
  Vector3 positionDestiny = Vector3(atof(nodeDestiny.x.c_str()),
                                   atof(nodeDestiny.y.c_str()),
                                   atof(nodeDestiny.z.c_str()));

  Ogre::Vector3 direction = positionDestiny - body->getSceneNode()->getPosition();
  distanceToDestiny = direction.squaredLength();  
  body->setLinearVelocity(direction);
  std::cout << " distancia "<< idDestiny << "-" << distanceToDestiny << std::endl;
}


/*
 * Destructor
 */
/*Phantom::~Phantom() {
  delete body;
  delete shape;
  }*/

/*
 * Get  position
 */
const Vector3& Phantom::getPosition() {
  return this->body->getSceneNode()->getPosition();
}

/*
 * Move phantom in one direction
 */
void Phantom::checkMove() {

  graphml_boost::nodo_props nodeDestiny = PlayState::getSingleton().getGraphNode(idDestiny);
  Vector3 positionDestiny = Vector3(atof(nodeDestiny.x.c_str()),
                                   atof(nodeDestiny.y.c_str()),
                                   atof(nodeDestiny.z.c_str()));

  Ogre::Vector3 direction = positionDestiny - body->getSceneNode()->getPosition();
  float newDistance = direction.squaredLength();
  //std::cout << " distancia" << distanceToDestiny << std::endl;  
  if(newDistance < 0.1){
    idOrigin = idDestiny;
    std::cout << "cambiando de destino" << std::endl;
    calculateNewDestiny();
  }
  else{
    //std::cout << "sigue destino"<< std::endl;
    distanceToDestiny = newDistance;
  }



}


