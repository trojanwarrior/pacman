#include "ControlsState.h"
#include "RecordsState.h"
#include "PauseState.h"
#include "IntroState.h"
#include "PlayState.h"
#include "MenuState.h"
#include "OgreBulletCollisionsShape.h"
#include "Shapes/OgreBulletCollisionsTrimeshShape.h"
#include "Shapes/OgreBulletCollisionsStaticPlaneShape.h"
#include "graphml_boost.h"
#include <string>



//http://www.cplusplus.com/doc/tutorial/templates/          <--------Visita esta página para entender la linea justo debajo de esta
template<> PlayState* Ogre::Singleton<PlayState>::msSingleton = 0;

using namespace std;
using namespace Ogre;
using namespace OgreBulletDynamics;
using namespace OgreBulletCollisions;

void PlayState::enter ()
{
  _root = Ogre::Root::getSingletonPtr();

  // Se recupera el gestor de escena y la cámara.
  _sceneMgr = _root->getSceneManager("SceneManager");
  _camera = _sceneMgr->getCamera("IntroCamera");
  _viewport = _root->getAutoCreatedWindow()->getViewport(0);
  // Nuevo background colour.
  _viewport->setBackgroundColour(Ogre::ColourValue(0.0, 0.0, 1.0));

  AxisAlignedBox boundBox =  AxisAlignedBox (Ogre::Vector3 (-10000, -10000, -10000),Ogre::Vector3 (10000,  10000,  10000));
  _world = new DynamicsWorld(_sceneMgr,boundBox,Vector3(0,-9.81,0),true,true,15000);
  createScene();
  _pacman = new Pacman(_world);
  _pacmanDir = 0;


  _debugDrawer = new OgreBulletCollisions::DebugDrawer();
  _debugDrawer->setDrawWireframe(true);
  SceneNode *node = _sceneMgr->getRootSceneNode()->
                    createChildSceneNode("debugNode", Vector3::ZERO);
  node->attachObject(static_cast<SimpleRenderable*>(_debugDrawer));
  _world->setDebugDrawer (_debugDrawer);
  // _world->setShowDebugShapes (true);  

  _exitGame = false;
  sounds::getInstance()->play_effect("intermission");
}

void PlayState::exit ()
{
  _sceneMgr->clearScene();
  _root->getAutoCreatedWindow()->removeAllViewports();
  delete _pacman;
  MyGUI::LayoutManager::getInstance().unloadLayout(layout);
}

void PlayState::pause()
{
}

void PlayState::resume()
{
  // Se restaura el background colour.
  _viewport->setBackgroundColour(Ogre::ColourValue(0.0, 0.0, 1.0));
}

bool PlayState::frameStarted(const Ogre::FrameEvent& evt)
{
  _world->stepSimulation(evt.timeSinceLastFrame);
  if (_pacmanDir != 0) {
    _pacman->move(_pacmanDir, evt.timeSinceLastFrame);
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


  // Tecla p --> PauseState.
  if (e.key == OIS::KC_P) {
    pushState(PauseState::getSingletonPtr());
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
  else if (e.key == OIS::KC_ESCAPE) {

    popState();
    pushState(IntroState::getSingletonPtr());
  }
  return true;
}

bool PlayState::keyReleased(const OIS::KeyEvent &e)
{
  if (e.key == OIS::KC_ESCAPE) 
  {
    //Ahora mismo lo dejo así, se resetea la pila de estados
    //y volvemos al InstroState denotado por el cambio de color del fondo.
    //Aquí en realidad hay que hacer un pushState(PauseState::getSingletonPtr())
    //y apilamos el estado de pausa donde debería aparecer la gui de pausa.
    //En ese estado daremos la opción de proseguir que provocará que se active el resume
    //del estado PlayState, o sea, del estado inmendiatamente debajo del de pausa en la pila.
    //O bien, la opción de terminar el juego y volver al menú principal lo que provocará un reseteo
    //de toda la pila de estados (gestionada por GameManager).
    //Para ver como funciona la pausa, presiona la tecla p para activarla (apilarla) y otra vez a p
    //para volver al estado PlayState.
    changeState(IntroState::getSingletonPtr());
  }
  else if (e.key == OIS::KC_DOWN ||  e.key == OIS::KC_UP
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
    _camera->setPosition (Vector3 (0,15,0));
  _camera->lookAt (Vector3 (0,0,0.1));

      _camera->setProjectionType(Ogre::PT_ORTHOGRAPHIC);
        _camera->setOrthoWindowHeight(15);
  _sceneMgr->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));
  _sceneMgr->setShadowTechnique(SHADOWTYPE_STENCIL_MODULATIVE);
  _sceneMgr->setShadowColour(ColourValue(0.5, 0.5, 0.5));
  createLight();
  createLevel();
  createFloor();
}

/*
 *createLights
 */
void PlayState::createLight() {
  _sceneMgr->setShadowTextureCount(1);
  _sceneMgr->setShadowTextureSize(512);
  Light* light = _sceneMgr->createLight("Light1");
  light->setPosition(-5, 12, 2);
  light->setType(Light::LT_SPOTLIGHT);
  light->setDirection(Vector3(1, -1, 0));
  light->setSpotlightInnerAngle(Degree(25.0f));
  light->setSpotlightOuterAngle(Degree(60.0f));
  light->setSpotlightFalloff(0.0f);
  light->setCastShadows(true);
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
  RigidBody *rigidLevel = new  RigidBody("level", _world);
  rigidLevel->setStaticShape(trackTrimesh, 0.0, 0.0, Vector3::ZERO, Quaternion::IDENTITY);
  //  std::string &fileName = "/home/flush/CEDV/pacman/pacman/blender";
  graphLevel = new graphml_boost();
  graphLevel->cargaGrafo("/home/flush/CEDV/pacman/pacman/blender/level1.xml");
  graphLevel->getVertex("bigpill");
  
  
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
  CollisionShape *shape = new StaticPlaneCollisionShape (
      Ogre::Vector3(0, 1, 0), 0);
  RigidBody *rigidBodyPlane = new RigidBody("rigidBodyPlane", _world);
  rigidBodyPlane->setStaticShape(shape, 0.1, 0);
}


