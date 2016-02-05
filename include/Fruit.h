//
// Created by twsh on 5/02/16.
//

#ifndef PACMAN_FRUIT_H
#define PACMAN_FRUIT_H

#include "Ogre.h"
#include "OgreBulletDynamicsRigidBody.h"
#include "OgreBulletDynamicsWorld.h"
#include <string>

using namespace std;
using namespace Ogre;
using namespace OgreBulletDynamics;
using namespace OgreBulletCollisions;


class Fruit
{

private :
    string _name;
    const string nombreAnim[2] = {"reclamo","meneo"};


    RigidBody* body;
    CollisionShape* shape;
    Ogre::AnimationState *_anim; // Con un puntero vale. Pues la idea es que solo haya una animación en marcha.
                                 // Meneo sería la animación en standby y Reclamo, pues eso, un toque de atención: "Eo! que estoy aquí!! Ven a por mi!!
                                 // Y entonces es cuando el ansia te puede, intentas ir a por la dichosa fruta y MUERES!!! :D :D

public :
    enum tipoAnim {RECLAMO,MENEO};

    const Ogre::Vector3& getPosition();
    explicit Fruit(DynamicsWorld* _world,Vector3 position,string name);
    ~Fruit();
    Fruit(const Fruit &fruit);
    void animaFruta(tipoAnim tipo,Ogre::Real deltaT);

};


#endif //PACMAN_FRUIT_H
