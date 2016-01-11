#include "PauseState.h"
#include "IntroState.h"
#include <OgreOverlaySystem.h>
#include <OgreOverlayElement.h>
#include <OgreOverlayManager.h>

template<> PauseState* Ogre::Singleton<PauseState>::msSingleton = 0;

void PauseState::enter ()
{
  _root = Ogre::Root::getSingletonPtr();

  // Se recupera el gestor de escena y la cámara.
  _sceneMgr = _root->getSceneManager("SceneManager");
  _camera = _sceneMgr->getCamera("IntroCamera");
  _viewport = _root->getAutoCreatedWindow()->getViewport(0);
  //_viewport = _root->getAutoCreatedWindow()->addViewport(_camera);
  // Nuevo background colour.
  _viewport->setBackgroundColour(Ogre::ColourValue(0.0, 1.0, 0.0));

  mostrarPausa();

  _exitGame = false;
}

void PauseState::exit()
{
}

void PauseState::pause()
{
}

void PauseState::resume()
{
}

bool PauseState::frameStarted(const Ogre::FrameEvent& evt)
{
  return true;
}

bool PauseState::frameEnded(const Ogre::FrameEvent& evt)
{
//  if (_exitGame)
//    return false;
  
  return true;
}

void PauseState::keyPressed(const OIS::KeyEvent &e) 
{
  // Tecla p --> Estado anterior.
  if (e.key == OIS::KC_P) 
  {
    popState();
  }
}

void PauseState::keyReleased(const OIS::KeyEvent &e)
{
    if (e.key == OIS::KC_ESCAPE)
    {
        //Esto solo cuando queramos volver al menu principal, de lo contrario 
        //si volvemos al PlayState nos cargaríamos la escena del juego en marcha.
        _sceneMgr->clearScene(); 
        _root->getAutoCreatedWindow()->removeAllViewports();
        
        changeState(IntroState::getSingletonPtr());
        
    }
}

void PauseState::mouseMoved(const OIS::MouseEvent &e)
{
}

void PauseState::mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
}

void PauseState::mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
}

PauseState* PauseState::getSingletonPtr ()
{
    return msSingleton;
}

PauseState& PauseState::getSingleton ()
{ 
  assert(msSingleton);
  return *msSingleton;
}

void PauseState::mostrarPausa()
{
    // Si no creamos el OverlaySystem el overlayManager no chuta
    Ogre::OverlaySystem* sys = new Ogre::OverlaySystem(); 
    
    (void)sys;
    
    Ogre::OverlayManager* overlayManager = Ogre::OverlayManager::getSingletonPtr();
    // Create an overlay
    Ogre::Overlay* overlay = overlayManager->create("OverlayPausa");

    // Create a panel
    Ogre::OverlayContainer* panel = static_cast<Ogre::OverlayContainer*>(overlayManager->createOverlayElement("Panel", "PanelPausa"));
    panel->setPosition( 0.8, 0.8 );
    panel->setDimensions( 0.5, 0.5 );
    panel->setMaterialName("matPausa");
    
    // Add the panel to the overlay
    overlay->add2D(panel);
    overlay->setZOrder(0);

    // Show the overlay
    overlay->show();
}
