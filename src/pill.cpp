#include "pill.h"
#include "Shapes/OgreBulletCollisionsSphereShape.h"



using namespace Ogre;
using namespace OgreBulletDynamics;
using namespace OgreBulletCollisions;


/*
 *Constructor
 */
Pill::Pill(DynamicsWorld *_world, Vector3 position,bool big,int id,int idNode) {
  _big = big;
  _id = id;
  _idNode = idNode;
  SceneManager* _sceneMgr = Root::getSingleton().
                            getSceneManager("SceneManager");
  std::stringstream str;
   str <<(_big ? "bigpill":"pill")<< (id);
  _node = _sceneMgr->createSceneNode(str.str());
  _sceneMgr->getRootSceneNode()->addChild(_node);
  Entity* pillEnt = _sceneMgr->createEntity("pill.mesh");
  pillEnt->setCastShadows(true);
  _node->attachObject(pillEnt);
  if (_big == true) {
    _node->scale(1.5, 1.5, 1.5);
  }
  _body = new  RigidBody(str.str(), _world, COL_PILL,COL_PACMAN);
  _shape = new SphereCollisionShape(big ? 0.3 :0.1);
  _body->setShape(_node,
                 _shape,
                 0.0,
                 0.0,
                 0.01,
                 Vector3::ZERO,
                 Quaternion::IDENTITY);

  btTransform transform; //Declaration of the btTransform
  transform.setIdentity(); //This function put the variable of the object to default. The ctor of btTransform doesnt do it.
  transform.setOrigin(OgreBulletCollisions::OgreBtConverter::to(position)); //Set the new position/origin
  _body->getBulletRigidBody()->setWorldTransform(transform); //Apply the btTransform to the body
}

/*Pill::Pill(Pill &pill){
  _node = pill._node;
  _body = pill._body;
  _shape = pill._shape;
  _big = pill._big;
  _id = pill._id;
   return this;

   }*/

/*Pill& operator =(const Pill& pill){
  _node = pill._node;
  _body = pill._body;
  _shape = pill._shape;
  _big = pill._big;
  _id = pill._id;
return *this;


}*/

/*
 * Destructor
 */
/*Pill::~Pill() {
delete _body;
delete _shape;

}*/


  
btRigidBody* Pill::getBtRigidBody(){
  return _body->getBulletRigidBody();

}

