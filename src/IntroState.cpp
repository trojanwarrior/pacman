#include <iostream>
#include "IntroState.h"
#include "PlayState.h"

using namespace std;

template<> IntroState* Ogre::Singleton<IntroState>::msSingleton = 0;

void IntroState::enter ()
{
  _root = Ogre::Root::getSingletonPtr();

  try
  {
    _sceneMgr = _root->getSceneManager("SceneManager");
  }
  catch(...)
  {
    cout << "SceneManager no existe, creándolo \n";  
    _sceneMgr = _root->createSceneManager(Ogre::ST_GENERIC, "SceneManager");
  }
  
  try
  {
    _camera = _sceneMgr->getCamera("IntroCamera");
  }
  catch(...)
  {
    cout << "IntroCamera no existe, creándola \n";  
    _camera = _sceneMgr->createCamera("IntroCamera");
  }
  
  try
  {
    _viewport = _root->getAutoCreatedWindow()->addViewport(_camera);      
  }
  catch(...)
  {
    _viewport = _root->getAutoCreatedWindow()->getViewport(0);
  }
  
  
  //El fondo del pacman siempre es negro
  _viewport->setBackgroundColour(Ogre::ColourValue(0.0, 0.0, 0.0));

  // TODO
  // crear interfaz y mostrarla.
  // En los eventos de teclado, ratón, gamepad cambiaríamos de estado

  _exitGame = false;
}

void IntroState::exit()
{
  _sceneMgr->clearScene();
  _root->getAutoCreatedWindow()->removeAllViewports();
}

void IntroState::pause ()
{
    // Aquí no hay pausa que valga, nada que hacer
}

void IntroState::resume ()
{
    // TODO
    // Aquí hay que repintar la escena en negro y recrear la interfaz
    // y resetear cualquier cosa que nos haga falta. De hecho creo que enter y resume
    // deberían hacer prácticamente lo mismo.
   
}

bool IntroState::frameStarted(const Ogre::FrameEvent& evt) 
{
  return true;
}

bool IntroState::frameEnded(const Ogre::FrameEvent& evt)
{
  if (_exitGame)
    return false;
  
  return true;
}

void IntroState::keyPressed(const OIS::KeyEvent &e)
{
  // Transición al siguiente estado.
  // Espacio --> PlayState
  if (e.key == OIS::KC_SPACE) 
  {
    changeState(PlayState::getSingletonPtr());
  }
}

void IntroState::keyReleased(const OIS::KeyEvent &e )
{
  if (e.key == OIS::KC_ESCAPE)
  {
    _exitGame = true;
  }
}

void IntroState::mouseMoved(const OIS::MouseEvent &e)
{
}

void IntroState::mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
}

void IntroState::mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
}

IntroState* IntroState::getSingletonPtr ()
{
    return msSingleton;
}

IntroState& IntroState::getSingleton ()
{ 
  assert(msSingleton);
  return *msSingleton;
}
