//
// Created by twsh on 5/02/16.
//

#include "Fruit.h"
#include "Shapes/OgreBulletCollisionsSphereShape.h"
#include "Shapes/OgreBulletCollisionsBoxShape.h"

using namespace Ogre;
using namespace std;
using namespace OgreBulletDynamics;
using namespace OgreBulletCollisions;


Fruit::Fruit(DynamicsWorld* _world,Ogre::Vector3 position, const std::string &name)
{
    _name = name;

    SceneManager* _sceneMgr = Root::getSingleton().getSceneManager("SceneManager");

    Entity *frutaEnt = _sceneMgr->createEntity(name+"Ent", name+".mesh");

    SceneNode *nodeFruit = _sceneMgr->createSceneNode("nodeFruit"+name);
    nodeFruit->attachObject(frutaEnt);
    nodeFruit->scale(0.2, 0.2, 0.2);
    _sceneMgr->getRootSceneNode()->addChild(nodeFruit);

    body = new  RigidBody(_name, _world);
    shape = new BoxCollisionShape(*(frutaEnt->getBoundingBox().getAllCorners()));


    body->setShape(nodeFruit,
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

Fruit::~Fruit()
{
    delete body;
    delete shape;
}

Fruit::Fruit(const Fruit &fruit)
{
    this->_anim = fruit._anim;
    this->_name = fruit._name;
    this->body = fruit.body;
    this->shape = fruit.shape;
}

Fruit::Fruit(Fruit&&)
{
    cout << "a ver que hace";
}

void Fruit::animaFruta(Fruit::tipoAnim tipo,Ogre::Real deltaT)
{

    if (!_anim)
    {
        SceneManager *sceneMgr = Root::getSingleton().getSceneManager("SceneManager");
        _anim = sceneMgr->getEntity(_name)->getAnimationState(nombreAnim[tipo]);
        _anim->setEnabled(true);
        _anim->setTimePosition(0.0);
        _anim->setLoop(true);
    }
    else
        _anim->addTime(deltaT);
}

void Fruit::cambiaAnimacion(Fruit::tipoAnim tipo, Ogre::Real deltaT)
{
    _anim = NULL;
    animaFruta(tipo,deltaT);
}

const Ogre::Vector3& Fruit::getPosition()
{
    return this->body->getSceneNode()->getPosition();
}
