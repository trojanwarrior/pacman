#include "ControlsState.h"
#include "PauseState.h"
#include "IntroState.h"
#include "PlayState.h"
#include "MenuState.h"
#include <stdlib.h>
#include "records.h"

//http://www.cplusplus.com/doc/tutorial/templates/          <--------Visita esta página para entender la linea justo debajo de esta
template<> ControlsState* Ogre::Singleton<ControlsState>::msSingleton = 0;

using namespace std;
using namespace Ogre;

void ControlsState::enter ()
{
  _root = Ogre::Root::getSingletonPtr();

  // Se recupera el gestor de escena y la cámara.
  _sceneMgr = _root->getSceneManager("SceneManager");
  _camera = _sceneMgr->getCamera("IntroCamera");
  _viewport = _root->getAutoCreatedWindow()->getViewport(0);
  // Nuevo background colour.
  _viewport->setBackgroundColour(Ogre::ColourValue(0.0, 0.0, 1.0));
  
  //mostrarFondo(); 
  createScene();

  _exitGame = false;
 sounds::getInstance()->play_effect("eat_ghost");
}

void ControlsState::exit ()
{
  _sceneMgr->clearScene();
  _root->getAutoCreatedWindow()->removeAllViewports();
MyGUI::LayoutManager::getInstance().unloadLayout(layout);

}

void ControlsState::pause()
{
}

void ControlsState::resume()
{
  // Se restaura el background colour.
  _viewport->setBackgroundColour(Ogre::ColourValue(0.0, 0.0, 1.0));
}

bool ControlsState::frameStarted(const Ogre::FrameEvent& evt)
{
  return true;
}

bool ControlsState::frameEnded(const Ogre::FrameEvent& evt)
{
//  if (_exitGame)
//    return false;
  
  return true;
}


bool ControlsState::keyPressed(const OIS::KeyEvent &e)
{
  cout << __FUNCTION__ << endl;
  popState();
  cout << "After POP STATE " << endl;
  return true;
  // Tecla p --> PauseState.
  if (e.key == OIS::KC_P) {
    pushState(PauseState::getSingletonPtr());
  }
  else if (e.key == OIS::KC_S) {
    pushState(PlayState::getSingletonPtr());
  }
  return true;
}

bool ControlsState::keyReleased(const OIS::KeyEvent &e)
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
  return true;
}

bool ControlsState::mouseMoved(const OIS::MouseEvent &e)
{
  return true;
}

bool ControlsState::mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
  return true;
}

bool ControlsState::mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
  return true;
}

ControlsState* ControlsState::getSingletonPtr()
{
cout << __FUNCTION__<<endl;
    return msSingleton;
}

ControlsState& ControlsState::getSingleton()
{ 
  assert(msSingleton);
  return *msSingleton;
}

ControlsState::~ControlsState()
{
    // Don't forget to delete the Rectangle2D in the destructor of your application:
    delete _rect;
}


// Function to put a non-power 2 size image in the upper left corner of a larger size texture
//CODIGO DUPLICADO, LO SÉ, YA LO ORGANIZARÉ MEJOR
TextureUnitState* ControlsState::CreateTextureFromImgWithoutStretch(const String& texName, Real texSize, const String& imgName)
{
    TexturePtr tex = TextureManager::getSingleton().createManual(
        texName,
        "General",
        TEX_TYPE_2D,
        texSize, texSize,
        1, PF_R8G8B8);

    // get image
    Image img;
    img.load(imgName, "General");

    // Copy image to the upper left corner of the texture.
    tex->getBuffer(0,0)->blitFromMemory(img.getPixelBox(0,0), Image::Box(0, 0, img.getWidth(), img.getHeight()));

    MaterialPtr pmat = MaterialManager::getSingleton().create(texName, 
                                            ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME); 
    pmat->getTechnique(0)->getPass(0)->createTextureUnitState(); 
    pmat->getTechnique(0)->getPass(0)->getTextureUnitState(0)->setTextureName(texName); 
    pmat->getTechnique(0)->getPass(0)->getTextureUnitState(0)->setTextureFiltering(FO_NONE,FO_NONE,FO_NONE); 
    pmat->getTechnique(0)->getPass(0)->setDepthCheckEnabled(false);
    pmat->getTechnique(0)->getPass(0)->setDepthWriteEnabled(false);
    pmat->getTechnique(0)->getPass(0)->setLightingEnabled(false);

    TextureUnitState* texState= pmat->getTechnique(0)->getPass(0)->getTextureUnitState(0);        
    
    // Example of background scrolling
    //pmat->getTechnique(0)->getPass(0)->getTextureUnitState(0)->setScrollAnimation(-0.25, 0.0);


    return texState;    
}

void ControlsState::mostrarFondo()
{
    String texName = "FondoPlay";
    Real texW = 1024;

    // Create background material
    Ogre::TexturePtr tex = CreateTextureFromImgWithoutStretch(texName, texW, "pacman_screen_800x600.jpg")->_getTexturePtr();;

    Real windowW = 800;
    Real windowH = 600;

    // Create background rectangle covering the whole screen
    _rect = new Rectangle2D(true);       
    _rect->setCorners(-1.0, 1.0, texW /windowW*2.0+-1.0, 1.0-texW /windowH*2.0);
    _rect->setMaterial(texName);
    
    // Render the background before everything else
    _rect->setRenderQueueGroup(Ogre::RENDER_QUEUE_BACKGROUND);
     
    // Use infinite AAB to always stay visible
    Ogre::AxisAlignedBox aabInf;
    aabInf.setInfinite();
    _rect->setBoundingBox(aabInf);
     
    // Attach background to the scene
    Ogre::SceneNode* node = _sceneMgr->getRootSceneNode()->createChildSceneNode("Background");
    node->attachObject(_rect);
     
}

void ControlsState::createScene()
{
                string name="";
                char points_str [32];
                int points=0;
                layout = MyGUI::LayoutManager::getInstance().loadLayout("pacman_controls.layout");
                high_score_txt = MyGUI::Gui::getInstance().findWidget<MyGUI::EditBox>("high_score");
                records::getInstance()->getBest(name,points);
                sprintf(points_str,"%d",points); 
                high_score_txt->setCaption(points_str);
}

