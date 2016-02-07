#include "MenuState.h"
#include "PauseState.h"
#include "IntroState.h"
#include "PlayState.h"
#include "ControlsState.h"
#include "RecordsState.h"
#include "records.h"


//http://www.cplusplus.com/doc/tutorial/templates/          <--------Visita esta página para entender la linea justo debajo de esta
template<> MenuState* Ogre::Singleton<MenuState>::msSingleton = 0;

using namespace std;
using namespace Ogre;

void MenuState::enter ()
{
  _root = Ogre::Root::getSingletonPtr();

  // Se recupera el gestor de escena y la cámara.
  _sceneMgr = _root->getSceneManager("SceneManager");
  _camera = _sceneMgr->getCamera("IntroCamera");
  _viewport = _root->getAutoCreatedWindow()->addViewport(_camera);
  // Nuevo background colour.
  _viewport->setBackgroundColour(Ogre::ColourValue(0.0, 0.0, 1.0));
  
  //mostrarFondo(); 
  createScene();

  _exitGame = false;
 sounds::getInstance()->play_effect("eat_ghost");
}

void MenuState::exit ()
{

  _sceneMgr->clearScene();
  _root->getAutoCreatedWindow()->removeAllViewports();

}

void MenuState::pause()
{
}

void MenuState::resume()
{
  enter();
  // Se restaura el background colour.
  _viewport->setBackgroundColour(Ogre::ColourValue(0.0, 0.0, 1.0));
}

bool MenuState::frameStarted(const Ogre::FrameEvent& evt)
{
  return true;
}

bool MenuState::frameEnded(const Ogre::FrameEvent& evt)
{
  if (_exitGame)
    return false;
  
  return true;
}


bool MenuState::keyPressed(const OIS::KeyEvent &e)
{
  cout << __FUNCTION__ << endl;

  if (e.key == OIS::KC_S) {
    MyGUI::LayoutManager::getInstance().unloadLayout(layout);
    pushState(PlayState::getSingletonPtr());
  }
  else if (e.key == OIS::KC_C) {
    MyGUI::LayoutManager::getInstance().unloadLayout(layout);
    pushState(ControlsState::getSingletonPtr());
  }
  else if (e.key == OIS::KC_R) {
    MyGUI::LayoutManager::getInstance().unloadLayout(layout);
    pushState(RecordsState::getSingletonPtr());
  }
  else if (e.key == OIS::KC_ESCAPE) 
  {
    sounds::getInstance()->play_effect("eat_ghost");
    _exitGame = true;
  }
  return true;
}

bool MenuState::keyReleased(const OIS::KeyEvent &e)
{
//  if (e.key == OIS::KC_ESCAPE) 
//  {
//    // Fin del juego, salimos.
//    _exitGame = true;
//  }
  return true;
}

bool MenuState::mouseMoved(const OIS::MouseEvent &e)
{
  return true;
}

bool MenuState::mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
  return true;
}

bool MenuState::mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
  return true;
}

MenuState* MenuState::getSingletonPtr()
{
    cout << __FUNCTION__<<endl;
    return msSingleton;
}

MenuState& MenuState::getSingleton()
{ 
  assert(msSingleton);
  return *msSingleton;
}

MenuState::~MenuState()
{
    // Don't forget to delete the Rectangle2D in the destructor of your application:
    delete _rect;
}


// Function to put a non-power 2 size image in the upper left corner of a larger size texture
//CODIGO DUPLICADO, LO SÉ, YA LO ORGANIZARÉ MEJOR
TextureUnitState* MenuState::CreateTextureFromImgWithoutStretch(const String& texName, Real texSize, const String& imgName)
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

void MenuState::mostrarFondo()
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

void MenuState::createScene()
{
                string name="";
                char points_str [32];
                int points=0;
                layout = MyGUI::LayoutManager::getInstance().loadLayout("pacman_start.layout");
                high_score_txt = MyGUI::Gui::getInstance().findWidget<MyGUI::EditBox>("high_score");
                records::getInstance()->getBest(name,points);
                sprintf(points_str,"%d",points);
                high_score_txt->setCaption(points_str);


}

MenuState::MenuState()
{
  high_score_txt = NULL;
}
