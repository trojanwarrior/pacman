#include "PlayState.h"
#include "PauseState.h"
#include "IntroState.h"


//http://www.cplusplus.com/doc/tutorial/templates/          <--------Visita esta página para entender la linea justo debajo de esta
template<> PlayState* Ogre::Singleton<PlayState>::msSingleton = 0;

void
PlayState::enter ()
{
  _root = Ogre::Root::getSingletonPtr();

  // Se recupera el gestor de escena y la cámara.
  _sceneMgr = _root->getSceneManager("SceneManager");
  _camera = _sceneMgr->getCamera("IntroCamera");
  _viewport = _root->getAutoCreatedWindow()->addViewport(_camera);
  // Nuevo background colour.
  _viewport->setBackgroundColour(Ogre::ColourValue(0.0, 0.0, 1.0));

  _exitGame = false;
}

void PlayState::exit ()
{
  _sceneMgr->clearScene();
  _root->getAutoCreatedWindow()->removeAllViewports();
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
  return true;
}

bool PlayState::frameEnded(const Ogre::FrameEvent& evt)
{
//  if (_exitGame)
//    return false;
  
  return true;
}

void PlayState::keyPressed(const OIS::KeyEvent &e)
{
  // Tecla p --> PauseState.
  if (e.key == OIS::KC_P) {
    pushState(PauseState::getSingletonPtr());
  }
}

void PlayState::keyReleased(const OIS::KeyEvent &e)
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
}

void PlayState::mouseMoved(const OIS::MouseEvent &e)
{
}

void PlayState::mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
}

void PlayState::mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
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
