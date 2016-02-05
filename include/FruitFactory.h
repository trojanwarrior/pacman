//
// Created by twsh on 5/02/16.
//

#ifndef PACMAN_FRUITFACTORY_H
#define PACMAN_FRUITFACTORY_H

#include "Fruit.h"

using namespace std;
using namespace Ogre;
using namespace OgreBulletDynamics;
using namespace OgreBulletCollisions;

class FruitFactory
{
private:
    const std::string names[8] = {"Campana","Cereza","Fresa","Galaxian","Llave","Manzana","Naranja","Uvas"};
    FruitFactory() { };
    FruitFactory(FruitFactory const&) = delete;
    void operator = (FruitFactory const &) = delete;
    ~FruitFactory(){};

public:

    enum tipoFruta{CAMPANA,CEREZA,FRESA,GALAXIAN,LLAVE,MANZANA,NARANJA,UVAS};

    static  FruitFactory &getInstance(){
        static FruitFactory instance;
        return instance;
    };

    std::vector<Fruit>* createAllFruits(DynamicsWorld* _world);
    Fruit *createFruit(DynamicsWorld* _world, Vector3 position,tipoFruta tipo);

};


#endif //PACMAN_FRUITFACTORY_H
