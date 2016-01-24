#include "ControlsState.h"
#include "RecordsState.h"
#include "PauseState.h"
#include "IntroState.h"
#include "PlayState.h"
#include "MenuState.h"
#include "Base/Main.h"

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
  _viewport->setBackgroundColour(Ogre::ColourValue(0.0, 0.0, 1.0));
  
  //mostrarFondo();
  createScene();

  _exitGame = false;
 sounds::getInstance()->play_effect("intermission");
}

void PlayState::exit ()
{
  _sceneMgr->clearScene();
  _root->getAutoCreatedWindow()->removeAllViewports();
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
  popState();
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
    // Don't forget to delete the Rectangle2D in the destructor of your application:
    delete _rect;
}


// Function to put a non-power 2 size image in the upper left corner of a larger size texture
//CODIGO DUPLICADO, LO SÉ, YA LO ORGANIZARÉ MEJOR
TextureUnitState* PlayState::CreateTextureFromImgWithoutStretch(const String& texName, Real texSize, const String& imgName)
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

void PlayState::mostrarFondo()
{
    String texName = "FondoPlay";
    Real texW = 1024;

    // Create background material
    Ogre::TexturePtr tex = CreateTextureFromImgWithoutStretch(texName, texW, "pacman_screen_800x600.jpg")->_getTexturePtr();

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

void PlayState::createScene()
{
//JAM
                 MyGUI::EditBox* high_score_txt;
                layout = MyGUI::LayoutManager::getInstance().loadLayout("pacman_play.layout");
                const MyGUI::VectorWidgetPtr& root = MyGUI::LayoutManager::getInstance().loadLayout("HelpPanel.layout");
                if (root.size() == 1)
                root.at(0)->findWidget("Text")->castType<MyGUI::TextBox>()->setCaption("pacman");
                high_score_txt = MyGUI::Gui::getInstance().findWidget<MyGUI::EditBox>("high_score");
//JAM
return;
  
 Entity* entCereza = _sceneMgr->createEntity("Cereza.mesh");
    Ogre::SceneNode* nodeCereza = _sceneMgr->createSceneNode("CerezaNode");
    nodeCereza->attachObject(entCereza);
    _sceneMgr->getRootSceneNode()->addChild(nodeCereza);

    Entity* entPacman = _sceneMgr->createEntity("Pacman.mesh");
    Ogre::SceneNode* nodePacman = _sceneMgr->createSceneNode("PacmanNode");
    nodePacman->attachObject(entPacman);
    _sceneMgr->getRootSceneNode()->addChild(nodePacman);


//  Ogre::Plane plane1(Ogre::Vector3::UNIT_Y, -5);
//  Ogre::MeshManager::getSingleton().createPlane("plane1",
//    Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane1,
//    200,200,1,1,true,1,20,20,Ogre::Vector3::UNIT_Z);
//
//  Ogre::SceneNode* nodeGround = _sceneMgr->createSceneNode("ground");
//  Ogre::Entity* groundEnt = _sceneMgr->createEntity("planeEnt", "plane1");
//  groundEnt->setMaterialName("Ground");
//  nodeGround->attachObject(groundEnt);

  _sceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);
  Ogre::Light* light = _sceneMgr->createLight("Light1");
  light->setType(Ogre::Light::LT_DIRECTIONAL);
  light->setDirection(Ogre::Vector3(1,-1,0));
  //nodeGround->attachObject(light);
  //nodeCereza->attachObject(light);
    nodePacman->attachObject(light);


  //_sceneMgr->getRootSceneNode()->addChild(nodeGround);
}

