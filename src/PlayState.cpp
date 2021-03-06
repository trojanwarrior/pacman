#include "ControlsState.h"
#include "RecordsState.h"
#include "PauseState.h"
#include "IntroState.h"
#include "PlayState.h"
#include "MenuState.h"
#include "OgreBulletCollisionsShape.h"
#include "Shapes/OgreBulletCollisionsTrimeshShape.h"
#include "Shapes/OgreBulletCollisionsStaticPlaneShape.h"
#include "Shapes/OgreBulletCollisionsSphereShape.h"
#include "OgreBulletDynamicsWorld.h"
#include "graphml_boost.h"
#include <string>
#include <vector>
#include "records.h"
#include "OgreUtil.h"
#include <ctime>

#define TIME_AFRAID  10
//http://www.cplusplus.com/doc/tutorial/templates/          <--------Visita esta página para entender la linea justo debajo de esta
template<> PlayState* Ogre::Singleton<PlayState>::msSingleton = 0;

using namespace std;
using namespace Ogre;
using namespace OgreBulletDynamics;
using namespace OgreBulletCollisions;

void PlayState::enter ()
{
  stopWorld =false;

  _root = Ogre::Root::getSingletonPtr();

  // Se recupera el gestor de escena y la cámara.
  _sceneMgr = _root->getSceneManager("SceneManager");
  _camera = _sceneMgr->getCamera("IntroCamera");
  _viewport = _root->getAutoCreatedWindow()->getViewport(0);
  // Nuevo background colour.
  _viewport->setBackgroundColour(Ogre::ColourValue(0.0, 0.0, 0.0));

  AxisAlignedBox boundBox =  AxisAlignedBox (Ogre::Vector3 (-10000, -10000, -10000),Ogre::Vector3 (10000,  10000,  10000));
  _world = new DynamicsWorld(_sceneMgr,boundBox,Vector3(0,-9.81,0),true,true,15000);
  createScene();


  _debugDrawer = new OgreBulletCollisions::DebugDrawer();
  _debugDrawer->setDrawWireframe(true);
  SceneNode *node = _sceneMgr->getRootSceneNode()->
                    createChildSceneNode("debugNode", Vector3::ZERO);
  node->attachObject(static_cast<SimpleRenderable*>(_debugDrawer));
  _world->setDebugDrawer (_debugDrawer);
  // _world->setShowDebugShapes (true);

  _exitGame = false;
  sounds::getInstance()->play_effect("intermission");
  paused=false;
  timeAfraid =-1;
  _resucitando = false;
  _deltaT = 0;
}

void PlayState::exit ()
{
  
//  message_txt->setVisible(false);
//  message_wall->setVisible(false);
//  message_txt->setCaption("");
//  paused=false;
//  cout << "SI SALE EL CARTEL ME CORTO LOS...." << endl;
  
  _sceneMgr->clearScene();
  _root->getAutoCreatedWindow()->removeAllViewports();
  delete _pacman;
  MyGUI::LayoutManager::getInstance().unloadLayout(layout);
}

void PlayState::pause()
{
    //SI ENTRAMOS AQUÍ ES POR QUE HAN APILADO UN ESTADO ENCIMA DE ESTE, LUEGO EL GAMEMANAGER
    //SERÁ EL QUE LLAME A ESTE MÉTODO. POR LO QUE NO ES NECESARIO SIQUIERA PREGUNTAR SI ESTAMOS
    //EN PAUSA. 

//  if (paused)
//  {
//    message_txt->setVisible(false);
//    message_wall->setVisible(false);
//    message_txt->setCaption("");
//    paused=false;
//  }
//  else
//  {
    if (!_resucitando)
    {
        message("PAUSE");
        paused = true;
//  }
        sounds::getInstance()->play_effect("eat_fruit");
    }
}

void PlayState::resume()
{
  // Se restaura el background colour.
  _viewport->setBackgroundColour(Ogre::ColourValue(0.0, 0.0, 1.0));
  
    //SI ENTRAMOS AQUÍ ES QUE ESTE ESTADO ESTABA EN LA PILA Y AHORA
    //ES EL QUE ESTÁ EN EL TOP DE LA PILA, LUEGO RETOMA EL CONTROL.
    message (""); 
    paused=false;

    if (_resucitando)
    {
        _resucitando = false;
        stopWorld = false;
    }

}

bool PlayState::frameStarted(const Ogre::FrameEvent& evt)
{
  _deltaT = evt.timeSinceLastFrame;
  if(!stopWorld)
  {
      _world->stepSimulation(evt.timeSinceLastFrame);
      if (_pacmanDir != 0) {
        _pacman->move(_pacmanDir, evt.timeSinceLastFrame);
      }

      if(_pacman)
      {
        if (!_pacman->getEstoyMuriendo()) // Si no me estoy muriendo pues como siempre, a zampar
        {
            _pacman->setCurrentNode(getPacmanNode());
            _pacman->updateAnim(evt.timeSinceLastFrame);
            Vector3 pacPos = _pacman->getPosition();
            _camera->setPosition(Vector3(pacPos.x, 5, pacPos.z - 4));
            _camera->lookAt(pacPos);
        }
        else // Pero si me estoy muriendo....
        {
            cout << "ME ESTOY MURIENDO!!!!!!!\n";
            if (!_pacman->heMuertoDelTodo())  // Si no me he muerto del todo, me sigo muriendo, cansino.
            {
                _pacman->animaMuerte(evt.timeSinceLastFrame);
            }
            else // Y si finalmente me muero del todo pues resucito cual YisusCrais :D
            {
                reseteaPersonajes();
//                _resucitando = true;
//                stopWorld = true;
//                pushState(PauseState::getSingletonPtr());
            }
        }
      }
      //set_score((int)(_root->getRenderSystem()->getRenderTargetIterator().getNext()->getAverageFPS()));


      for (std::vector<Phantom>::iterator it = _phantoms->begin();
          it != _phantoms->end(); ++it) {
         (*it).checkMove(evt.timeSinceLastFrame);
      }

      long int now = time(NULL);
      if ( timeAfraid>0 && (now - timeAfraid) >= TIME_AFRAID){
        setPhantomsAfraid(false);
      }

      if (_frutas)
      {
          //Animamos la fruta
          _frutas->at(_fruta_aleatoria).animaFruta(Fruit::RECLAMO,evt.timeSinceLastFrame);
          //Le establecemos una velocidad nula, para que se esté quietecita :D
          _frutas->at(_fruta_aleatoria).getBtRigidBody()->setLinearVelocity(btVector3(0,0,0));
      }
  }
  return true;
}

bool PlayState::frameEnded(const Ogre::FrameEvent& evt)
{
  //  if (_exitGame) 
  //    return false;
  
  return true;
}




bool PlayState::keyPressed(const OIS::KeyEvent &e)
{
  if (!user_name_txt->getVisible())
  {
//        if (paused) pause(); // ESTO NO ES NECESARIO. AL HACER UN PUSHSTATE EL GAMEMANAGER LLAMARÁ A PAUSE() AUTOMATICAMENTE
//        else
        if (!paused)
        {
              if (e.key == OIS::KC_P) {
                paused = true;
                pushState(PauseState::getSingletonPtr());
              }
              else if (e.key == OIS::KC_G) {
                paused = true;
                game_over();
              }
              else if (e.key == OIS::KC_W) {
                win();
             }
             else if (e.key == OIS::KC_UP)
             {
               _pacmanDir = UP_DIR;
             }
             else if (e.key == OIS::KC_DOWN) {
               _pacmanDir = DOWN_DIR;
             }
             else if (e.key == OIS::KC_LEFT) {
               _pacmanDir = LEFT_DIR;
             }
             else if (e.key == OIS::KC_RIGHT) {
               _pacmanDir = RIGHT_DIR;
             }
//             else if (e.key == OIS::KC_ESCAPE) {              
//               popState();
//               pushState(IntroState::getSingletonPtr());
//             }
        }
  }
  else
  {
    sounds::getInstance()->play_effect("eat_fruit");

    MyGUI::UString txt = user_name_txt->getCaption();
    if ((int)e.key==14 && txt.size()>0) txt.resize(txt.size()-1);
    else
    {
      if (((int)e.text >=65 && (int)e.text<=90) || ((int)e.text>=97 && (int)e.text<=122))
      {
        if (txt.size()<3) txt.push_back(e.text);
      }
    }
    user_name_txt->setCaption(txt);
    if (e.key==OIS::KC_RETURN)
    {
      cout << "NEW RECORD TO SAVE" << endl;
        records::getInstance()->add_record(txt,get_score());
        records::getInstance()->saveFile(NULL);
        sounds::getInstance()->play_effect("eat_ghost");
        user_name_txt->setVisible(false);
        popState();
        stopWorld = false;
    }
  }
  return true;

}

bool PlayState::keyReleased(const OIS::KeyEvent &e)
{
/*  if (e.key == OIS::KC_ESCAPE) 
  {  
    if (paused)
    {
        message_txt->setVisible(false);
        message_wall->setVisible(false);
        message_txt->setCaption("");
        paused=false;
        //changeState(MenuState::getSingletonPtr());
        popState();
    }
  }
  else*/ if (e.key == OIS::KC_DOWN ||  e.key == OIS::KC_UP
           || e.key == OIS::KC_LEFT || e.key == OIS::KC_RIGHT) {
    _pacmanDir = 0;
  }
  return true;
}

bool PlayState::mouseMoved(const OIS::MouseEvent &e)
{
  return true;
}

bool PlayState::mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
  return true;
}

bool PlayState::mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
  return true;
}

PlayState* PlayState::getSingletonPtr()
{

  return msSingleton;
}

PlayState& PlayState::getSingleton()
{ 
  assert(msSingleton);
  return *msSingleton;
}

PlayState::~PlayState()
{


}




/*
 *Create Scene, camera etc
 */

void PlayState::createScene()
{

  //    _camera->setPosition (Vector3 (0,15,0));
   _camera->setPosition (Vector3 (0,5,-10));
  _camera->lookAt (Vector3 (0,0,0.1));

  //     _camera->setProjectionType(Ogre::PT_ORTHOGRAPHIC);
  //    _camera->setOrthoWindowHeight(15);
  _sceneMgr->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));
  _sceneMgr->setShadowTechnique(SHADOWTYPE_STENCIL_MODULATIVE);
  _sceneMgr->setShadowColour(ColourValue(0.5, 0.5, 0.5));
  createLight();
  createFloor();
  createLevel();
  
  createFruits();
  createMyGui();
}

void PlayState::createMyGui()
{
   string name="";
   char points_str [32];
   int points=0;

   layout = MyGUI::LayoutManager::getInstance().loadLayout("pacman_play.layout");
   high_score_txt = MyGUI::Gui::getInstance().findWidget<MyGUI::EditBox>("high_score");
   score_txt = MyGUI::Gui::getInstance().findWidget<MyGUI::EditBox>("score");
   lives_txt = MyGUI::Gui::getInstance().findWidget<MyGUI::EditBox>("lives");
   user_name_txt = MyGUI::Gui::getInstance().findWidget<MyGUI::EditBox>("user_name");
   message_txt = MyGUI::Gui::getInstance().findWidget<MyGUI::EditBox>("message");
   message_wall = MyGUI::Gui::getInstance().findWidget<MyGUI::ImageBox>("message_wall");
   message_txt->setCaption("");
   message_txt->setVisible(false);
   message_wall->setVisible(false);
   user_name_txt->setCaption("");
   user_name_txt->setVisible(false);

   records::getInstance()->getBest(name,points);
   sprintf(points_str,"%d",points);
   high_score_txt->setCaption(points_str);
   set_score(0);
   set_lives(3);
}

/*pp
 *createLights
 */
void PlayState::createLight() {
  _sceneMgr->setShadowTextureCount(2);
  _sceneMgr->setShadowTextureSize(512);
  Light* light = _sceneMgr->createLight("Light1");
  light->setPosition(0, 12, 0);
  light->setType(Light::LT_SPOTLIGHT);
  light->setDirection(Vector3(0, -1, 0));
  light->setSpotlightInnerAngle(Degree(60.0f));
  light->setSpotlightOuterAngle(Degree(80.0f));
  light->setSpotlightFalloff(0.0f);
  light->setCastShadows(true);
  /*
  Light* light2 = _sceneMgr->createLight("Light2");
  light2->setPosition(5, 12, 2);
  light2->setType(Light::LT_SPOTLIGHT);
  light2->setDirection(Vector3(-1, -1, 0));
  light2->setSpotlightInnerAngle(Degree(25.0f));
  light2->setSpotlightOuterAngle(Degree(60.0f));
  light2->setSpotlightFalloff(0.0f);
  light2->setCastShadows(true);
  */
}


void PlayState::createPhantoms(){
  graphml_boost::ruta_t phantomZone = graphLevel->getVertices(PHANTOM_START_NODE);
   _phantoms = PhantomFactory::getInstance().createAllPhantoms(_world,phantomZone);
   cout << "Creados " << _phantoms->size() << " fantasmas\n";
}

void PlayState::createFruits()
{
 _frutas = FruitFactory::getInstance().createAllFruits(_world);

 for (size_t i=0; i< _frutas->size(); i++)
     cout << "posicion frutas " << _frutas->at(i).getPosition() << endl;

  graphml_boost::nodo_props nodo = graphLevel->getNodoAleatorio();
  Ogre::Vector3 donde(atof(nodo.x.c_str()),
                      atof(nodo.y.c_str()),
                      atof(nodo.z.c_str()));
  cout << "VA A APARECER UNA FRUTA EN " << donde << endl;
  
  _fruta_aleatoria = rand() % _frutas->size();
  //_fruta_aleatoria = 1;
  _frutas->at(_fruta_aleatoria).aparece(donde);

}

void PlayState::createPacman(){
  _pacmanDir = 0;
  graphml_boost::nodo_props  nodePacmanStart = *(graphLevel->getVertices(PACMAN_START_NODE).begin());
  Vector3 position = Vector3(atof(nodePacmanStart.x.c_str()),
                                   atof(nodePacmanStart.y.c_str()),
                                   atof(nodePacmanStart.z.c_str()));
  _pacman = new Pacman(_world, position, nodePacmanStart.idBoost);
}
/*
 * Create Pacman Level
 *
 */
void PlayState::createLevel(){
  StaticGeometry* stage =   _sceneMgr->createStaticGeometry("SG");
  Entity* entLevel = _sceneMgr->  createEntity("level1.mesh");
  
  entLevel->setCastShadows(true);
  stage->addEntity(entLevel, Vector3(0,0,0));
  stage->build();
  
  StaticMeshToShapeConverter *trimeshConverter = new StaticMeshToShapeConverter(entLevel);
  TriangleMeshCollisionShape *trackTrimesh = trimeshConverter->createTrimesh();
  RigidBody *rigidLevel = new  RigidBody("level", _world,COL_WALL,COL_PILL | COL_PACMAN);
  rigidLevel->setStaticShape(trackTrimesh, 0.0, 0.0, Vector3::ZERO, Quaternion::IDENTITY);
  
  std::string fileName = "./blender/level1.xml";
  
  graphLevel = new graphml_boost();
  graphLevel->cargaGrafo(fileName);
  
   paintPills(false);
   paintPills(true);  
    
   createPacman();
  createPhantoms();
}

void PlayState::paintPills(bool bigpill){
  graphml_boost::ruta_t pillsNodes  = graphLevel->getVertices(bigpill ? BIGPILL_NODE : REGULAR_NODE);
  for(graphml_boost::ruta_t::iterator it = pillsNodes.begin(); it != pillsNodes.end(); ++it) {

    graphml_boost::nodo_props node = *it;
    Vector3 position = Vector3(atof(node.x.c_str()),
                                   atof(node.y.c_str()),
                                   atof(node.z.c_str()));
    Pill pill = Pill(_world, position, bigpill, atoi(node.id.c_str()),node.idBoost);
    _pills.push_back(pill);

  }
}
  

/*
 * Create pacman floor
 */
void PlayState::createFloor() {
  SceneNode* floorNode = _sceneMgr->createSceneNode("floor");
  Plane planeFloor;
  planeFloor.normal = Vector3(0, 1, 0);
  planeFloor.d = 2;
  MeshManager::getSingleton().createPlane("FloorPlane",
                                          ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
                                          planeFloor, 200000, 200000, 20, 20,
                                          true, 1, 9000, 9000,
                                          Vector3::UNIT_Z);
  Entity* entFloor = _sceneMgr->createEntity("floor", "FloorPlane");
  entFloor->setCastShadows(false);
  entFloor->setMaterialName("floor");
  
  floorNode->attachObject(entFloor);
  _sceneMgr->getRootSceneNode()->addChild(floorNode);
  floorNode->setPosition(Vector3(0,2,0));
  CollisionShape *shape = new StaticPlaneCollisionShape (
      Ogre::Vector3(0, 1, 0), 0);
  RigidBody *rigidBodyPlane = new RigidBody("rigidBodyPlane", _world,COL_FLOOR,COL_PILL|COL_PACMAN | COL_PHANTOM);
  rigidBodyPlane->setStaticShape(shape, 0.1, 0);

}

void PlayState::win()
{
  message ("YOU WIN!!");
  sounds::getInstance()->play_effect("intermission");
}
void PlayState::message(string msg)
{
  if (msg.length()>0)
  {
    message_txt->setVisible(true);
    message_wall->setVisible(true);
    message_txt->setCaption(msg);
  }
  else
  {
    message_txt->setVisible(false);
    message_wall->setVisible(false);
    message_txt->setCaption("");
  }
}
void PlayState::game_over()
{
  stopWorld = true;
  //set_lives(0);
  message ("GAME OVER");
  sounds::getInstance()->play_effect("pacman_death");
  user_name_txt->setCaption("");
  user_name_txt->setVisible(true);
  MyGUI::InputManager::getInstance().setKeyFocusWidget(user_name_txt);
}

void PlayState::set_lives (int lives)
{
  if (lives <0) lives =0;
  char tmp [64];
  this->lives = lives;
  sprintf(tmp,"%d",lives);
  lives_txt->setCaption(tmp);
}

int PlayState::get_lives ()
{
  return lives;
}

void PlayState::set_score (int score)
{
  char tmp [64];
  this->score = score;
  sprintf(tmp,"%d",score);
  score_txt->setCaption(tmp);
}

int PlayState::get_score ()
{
  return score;

}

//void PlayState::setPhantomsAfraid(bool afraid){
//         for (std::vector<Phantom>::iterator it = _phantoms->begin();
//               it != _phantoms->end(); ++it) {
//           (*it).setAfraid(afraid);
//           
//         
//          }
//         if(afraid){
//           timeAfraid = static_cast<long int> (time(NULL));
//         }
//         else{
//           timeAfraid=-1;
//         }
//}

void PlayState::setPhantomsAfraid(bool afraid)
{
    if(afraid)
    {
        for (std::vector<Phantom>::iterator it = _phantoms->begin(); it != _phantoms->end(); ++it) 
          if ((*it).getEstado() != estadoPhantom::MUERTO){
                (*it).changeStatePhantom(estadoPhantom::ACOJONADO);
                timeAfraid = static_cast<long int> (time(NULL));
          }
    }
    
    else
    {
        for (std::vector<Phantom>::iterator it = _phantoms->begin(); it != _phantoms->end(); ++it)
            if ((*it).getEstado() != estadoPhantom::MUERTO)
            {
                (*it).changeStatePhantom(estadoPhantom::NORMAL);
                timeAfraid=-1;
            
            }
    }
}


 int PlayState::getFarNode(){

  Ogre::Real distance = 0;
  int idFarNode = -1;
  graphml_boost::ruta_t nodes  = graphLevel->getVertices(REGULAR_NODE);
  graphml_boost::nodo_props  pacNode= graphLevel->getGraphNode(_pacman->getCurrentNode());
   Vector3 positionPacman = Vector3(atof(pacNode.x.c_str()),
                                   atof(pacNode.y.c_str()),
                                   atof(pacNode.z.c_str()));

   for (graphml_boost::ruta_t::iterator it = nodes.begin();
        it != nodes.end(); ++it) {
     Vector3 positionNode = Vector3(atof((*it).x.c_str()),
                                    atof((*it).y.c_str()),
                                    atof((*it).z.c_str()));
        Ogre::Real nodeDistance = positionNode.squaredDistance(positionPacman);
        if(nodeDistance > distance){
          distance = nodeDistance;
          idFarNode = (*it).idBoost;
        }
        
  }
   return idFarNode;


  
}
 int PlayState::getPacmanNode(){

  Ogre::Real distance = 10000;
  int idPacmanNode = -1;
  graphml_boost::ruta_t nodes  = graphLevel->getVertices(REGULAR_NODE);
  Vector3 positionPacman =   _pacman->getPosition();


   for (graphml_boost::ruta_t::iterator it = nodes.begin();
        it != nodes.end(); ++it) {
     Vector3 positionNode = Vector3(atof((*it).x.c_str()),
                                    atof((*it).y.c_str()),
                                    atof((*it).z.c_str()));
        Ogre::Real nodeDistance = positionNode.squaredDistance(positionPacman);
        if(nodeDistance < distance){
          distance = nodeDistance;
          idPacmanNode = (*it).idBoost;
        }
        
  }
   return idPacmanNode;


  
}
/**
 * Manage collisions with Pacman
 */
void PlayState::handleCollision(btCollisionObject *body0, btCollisionObject *body1) {

    if (body0 == _pacman->getBtRigidBody() ||
      body1 == _pacman->getBtRigidBody()) {

    btCollisionObject* pacBody = (body0 == _pacman->getBtRigidBody()) ? body0 : body1;
    btCollisionObject* otherObject = (pacBody == body0)?body1:body0;
    // Check pills collision
    for (std::vector<Pill>::iterator it = _pills.begin();
         
      it != _pills.end(); ++it) {
     


      if ( (*it).getBtRigidBody() == otherObject) {



        if(!(*it).isEaten()){
          sounds::getInstance()->halt_effect();
          sounds::getInstance()->play_effect("chomp2");
          int points = (*it).isBig()? 50 : 10;
          (*it).eat();
          _world->getBulletDynamicsWorld()->removeCollisionObject((*it).getBtRigidBody());
                OgreUtil::destroySceneNode((*it).getSceneNode());


          
                set_score(score+points);
                if((*it).isBig()){
                    sounds::getInstance()->play_effect("energizer");
                    setPhantomsAfraid(true);

                }
                it = _pills.erase(it);
        }


          

        break;

      }
    }

    //Check Phantom Collision
    for (std::vector<Phantom>::iterator it = _phantoms->begin();it != _phantoms->end(); ++it) 
    {
        //Phantom phantom = *it;

        //if ( phantom.getBtRigidBody() == otherObject) 
        if ( (*it).getBtRigidBody() == otherObject) 
        {
            //switch (phantom.getEstado())
            switch ((*it).getEstado())
            {
                case estadoPhantom::NORMAL:
                             // Hay que preguntar si ya se estaba muriendo, de lo contrario, los fantasmas se ensañan y mientras se está
                             // muriendo la primera vez, le siguen dando y se acumulan las muertes :D
                             if (!_pacman->getEstoyMuriendo())
                             {
                                    if (get_lives() > 0)
                                    {
                                        sounds::getInstance()->halt_effect();
                                        sounds::getInstance()->play_effect("pacman_death");
                                        cout << get_lives() << endl;
                                    }
                                    set_lives(get_lives()-1);
                                    if(get_lives() == 0)
                                    {
                                      _pacman->stop();
                                      game_over();
                                    }
                                    else
                                    {
                                        _pacman->arrancaMuerte(_deltaT);
        //                              reseteaPersonajes();
        //                              _resucitando = true;
        //                              stopWorld = true;
        //                              pushState(PauseState::getSingletonPtr());
                                    }
                             }
                            break;
                case estadoPhantom::ACOJONADO: 
                            //phantom.changeStatePhantom(estadoPhantom::MUERTO); 
                            (*it).changeStatePhantom(estadoPhantom::MUERTO); 
                case estadoPhantom::MUERTO:; // Si esta muerto déjalo estar :D
            }
            
            break;
        }
    }
    
    // Check for fruit collision
        if (_frutas->at(_fruta_aleatoria).getBtRigidBody() == otherObject)
        {
            sounds::getInstance()->play_effect("eat_fruit");
            _world->getBulletDynamicsWorld()->removeCollisionObject(_frutas->at(_fruta_aleatoria).getBtRigidBody());
            _frutas->at(_fruta_aleatoria).desaparece();
            //set_score(score + 100); // de momento puntua 100, tengo que poner un enum con puntuaciones por frutas
        }
            


    }




}
void PlayState::unloadLayout()
{
  MyGUI::LayoutManager::getInstance().unloadLayout(layout);
}

bool PlayState::getResucitando()
{
    return _resucitando;
}


void PlayState::reseteaPersonajes()
{
    _pacman->reset();
    for (std::vector<Phantom>::iterator it2 = _phantoms->begin(); it2 != _phantoms->end(); ++it2)
        (*it2).reset();

//  _resucitando = true;
//  stopWorld = true;
//  pushState(PauseState::getSingletonPtr());

}