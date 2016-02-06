//
// Created by twsh on 5/02/16.
//

#include "FruitFactory.h"
#include "Fruit.h"



Fruit * FruitFactory::createFruit(DynamicsWorld* _world, Vector3 position,tipoFruta tipo){
    return new Fruit(_world, position,names[tipo]);
}


std::vector<Fruit>* FruitFactory::createAllFruits(DynamicsWorld* _world)
{
    std::vector<Fruit>* fruits = new std::vector<Fruit>();
    Ogre::Vector3 v(0,0,0);
    for(size_t type=0; type < names->size();type++)
    {
        Fruit fruta(_world, v,names[type]);
        fruits->push_back(fruta);
    }
    return fruits;
}

