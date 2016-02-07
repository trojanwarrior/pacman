#include "phantomFactory.h"
#include "phantom.h"





std::vector<Phantom>* PhantomFactory::createAllPhantoms(DynamicsWorld* _world,graphml_boost::ruta_t phantomZone){
  std::vector<Phantom>* phantoms = new std::vector<Phantom>();
  
  for(int type = 0; type< 4;type++){
    
    int positionIndex=  rand() % phantomZone.size();
    std::cout <<   positionIndex << endl;
    graphml_boost::nodo_props_t startNode = phantomZone.at(positionIndex);
    Vector3 position  =  Vector3(atof(startNode.x.c_str()),
                                   atof(startNode.y.c_str()),
                                   atof(startNode.z.c_str()));
    phantoms->push_back(Phantom(_world, position,names[type], speeds[type],smarts[type],materials[type],startNode.idBoost));
    std::cout << phantoms->at(0).getBody()->getSceneNode() << std::endl;
  }
  return phantoms;
}


