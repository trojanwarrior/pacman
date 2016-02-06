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
#include "graphml_boost.h"
#include <string>
#include <vector>
#include "records.h"
#include "OgreUtil.h"

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
  if (paused)
  {
    message_txt->setVisible(false);
    message_wall->setVisible(false);
    message_txt->setCaption("");
    paused=false;
  }
  else
  { 
    message_txt->setVisible(true);
    message_wall->setVisible(true);
    message_txt->setCaption("PAUSE");
    paused=true;
  }
  sounds::getInstance()->play_effect("eat_fruit");
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
  if(_pacman){
    _pacman->updateAnim(evt.timeSinceLastFrame);
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
    if (paused) pause();
    else
    {
      if (e.key == OIS::KC_P) {
        pause();
      }
      else if (e.key == OIS::KC_G) {
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
     else if (e.key == OIS::KC_ESCAPE) {
       popState();
       pushState(IntroState::getSingletonPtr());
     }
   }
 }
   else
  {
    sounds::getInstance()->play_effect("eat_fruit");
    cout << (int)e.key<<endl;
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
        //records::getInstance()->add_record(txt,get_score());
        records::getInstance()->add_record(txt,9000);
        records::getInstance()->saveFile(NULL);
        sounds::getInstance()->play_effect("eat_ghost");
        user_name_txt->setVisible(false);
        popState();
    }
  }
  return true;

}

bool PlayState::keyReleased(const OIS::KeyEvent &e)
{
  if (e.key == OIS::KC_ESCAPE) 
  {
    // Cambiamos de estado apilando el estado PauseState.
    // Cuando salgamos de PauseState (se desapile el estado PauseState)
    // habrá que controlar lo que se necesite en el método
    // resume.
    pushState(PauseState::getSingletonPtr());

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
  createFruits();

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


void PlayState::createPhantoms(){
   graphml_boost::ruta_t phantomZone = graphLevel->getVertices(PHANTOM_START_NODE);
   PhantomFactory::getInstance().createAllPhantoms(_world,phantomZone);
}

void PlayState::createFruits()
{
  FruitFactory::getInstance().createAllFruits(_world);
}

void PlayState::createPacman(){
  _pacmanDir = 0;
  graphml_boost::nodo_props  nodePacmanStart = *(graphLevel->getVertices(PACMAN_START_NODE).begin());
  Vector3 position = Vector3(atof(nodePacmanStart.x.c_str()),
                                   atof(nodePacmanStart.y.c_str()),
                                   atof(nodePacmanStart.z.c_str()));
  _pacman = new Pacman(_world, position);
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
    Pill pill = Pill(_world, position, bigpill, atoi(node.id.c_str()));
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
  CollisionShape *shape = new StaticPlaneCollisionShape (
      Ogre::Vector3(0, 1, 0), 0);
  RigidBody *rigidBodyPlane = new RigidBody("rigidBodyPlane", _world);
  rigidBodyPlane->setStaticShape(shape, 0.1, 0);

}

void PlayState::win()
{
  message_txt->setVisible(true);
  message_wall->setVisible(true);
  message_txt->setCaption("YOU WIN!!");
  sounds::getInstance()->play_effect("intermission");
}
void PlayState::game_over()
{
  set_lives(0);
  message_txt->setVisible(true);
  message_wall->setVisible(true);
  message_txt->setCaption("GAME OVER");
  sounds::getInstance()->play_effect("pacman_death");
  user_name_txt->setCaption("");
  user_name_txt->setVisible(true);
  MyGUI::InputManager::getInstance().setKeyFocusWidget(user_name_txt);
}
void PlayState::set_lives (int lives)
{
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
      Pill pill = *it;


      if ( pill.getBtRigidBody() == otherObject) {



        int points = pill.isBig()? 50 : 10;
        _world->getBulletDynamicsWorld()->removeCollisionObject(pill.getBtRigidBody());
                it = _pills.erase(it);
        OgreUtil::destroySceneNode(pill.getSceneNode());
        set_score(score+points);
        break;


      }
    }





    }




}

