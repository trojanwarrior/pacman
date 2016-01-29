#include "ControlsState.h"
#include "RecordsState.h"
#include "PauseState.h"
#include "IntroState.h"
#include "PlayState.h"
#include "MenuState.h"
#include "records.h"

//http://www.cplusplus.com/doc/tutorial/templates/          <--------Visita esta página para entender la linea justo debajo de esta
template<> PlayState* Ogre::Singleton<PlayState>::msSingleton = 0;

using namespace std;
using namespace Ogre;

void PlayState::enter ()
{
  _root = Ogre::Root::getSingletonPtr();

  // Se recupera el gestor de escena y la cámara.
  _sceneMgr = _root->getSceneManager("SceneManager");
  _camera = _sceneMgr->getCamera("IntroCamera");
  _viewport = _root->getAutoCreatedWindow()->getViewport(0);
  // Nuevo background colour.
  _viewport->setBackgroundColour(Ogre::ColourValue(0.0, 0.0, 0.0));

  createScene();
  _pacman = new Pacman();

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
  deltaTime = evt.timeSinceLastFrame;
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

  return true;
  // Tecla p --> PauseState.
  if (e.key == OIS::KC_P) {
    pushState(PauseState::getSingletonPtr());
  }
  else if (e.key == OIS::KC_A)
{    printf("movidendo arriba delta %f \n",deltaTime);
    _pacman->move(UP_DIR, deltaTime);
  }
  else if (e.key == OIS::KC_DOWN) {
    _pacman->move(UP_DIR, deltaTime);
  }
  else if (e.key == OIS::KC_LEFT) {
    _pacman->move(LEFT_DIR, deltaTime);
  }
  else if (e.key == OIS::KC_UP) {
    _pacman->move(RIGHT_DIR, deltaTime);
  }
  else if (e.key == OIS::KC_ESCAPE) {
   printf("movidendo arriba delta %f \n",deltaTime);
    popState();
     pushState(IntroState::getSingletonPtr());
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



void PlayState::createScene()
{
   string name="";
   char points_str [32];
   int points=0;

   layout = MyGUI::LayoutManager::getInstance().loadLayout("pacman_play.layout");
   high_score_txt = MyGUI::Gui::getInstance().findWidget<MyGUI::EditBox>("high_score");
   score_txt = MyGUI::Gui::getInstance().findWidget<MyGUI::EditBox>("score");
   lives_txt = MyGUI::Gui::getInstance().findWidget<MyGUI::EditBox>("lives");

   records::getInstance()->getBest(name,points);
   sprintf(points_str,"%d",points);
   high_score_txt->setCaption(points_str);
   set_score(0);
   set_lives(3);

  _camera->setPosition (Vector3 (0,15,0));
  _camera->lookAt (Vector3 (0,0,0.1));
  //_camera->pitch(Ogre::Degree(-90));
  _camera->setProjectionType(Ogre::PT_ORTHOGRAPHIC);
  _camera->setOrthoWindowHeight(15);


  _sceneMgr->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));
  StaticGeometry* stage =   _sceneMgr->createStaticGeometry("SG");
  Entity* ent1 = _sceneMgr->  createEntity("level1.mesh");
  stage->addEntity(ent1, Vector3(0,0,0));
  stage->build();
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

