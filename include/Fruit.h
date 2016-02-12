//
// Created by twsh on 5/02/16.
//

#ifndef PACMAN_FRUIT_H
#define PACMAN_FRUIT_H

#include "Ogre.h"
#include "OgreBulletDynamicsRigidBody.h"
#include "OgreBulletDynamicsWorld.h"
#include <string>
#include <Shapes/OgreBulletCollisionsBoxShape.h>


using namespace std;
using namespace Ogre;
using namespace OgreBulletCollisions;
using namespace OgreBulletDynamics;


class Fruit
{

private :
    string _name;
    const string nombreAnim[2] = {"reclamo","meneo"};
    

    RigidBody* body;
    BoxCollisionShape* shape;
    
    DynamicsWorld* _world;
    Entity *_frutaEnt;
    SceneNode *_nodeFruit;
    Ogre::Vector3 _position;

    Ogre::AnimationState *_anim; // Con un puntero vale. Pues la idea es que solo haya una animación en marcha.
                                 // Meneo sería la animación en standby y Reclamo, pues eso, un toque de atención: "Eo! que estoy aquí!! Ven a por mi!!
                                 // Y entonces es cuando el ansia te puede, intentas ir a por la dichosa fruta y MUERES!!! :D :D

public :
    enum tipoAnim {RECLAMO,MENEO};
    const Ogre::Vector3& getPosition();
    explicit Fruit(DynamicsWorld* _world,Vector3 position,const string &name);
//    Fruit(Fruit&&); //constructor move
//    ~Fruit();
//    Fruit& operator=(const Fruit & fruit); // constructor de asignacion
    Fruit(const Fruit &fruit); //constructor de copia
    void animaFruta(tipoAnim tipo,Ogre::Real deltaT);
    void cambiaAnimacion(tipoAnim tipo, Ogre::Real deltaT);
    void setPosition(Ogre::Vector3 posicion);
    void aparece(Ogre::Vector3 donde);
    void desaparece();
    btRigidBody* getBtRigidBody();

};


#endif //PACMAN_FRUIT_H
