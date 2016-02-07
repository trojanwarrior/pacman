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

  body = new  RigidBody(_name, _world,COL_PHANTOM,COL_PACMAN);
  shape = new SphereCollisionShape(0.2);
  body->setShape(nodeGhost,
                        shape,
                        0.0,
                        0.0,
                        0.01,
                        Vector3::ZERO,
                        Quaternion::IDENTITY);
  body->enableActiveState();

  btTransform transform; //Declaration of the btTransform
  transform.setIdentity(); //This function put the variable of the object to default. The ctor of btTransform doesnt do it.

  std::cout << "creando fantasma en " <<position;
  transform.setOrigin(OgreBulletCollisions::OgreBtConverter::to(position)); //Set the new position/origin
  body->getBulletRigidBody()->setWorldTransform(transform); //Apply the btTransform to the body*/
  calculateNewDestiny();

 
  
}
Vector3  Phantom::getBulletPosition(){
  btVector3 btPos = body->getBulletRigidBody()->getCenterOfMassPosition();
  return Vector3(btPos.getX(),btPos.getY(),btPos.getZ());
  


}

void Phantom::calculateNewDestiny(){


  int idPacmanNode = PlayState::getSingleton().getPacman()->getCurrentNode();
  graphml_boost::ruta_t route = PlayState::getSingleton().calculateRoute(idOrigin, idPacmanNode);
  if(idOrigin == startNode){
    for(int i = route.size()-1; i>=0;i-- ){
      //std::cout << route[i].idBoost << ","<< std::endl;
      
    }
  }
  graphml_boost::nodo_props nodeDestiny = route[(route.size()-2)];
  //std::cout << idOrigin << "-" << getBulletPosition() << std::endl;

  idDestiny = nodeDestiny.idBoost;
  //  std::cout << "ruta "<< idOrigin << "->"<<idDestiny << "("<< nodeDestiny.id<< ")"<< std::endl;
  
  
  Vector3 positionDestiny = Vector3(atof(nodeDestiny.x.c_str()),
                                   atof(nodeDestiny.y.c_str()),
                                   atof(nodeDestiny.z.c_str()));
  Vector3 positionOrigin = getBulletPosition();

  //std::cout << positionOrigin << "->" << positionDestiny << std::endl;


   positionOrigin.y = 0;

   Ogre::Vector3 direction = positionDestiny - positionOrigin;
  distanceToDestiny = direction.length();
  direction.normalise();
  direction = direction*speed;
  //  std::cout << "velocidad" << direction << std::endl;
  body->setLinearVelocity(direction);
  //  std::cout << " distancia "<< idDestiny << "-" << distanceToDestiny << std::endl;

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
                                   0,
                                   atof(nodeDestiny.z.c_str()));

  Vector3 origin = getBulletPosition();
  origin.y = 0;
  //  std::cout << getBulletPosition() << "->" << positionDestiny << std::endl;
  Ogre::Vector3 direction = positionDestiny - origin;
  float newDistance = direction.length();

  // std::cout << newDistance << "-" << getBulletPosition() << "-" << positionDestiny << std::endl;

  
  if(newDistance < 0.05 ){
    idOrigin = idDestiny;
    //    std::cout << "cambiando de destino" << std::endl;
    calculateNewDestiny();
  }
  else{

    distanceToDestiny = newDistance;
  }



}


