#include "ControlsState.h"
#include "RecordsState.h"
#include "PauseState.h"
#include "IntroState.h"
#include "PlayState.h"
#include "MenuState.h"
#include "records.h"

//http://www.cplusplus.com/doc/tutorial/templates/          <--------Visita esta página para entender la linea justo debajo de esta
template<> RecordsState* Ogre::Singleton<RecordsState>::msSingleton = 0;

using namespace std;
using namespace Ogre;

void RecordsState::enter ()
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
  loadRecords();
  _exitGame = false;
   sounds::getInstance()->play_effect("eat_ghost");
}

void RecordsState::loadRecords()
{
          int result = 0;
          int cont=0;
          string name;
          int points;
          string tmp_users="";
          string tmp_points="";
          char tmp_char [64];
          int max_records = 10;
          result = records::getInstance()->getNext(name,points,true);
          cont++;
          while (result == 0)
          {

            if (result == 0)
            {
              sprintf(tmp_char,"\%s\n",name.c_str());
              tmp_users += string(tmp_char);
              sprintf(tmp_char,"\%d\n",points);
              tmp_points += string(tmp_char);
            }
            if (max_records!=0 && cont >= max_records) break;
            result = records::getInstance()->getNext(name,points);
            cont++;
          }
          score_names_txt->setCaption(tmp_users);
          score_points_txt->setCaption(tmp_points);
}
void RecordsState::exit ()
{
  _sceneMgr->clearScene();
  _root->getAutoCreatedWindow()->removeAllViewports();
  MyGUI::LayoutManager::getInstance().unloadLayout(layout);
}

void RecordsState::pause()
{
}

void RecordsState::resume()
{
  // Se restaura el background colour.
  _viewport->setBackgroundColour(Ogre::ColourValue(0.0, 0.0, 1.0));
}

bool RecordsState::frameStarted(const Ogre::FrameEvent& evt)
{
  return true;
}

bool RecordsState::frameEnded(const Ogre::FrameEvent& evt)
{
  return true;
}


bool RecordsState::keyPressed(const OIS::KeyEvent &e)
{
  popState();
  
  return true;
}

bool RecordsState::keyReleased(const OIS::KeyEvent &e)
{
  return true;
}

bool RecordsState::mouseMoved(const OIS::MouseEvent &e)
{
  return true;
}

bool RecordsState::mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
  return true;
}

bool RecordsState::mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
  return true;
}

RecordsState* RecordsState::getSingletonPtr()
{
    return msSingleton;
}

RecordsState& RecordsState::getSingleton()
{ 
  assert(msSingleton);
  return *msSingleton;
}

RecordsState::~RecordsState()
{
    // Don't forget to delete the Rectangle2D in the destructor of your application:
    delete _rect;
}


// Function to put a non-power 2 size image in the upper left corner of a larger size texture
//CODIGO DUPLICADO, LO SÉ, YA LO ORGANIZARÉ MEJOR
TextureUnitState* RecordsState::CreateTextureFromImgWithoutStretch(const String& texName, Real texSize, const String& imgName)
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

void RecordsState::mostrarFondo()
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

void RecordsState::createScene()
{
                string name="";
                char points_str [32];
                int points=0;
                layout = MyGUI::LayoutManager::getInstance().loadLayout("pacman_records.layout");
                high_score_txt = MyGUI::Gui::getInstance().findWidget<MyGUI::EditBox>("high_score");
                score_positions_txt = MyGUI::Gui::getInstance().findWidget<MyGUI::TextBox>("score_positions");
                score_points_txt = MyGUI::Gui::getInstance().findWidget<MyGUI::TextBox>("score_points");
                score_names_txt = MyGUI::Gui::getInstance().findWidget<MyGUI::TextBox>("score_names");
                records::getInstance()->getBest(name,points);
                sprintf(points_str,"%d",points);
                high_score_txt->setCaption(points_str);

}

