#include "phantomFactory.h"
#include "phantom.h"

uint globalSeed = 1;
Phantom* PhantomFactory::createPhantom(DynamicsWorld* _world, Vector3 position,int type){
  return new Phantom(_world, position,names[type], speeds[type],smarts[type],materials[type]);
}


std::vector<Phantom>* PhantomFactory::createAllPhantoms(DynamicsWorld* _world,graphml_boost::ruta_t phantomZone){
  std::vector<Phantom>* phantoms = new std::vector<Phantom>();
  
  for(int type=0; type< 4;type++){
    
    int positionIndex=  rand_r(&globalSeed) % phantomZone.size();
    std::cout <<   positionIndex << endl;
    graphml_boost::nodo_props_t startNode = phantomZone.at(positionIndex);
    Vector3 position  =  Vector3(atof(startNode.x.c_str()),
                                   atof(startNode.y.c_str()),
                                   atof(startNode.z.c_str()));
    Phantom(_world, position,names[type], speeds[type],smarts[type],materials[type]);
  }
  return phantoms;
}


