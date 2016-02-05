#ifndef PHANTOM_FACTORY_H
#define PHANTOM_FACTORY_H
#include "phantom.h"
#include "graphml_boost.h"

#define CLYDE 1
#define BLINKY 2
#define INKY 3
#define PINKY 4


using namespace std;
using namespace Ogre;
using namespace OgreBulletDynamics;
using namespace OgreBulletCollisions;

class PhantomFactory {
 private:

  const string names[4] = {"clyde","blinky","inky","pinky"};
  const float smarts[4] = {70,80,90,100};
  const string materials[4] = {"materialClydeNaranja","materialBlinkyRojo","materialInkyCian","materialPinkyRosa"};
  const float speeds[4] = {1,2,1.5,1.8} ;

    PhantomFactory() {

    };
    PhantomFactory(PhantomFactory const&) = delete;
    void operator = (PhantomFactory const &) = delete;
    ~PhantomFactory(){};


 public:
  static  PhantomFactory &getInstance(){
    static PhantomFactory instance;
    return instance;
  };
  std::vector<Phantom>* createAllPhantoms(DynamicsWorld* _world,graphml_boost::ruta_t phantomZone);
  Phantom *createPhantom(DynamicsWorld* _world, Vector3 position,int type);

  


};
#endif
