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


Fruit::Fruit(DynamicsWorld* world,Ogre::Vector3 position, const std::string &name)
{
    cout << "creando fruta\n";
    _name = name;
    _world = world;
    _position = position;
    _anim = nullptr;

    SceneManager* _sceneMgr = Root::getSingleton().getSceneManager("SceneManager");

    _frutaEnt = _sceneMgr->createEntity(name+"Ent", name+".mesh");

    _nodeFruit = _sceneMgr->createSceneNode("nodeFruit"+name);
    _nodeFruit->attachObject(_frutaEnt);
    _nodeFruit->scale(0.2, 0.2, 0.2);
    //_sceneMgr->getRootSceneNode()->addChild(_nodeFruit);
    _nodeFruit->setVisible(false);

    body = new  RigidBody(_name, _world);
    shape = new BoxCollisionShape(_frutaEnt->getBoundingBox().getSize());


    body->setShape(_nodeFruit,
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
/*    if (body)
        delete body;
    if (shape)
        delete shape;
    body = nullptr;
    shape = nullptr;*/
    cout << "destruyendo fruta \n";
}

Fruit::Fruit(const Fruit &fruit)
{
    this->_anim = fruit._anim;
    this->_name = fruit._name; 
    this->_frutaEnt = fruit._frutaEnt;
    this->_nodeFruit = fruit._nodeFruit;
//    Ogre::AxisAlignedBox aab = this->_frutaEnt->getBoundingBox();
//    aab.getSize();
    this->body = new  RigidBody(fruit._name,fruit._world);
    this->shape = new BoxCollisionShape(fruit._frutaEnt->getBoundingBox().getSize());
    //this->shape = new BoxCollisionShape(Ogre::Vector3(1,1,1));
    *this->body = *fruit.body;
    *this->shape = *fruit.shape;
    cout << "construyendo copia fruta \n";

}

Fruit& Fruit::operator=(const Fruit & fruit)
{
    this->_anim = fruit._anim; //shallow copy, no hay problema.
    this->_name = fruit._name; //shallow copy, no hay problema.
    this->_frutaEnt = fruit._frutaEnt;
    this->_nodeFruit = fruit._nodeFruit;

    if (this->body)  delete body; // Si el objeto que recibe la asignación ya tenía memoria reservada deberemos resetearla.
    if (this->shape) delete shape;// Si el objeto que recibe la asignación ya tenía memoria reservada deberemos resetearla.
 
    this->body = new  RigidBody(fruit._name,fruit._world); // reservamos memoria nueva
    this->shape = new BoxCollisionShape(fruit._frutaEnt->getBoundingBox().getSize());// reservamos memoria nueva
    *this->body = *fruit.body;      // Ahora viene lo gracioso: el menda que hizo las clases de body y shape, ¿se curró los correspondientes
    *this->shape = *fruit.shape;    // constructores de asignación y copia???? Por que si no es así ya puedo hacer yo el pino con las orejas que es paná!
    cout << "constructor asignación fruta\n";
    return *this;
}

//Fruit::Fruit(Fruit&&)
//{
//    cout << "a ver que hace";
//}

void Fruit::animaFruta(Fruit::tipoAnim tipo,Ogre::Real deltaT)
{

    if (!_anim)
    {
        //SceneManager *sceneMgr = Root::getSingleton().getSceneManager("SceneManager");
        //_anim = sceneMgr->getEntity(_name)->getAnimationState(nombreAnim[tipo]);
        _anim = _frutaEnt->getAnimationState(nombreAnim[tipo]);
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

void Fruit::setPosition(Ogre::Vector3 posicion)  
{
  SceneManager *sceneMgr = Root::getSingleton().getSceneManager("SceneManager");
  sceneMgr->getSceneNode("nodeFruit" + this->_name)->setPosition(posicion);
}

void Fruit::aparece(Ogre::Vector3 donde)
{
    SceneManager* _sceneMgr = Root::getSingleton().getSceneManager("SceneManager");
    _sceneMgr->getRootSceneNode()->addChild(_nodeFruit);
    _nodeFruit->setPosition(donde);
    _nodeFruit->setVisible(true);
    //Se delega la activación de las animaciones fuera de esta clase.
}

void Fruit::desaparece()
{
    _nodeFruit->setVisible(false);  // Ocultamos la fruta
    _nodeFruit->getParentSceneNode()->removeChild(_nodeFruit); // nos independizamos :D
}

