#include "pacman.h"
#include "Shapes/OgreBulletCollisionsSphereShape.h"
#include "PlayState.h"
#include "OgreUtil.h"
#define PACMAN_EAT_ANIM "pakupaku"
#define ROTATE_LEFT 0
#define ROTATE_RIGHT 180
#define ROTATE_UP -90
#define ROTATE_DOWN 90


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


  power = false;
  startNode = idNodeStart;
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


  body = new  RigidBody("pacman", _world, COL_PACMAN,COL_WALL | COL_PILL | COL_PHANTOM | COL_FLOOR | COL_FRUIT);
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
  body->getBulletRigidBody()->forceActivationState(DISABLE_DEACTIVATION );
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
  //delete body; // MEMORY LAKE CONOCIDO, PENDIENTE DE REVISAR
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
void Pacman::stop()
{
  if (body!=NULL) body->setLinearVelocity( Ogre::Vector3(0,0,0 ));
}

void Pacman::move(int direction  , double deltaTime) {
  float x = 0,  z = 0;
  float spaceTranslated = this->speed;
  int rotate;
  switch (direction) {
    case UP_DIR: z+= spaceTranslated;
      rotate = ROTATE_UP;
      break;
    case DOWN_DIR: z-= spaceTranslated;
      rotate = ROTATE_DOWN;
      break;
    case LEFT_DIR: x+= spaceTranslated;
      rotate = ROTATE_LEFT;
      break;
    case RIGHT_DIR: x-= spaceTranslated;
      rotate = ROTATE_RIGHT;
      break;
  }
  int unrotate;
  //Change Direction, rotate Pacman
  if (direction != oldDir) {
      switch (oldDir) {
    case UP_DIR: unrotate = -ROTATE_UP;
      break;
    case DOWN_DIR: unrotate = -ROTATE_DOWN;
      break;
        case  LEFT_DIR :unrotate = -ROTATE_LEFT;
      break;
        case RIGHT_DIR :unrotate = -ROTATE_RIGHT;
      break;
      }


    

      std::cout << " degrees " << rotate << "-" << unrotate << "->"<<direction << std::endl;

    btTransform trans = body->getCenterOfMassTransform();
    btQuaternion quat;
    quat.setEuler(0,Radian(Ogre::Degree(unrotate)).valueRadians(),0);
    trans.setRotation(quat);

    body->getBulletRigidBody()->setWorldTransform(trans); //Apply the btTransform to the body*/
    trans = body->getCenterOfMassTransform();
    
    quat.setEuler(Radian(Ogre::Degree(rotate)).valueRadians(),0,0);
    trans.setRotation(quat);

    body->getBulletRigidBody()->setWorldTransform(trans); //Apply the btTransform to the body*/

    //body->setOrientation(transRot);

    oldDir = direction;
  }

//    Ogre::Quaternion quat = node->getOrientation();       //Saco un cuaternio con la orientación del nodo para pasárselo a Bullet
//    btTransform trans = body->getCenterOfMassTransform();
//    btQuaternion transRot = trans.getRotation();
//    btQuaternion rotQuat(quat.x,quat.y,quat.z,quat.w);
//    transRot = rotQuat * transRot;
//    trans.setRotation(transRot);
//    body->getBulletRigidBody()->setWorldTransform(trans); //Apply the btTransform to the body*/
    //body->setOrientation(transRot);


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


void Pacman::reset(){


  graphml_boost::nodo_props startNode_prop = PlayState::getSingleton().getGraphNode(startNode);

  Vector3 position = Vector3(atof(startNode_prop.x.c_str()),
                                   atof(startNode_prop.y.c_str()),
                                   atof(startNode_prop.z.c_str()));
  btTransform transform; //Declaration of the btTransform
  transform.setIdentity(); //This function put the variable of the object to default. The ctor of btTransform doesnt do it.


  transform.setOrigin(OgreBulletCollisions::OgreBtConverter::to(position)); //Set the new position/origin
  body->getBulletRigidBody()->setWorldTransform(transform); //Apply the btTransform to the body*/



}
