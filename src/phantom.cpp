#include "phantom.h"
#include "Shapes/OgreBulletCollisionsSphereShape.h"
#include <string>
#include "pacman.h"
#include "PlayState.h"
#include "OgreUtil.h"

#define GHOST_MOVE_ANIM "mareo2"
#define GHOST_EYE_AFRAID "cagaos"
#define GHOST_EYE_NORMAL "piupiu"
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

  afraid = false;
  smart = _smart;
  name = _name;
  speed = _speed;
  idOrigin = origin;
  startNode = origin;
  orgMaterial = material;
  estado = estadoPhantom::NORMAL;


  SceneManager* _sceneMgr = Root::getSingleton().
                            getSceneManager("SceneManager");

  Entity *ghostEnt = _sceneMgr->createEntity(name, "ghost.mesh");
  ghostEnt->setMaterialName(material);
  ghostEnt->setCastShadows(true);

   AnimationState*  anim = ghostEnt->getAnimationState(GHOST_MOVE_ANIM);
  anim->setEnabled(true);
  anim->setLoop(true);
  
  
  SceneNode *nodeGhost = _sceneMgr->createSceneNode("nodeGhost"+name);
  nodeGhost->attachObject(ghostEnt);
  nodeGhost->scale(0.2, 0.2, 0.2);
  _sceneMgr->getRootSceneNode()->addChild(nodeGhost);
  
  Entity *ojosEnt = _sceneMgr->createEntity("ojo"+name, "ojosGhost.mesh");
  SceneNode *nodeOjos = _sceneMgr->createSceneNode("nodeOjos"+name);
  nodeOjos->attachObject(ojosEnt);
  nodeGhost->addChild(nodeOjos);
  nodeOjos->setInheritOrientation(false);
  
  Entity *entBoca = _sceneMgr->createEntity("boca"+name,"bocaGhost.mesh");
  SceneNode *nodeBoca = _sceneMgr->createSceneNode("nodeBoca"+name);
  nodeBoca->attachObject(entBoca);
  nodeBoca->setVisible(false);
  nodeGhost->addChild(nodeBoca);
  nodeBoca->setInheritOrientation(false);
  


  body = new  RigidBody(_name, _world,COL_PHANTOM,COL_PACMAN | COL_FLOOR);
  shape = new SphereCollisionShape(0.2);
  body->setShape(nodeGhost,
                        shape,
                        0.0,
                        0.0,
                        0.1,
                        Vector3::ZERO,
                        Quaternion::IDENTITY);
  body->enableActiveState();

  btTransform transform; //Declaration of the btTransform
  transform.setIdentity(); //This function put the variable of the object to default. The ctor of btTransform doesnt do it.


  transform.setOrigin(OgreBulletCollisions::OgreBtConverter::to(position)); //Set the new position/origin
  body->getBulletRigidBody()->setWorldTransform(transform); //Apply the btTransform to the body*/
  changeStatePhantom(estadoPhantom::NORMAL);
  calculateNewDestiny();


 
  
}
Vector3  Phantom::getBulletPosition(){
  btVector3 btPos = body->getBulletRigidBody()->getCenterOfMassPosition();
  return Vector3(btPos.getX(),btPos.getY(),btPos.getZ());
  


}

void Phantom::calculateNewDestiny(){

  //std::cout << "Afraid " << afraid  << std::endl;
  int destiny;

//  if (afraid == true) {
//    destiny = PlayState::getSingleton().getFarNode();
//    std::cout << "Afraid " << destiny << std::endl;
//  }
//  else{
//      destiny = PlayState::getSingleton().getPacman()->getCurrentNode();
//      std::cout << "NO Afraid " << destiny << std::endl;
//  }

  switch (estado)
  {
      case estadoPhantom::NORMAL:      destiny = PlayState::getSingleton().getPacman()->getCurrentNode(); std::cout << "normal"<< std::endl;break;
      case estadoPhantom::ACOJONADO:   destiny = PlayState::getSingleton().getFarNode(); std::cout << "acojonadoooo"<< std::endl;break;
    case estadoPhantom::MUERTO:      destiny = startNode; std::cout << "muertooo"<< std::endl;
  }

  std::cout << " nodo " << idOrigin<<"->"<< destiny << std::endl; 
  graphml_boost::ruta_t route = PlayState::getSingleton().calculateRoute(idOrigin, destiny);

  graphml_boost::nodo_props nodeDestiny = (route.size()>1)? route[(route.size()-2)] :route[0];
  std::cout << idOrigin << "-" << getBulletPosition() << std::endl;

  idDestiny = nodeDestiny.idBoost;
  std::cout << "ruta "<< idOrigin << "->"<<idDestiny << "("<< nodeDestiny.id<< ")"<< std::endl;
  
  
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
   std::cout << "velocidad" << direction << std::endl;
  body->setLinearVelocity(direction);
  //std::cout << " distancia "<< idDestiny << "-" << distanceToDestiny << std::endl;

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
void Phantom::checkMove(Ogre::Real deltaT) {


  
  SceneManager* _sceneMgr = Root::getSingleton().getSceneManager("SceneManager");
  
  //if (estado == estadoPhantom::MUERTO) idDestiny = startNode;

  graphml_boost::nodo_props nodeDestiny = PlayState::getSingleton().getGraphNode(idDestiny);
  Vector3 positionDestiny = Vector3(atof(nodeDestiny.x.c_str()),
                                   0,
                                   atof(nodeDestiny.z.c_str()));

  Vector3 origin = getBulletPosition();
  origin.y = 0;
  //std::cout << getBulletPosition() << "->" << positionDestiny << std::endl;
  Ogre::Vector3 direction = positionDestiny - origin;
  float newDistance = direction.length();

  std::cout << "mojonooo "<< newDistance << "-" << origin << "-" << positionDestiny << std::endl;

  
  if(newDistance < 0.1 )
  {
        
    idOrigin = idDestiny;
    //std::cout << "cambiando de destino" << std::endl;

    //Si llego a mi destino y estaba muerto, establecemos estado a NORMAL  
    //de modo que al hacer calculateNewDestiny() volvamos a buscar a Pacman.
    if (estado == estadoPhantom::MUERTO && idOrigin == startNode)
       setNormal(); 

    calculateNewDestiny();
  }
  else{

    distanceToDestiny = newDistance;
    direction.normalise();
    direction = direction*speed;
  
    body->setLinearVelocity(direction);
  }
  
  _sceneMgr->getEntity("ojo"+name)->getAnimationState(GHOST_EYE_AFRAID)->addTime(deltaT);
  _sceneMgr->getEntity("ojo"+name)->getAnimationState(GHOST_EYE_NORMAL)->addTime(deltaT);
  _sceneMgr->getEntity(name)->getAnimationState(GHOST_MOVE_ANIM)->addTime(deltaT);
  

}

void Phantom::reset(){

  idOrigin = startNode;
  
  graphml_boost::nodo_props startNode_prop = PlayState::getSingleton().getGraphNode(startNode);

  Vector3 position = Vector3(atof(startNode_prop.x.c_str()),
                                   atof(startNode_prop.y.c_str()),
                                   atof(startNode_prop.z.c_str()));

  std::cout << "Posicion muerte" << position << std::endl;
  btTransform transform; //Declaration of the btTransform
  transform.setIdentity(); //This function put the variable of the object to default. The ctor of btTransform doesnt do it.

 
  transform.setOrigin(OgreBulletCollisions::OgreBtConverter::to(position)); //Set the new position/origin
  body->getBulletRigidBody()->setWorldTransform(transform); //Apply the btTransform to the body*/
  calculateNewDestiny();




}

//void Phantom::setAfraid(bool _afraid){
//    SceneManager* _sceneMgr = Root::getSingleton().
//                            getSceneManager("SceneManager");
//
//  afraid = _afraid;
//  std::cout << "AfradiSet " << afraid  << std::endl;
//  if(afraid == true){
//     _sceneMgr->getEntity(name)->setMaterialName("materialCagaosAzul");
//     _sceneMgr->getSceneNode("nodeBoca"+name)->setVisible(true);    
//
//     AnimationState*  anim = _sceneMgr->getEntity("ojo"+name)->getAnimationState(GHOST_EYE_AFRAID);
//     anim->setEnabled(true);
//     anim->setLoop(true);
//      anim = _sceneMgr->getEntity("ojo"+name)->getAnimationState(GHOST_EYE_NORMAL);
//      anim->setEnabled(false);
//
//                    
//  }
//  else{
//    _sceneMgr->getEntity(name)->setMaterialName(orgMaterial);
//     _sceneMgr->getSceneNode("nodeBoca"+name)->setVisible(false);    
//     AnimationState*  anim = _sceneMgr->getEntity("ojo"+name)->getAnimationState(GHOST_EYE_NORMAL);
//     anim->setEnabled(true);
//     anim->setLoop(true);
//     anim = _sceneMgr->getEntity("ojo"+name)->getAnimationState(GHOST_EYE_AFRAID);
//     anim->setEnabled(false); 
//  }
//
//}

void Phantom::setAfraid()
{
    body->setQueryFlags(COL_PACMAN | COL_FLOOR);
    SceneManager* _sceneMgr = Root::getSingleton().getSceneManager("SceneManager");

    afraid = true;
    std::cout << "AfraidSet " << afraid  << std::endl;
    //Asignamos nuevo material, o sea, los vestimos de acojonaos
    _sceneMgr->getEntity(name)->setMaterialName("materialCagaosAzul");
    //Mostramos la bocacha de acojone
    _sceneMgr->getSceneNode("nodeBoca"+name)->setVisible(true);    

    //Activamos animación de los ojos de acojone
    AnimationState*  anim = _sceneMgr->getEntity("ojo"+name)->getAnimationState(GHOST_EYE_AFRAID);
    anim->setEnabled(true);
    anim->setLoop(true);
    //Desactivamos la animación de cuando está en modo normal.
    anim = _sceneMgr->getEntity("ojo"+name)->getAnimationState(GHOST_EYE_NORMAL);
    anim->setEnabled(false);
    
    estado = estadoPhantom::ACOJONADO;
}

void Phantom::setNormal()
{
    body->setQueryFlags(COL_PACMAN | COL_FLOOR);
    SceneManager* _sceneMgr = Root::getSingleton().getSceneManager("SceneManager");
    
    //Asignamos el material de modo normal, el color propio del fantasma
    _sceneMgr->getEntity(name)->setMaterialName(orgMaterial);
    //Escondemos la boca de acojone
    _sceneMgr->getSceneNode("nodeBoca"+name)->setVisible(false);    
    //Activamos la animación de los ojos en modo normal
    AnimationState*  anim = _sceneMgr->getEntity("ojo"+name)->getAnimationState(GHOST_EYE_NORMAL);
    anim->setEnabled(true);
    anim->setLoop(true);
    //Desactivamos animación de ojos de acojone
    anim = _sceneMgr->getEntity("ojo"+name)->getAnimationState(GHOST_EYE_AFRAID);
    anim->setEnabled(false); 
    _sceneMgr->getSceneNode("nodeGhost" + name)->setVisible(true) ;
    estado = estadoPhantom::NORMAL;

    
}

void Phantom::setMuerto()
{
   
   //body->setDefaultQueryFlags()
    body->setQueryFlags(COL_FLOOR);
   
   SceneManager* _sceneMgr = Root::getSingleton().getSceneManager("SceneManager");
   
   // Ocultamos el cuerpo del fantasma. False para ocultarlo,
   //                                   False para que no lo hereden sus nodos hijo (los ojos en este caso)
   _sceneMgr->getSceneNode("nodeGhost" + name)->setVisible(false,false); 
   
   estado = estadoPhantom::MUERTO;
    calculateNewDestiny();   
   // Y en principio nada más. Cuando llegué a su destino habrá que llamar
   // otra vez a setNormal() y vuelta al ataque.

}

void Phantom::changeStatePhantom(estadoPhantom _estado)    
{

  switch(_estado)
  {
     case estadoPhantom::NORMAL:       setNormal(); break;
     case estadoPhantom::ACOJONADO:    setAfraid(); break;
     case estadoPhantom::MUERTO:       setMuerto(); break;
  }
}

estadoPhantom Phantom::getEstado()
{
    return static_cast<estadoPhantom>(estado);
}

  





