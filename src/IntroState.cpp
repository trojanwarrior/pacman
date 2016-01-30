#include <iostream>
#include "IntroState.h"
#include "PlayState.h"
#include "MenuState.h"
#include "MyGUI.h"
#include "MyGUI_OgrePlatform.h"

using namespace std;
using namespace Ogre;

template<> IntroState *Ogre::Singleton<IntroState>::msSingleton = 0;

void IntroState::enter()
{
    _root = Ogre::Root::getSingletonPtr();

    try
    {
        _sceneMgr = _root->getSceneManager("SceneManager");
    }
    catch (...)
    {
        cout << "SceneManager no existe, creándolo \n";
        _sceneMgr = _root->createSceneManager(Ogre::ST_GENERIC, "SceneManager");
    }

    try
    {
        _camera = _sceneMgr->getCamera("IntroCamera");
    }
    catch (...)
    {
        cout << "IntroCamera no existe, creándola \n";
        _camera = _sceneMgr->createCamera("IntroCamera");
    }

    try
    {
        _viewport = _root->getAutoCreatedWindow()->addViewport(_camera);
    }
    catch (...)
    {
        _viewport = _root->getAutoCreatedWindow()->getViewport(0);
    }


    MyGUI::OgrePlatform *mp = new MyGUI::OgrePlatform();
    mp->initialise(_root->getAutoCreatedWindow(), Ogre::Root::getSingleton().getSceneManager("SceneManager"));
    MyGUI::Gui *mGUI = new MyGUI::Gui();
    mGUI->initialise();


    //El fondo del pacman siempre es negro
    _viewport->setBackgroundColour(Ogre::ColourValue(0.0, 0.0, 0.0));

    //Configuramos la camara
    double width = _viewport->getActualWidth();
    double height = _viewport->getActualHeight();
    _camera->setAspectRatio(width / height);
    //_camera->setPosition(Vector3(0, 12, 18));
    _camera->setPosition(Vector3(0, 0, 18));
    _camera->lookAt(_sceneMgr->getRootSceneNode()->getPosition());
    _camera->lookAt(0,0,0);
    _camera->setNearClipDistance(0.1);
    _camera->setFarClipDistance(100);


    // TODO
    // crear interfaz y mostrarla.
    // En los eventos de teclado, ratón, gamepad cambiaríamos de estado
    mostrarFondo();
    createScene();


    _exitGame = false;
    sounds::getInstance()->play_effect("begin");
    _animStatePacman = NULL;
}

void IntroState::exit()
{
    _sceneMgr->clearScene();
    _root->getAutoCreatedWindow()->removeAllViewports();
}

void IntroState::pause()
{
    // Aquí no hay pausa que valga, nada que hacer
}

void IntroState::resume()
{
    // TODO
    // Aquí hay que repintar la escena en negro y recrear la interfaz
    // y resetear cualquier cosa que nos haga falta. De hecho creo que enter y resume
    // deberían hacer prácticamente lo mismo.

}

bool IntroState::frameStarted(const Ogre::FrameEvent &evt)
{
    Ogre::Real deltaT = evt.timeSinceLastFrame;
    Ogre::Real velocidad = 4.0;

    if (!_animStatePacman)
    {
        _animStatePacman = _sceneMgr->getEntity("Pacman")->getAnimationState("pakupaku");
        _animStatePacman->setEnabled(true);
        _animStatePacman->setLoop(true);
        _animStatePacman->setTimePosition(0.0);
    }
    else
        _animStatePacman->addTime(deltaT);

    if (!_animStateClyde)
    {
        _animStateClyde = _sceneMgr->getEntity("ghostClyde")->getAnimationState("mareo2");
        _animStateClyde->setEnabled(true);
        _animStateClyde->setLoop(true);
        _animStateClyde->setTimePosition(0.0);

    }
    else
        _animStateClyde->addTime(deltaT);

    if (!_animStateInky)
    {
        _animStateInky = _sceneMgr->getEntity("ghostInky")->getAnimationState("mareo2");
        _animStateInky->setEnabled(true);
        _animStateInky->setLoop(true);
        _animStateInky->setTimePosition(0.0);

    }
    else
        _animStateInky->addTime(deltaT);

    if (!_animStatePinky)
    {
        _animStatePinky = _sceneMgr->getEntity("ghostPinky")->getAnimationState("mareo2");
        _animStatePinky->setEnabled(true);
        _animStatePinky->setLoop(true);
        _animStatePinky->setTimePosition(0.0);

    }
    else
        _animStatePinky->addTime(deltaT);

    if (!_animStateBlinky)
    {
        _animStateBlinky = _sceneMgr->getEntity("ghostBlinky")->getAnimationState("mareo2");
        _animStateBlinky->setEnabled(true);
        _animStateBlinky->setLoop(true);
        _animStateBlinky->setTimePosition(0.0);

    }
    else
        _animStateBlinky->addTime(deltaT);


    Ogre::Vector3 v(_sceneMgr->getSceneNode("nodePacmanIntro")->getPosition());
    v += (Ogre::Vector3(-1,0,0) * deltaT * velocidad);
    _sceneMgr->getSceneNode("nodePacmanIntro")->setPosition(v);

    return true;
}

bool IntroState::frameEnded(const Ogre::FrameEvent &evt)
{
    if (_exitGame)
        return false;

    return true;
}

bool IntroState::keyPressed(const OIS::KeyEvent &e)
{
    // Transición al siguiente estado.
    // Espacio --> PlayState
    if (e.key == OIS::KC_SPACE)
    {
        changeState(MenuState::getSingletonPtr());
    }
    return true;
}

bool IntroState::keyReleased(const OIS::KeyEvent &e)
{
    if (e.key == OIS::KC_ESCAPE)
    {

        _exitGame = true;
    }
    return true;
}

bool IntroState::mouseMoved(const OIS::MouseEvent &e)
{
    return true;
}

bool IntroState::mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
    return true;
}

bool IntroState::mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
    return true;
}

IntroState *IntroState::getSingletonPtr()
{
    return msSingleton;
}

IntroState &IntroState::getSingleton()
{
    assert(msSingleton);
    return *msSingleton;
}

IntroState::~IntroState()
{
    // Don't forget to delete the Rectangle2D in the destructor of your application:
    delete _rect;

}

// Function to put a non-power 2 size image in the upper left corner of a larger size texture
TextureUnitState *IntroState::CreateTextureFromImgWithoutStretch(const String &texName, Real texSize,
                                                                 const String &imgName)
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
    tex->getBuffer(0, 0)->blitFromMemory(img.getPixelBox(0, 0), Image::Box(0, 0, img.getWidth(), img.getHeight()));

    MaterialPtr pmat = MaterialManager::getSingleton().create(texName,
                                                              ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
    pmat->getTechnique(0)->getPass(0)->createTextureUnitState();
    pmat->getTechnique(0)->getPass(0)->getTextureUnitState(0)->setTextureName(texName);
    pmat->getTechnique(0)->getPass(0)->getTextureUnitState(0)->setTextureFiltering(FO_NONE, FO_NONE, FO_NONE);
    pmat->getTechnique(0)->getPass(0)->setDepthCheckEnabled(false);
    pmat->getTechnique(0)->getPass(0)->setDepthWriteEnabled(false);
    pmat->getTechnique(0)->getPass(0)->setLightingEnabled(false);

    TextureUnitState *texState = pmat->getTechnique(0)->getPass(0)->getTextureUnitState(0);

    // Example of background scrolling
    //pmat->getTechnique(0)->getPass(0)->getTextureUnitState(0)->setScrollAnimation(-0.25, 0.0);


    return texState;
}


void IntroState::mostrarFondo()
{
    String texName = "FondoIntro";
    Real texW = 1024;

    // Create background material
    Ogre::TexturePtr tex = CreateTextureFromImgWithoutStretch(texName, texW,
                                                              "pacman_traditional_800x600.jpg")->_getTexturePtr();;

    Real windowW = 800;
    Real windowH = 600;

    // Create background rectangle covering the whole screen
    _rect = new Rectangle2D(true);
    _rect->setCorners(-1.0, 1.0, texW / windowW * 2.0 + -1.0, 1.0 - texW / windowH * 2.0);
    _rect->setMaterial(texName);

    // Render the background before everything else
    _rect->setRenderQueueGroup(Ogre::RENDER_QUEUE_BACKGROUND);

    // Use infinite AAB to always stay visible
    Ogre::AxisAlignedBox aabInf;
    aabInf.setInfinite();
    _rect->setBoundingBox(aabInf);

    // Attach background to the scene
    Ogre::SceneNode *node = _sceneMgr->getRootSceneNode()->createChildSceneNode("Background");
    node->attachObject(_rect);

}

void IntroState::createScene()
{
    _sceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);
    _sceneMgr->setAmbientLight(Ogre::ColourValue(0.8, 0.8, 0.8));

    Ogre::Entity *pacmanEnt = _sceneMgr->createEntity("Pacman","pacman.mesh");
    Ogre::SceneNode *nodePacmanIntro = _sceneMgr->createSceneNode("nodePacmanIntro");
    nodePacmanIntro->attachObject(pacmanEnt);
    nodePacmanIntro->yaw(Ogre::Degree(180));
    nodePacmanIntro->setPosition(9,-6,0);
    _sceneMgr->getRootSceneNode()->addChild(nodePacmanIntro);

    Ogre::Entity *ghostEntClyde = _sceneMgr->createEntity("ghostClyde","ghost.mesh");
    Ogre::SceneNode *nodeGhostClyde = _sceneMgr->createSceneNode("nodeGhostClyde");
    nodeGhostClyde->attachObject(ghostEntClyde);
    nodeGhostClyde->scale(0.9,0.9,0.9);
    nodePacmanIntro->addChild(nodeGhostClyde);
    nodeGhostClyde->setPosition(nodeGhostClyde->getPosition().x - 2.5,nodeGhostClyde->getPosition().y,nodeGhostClyde->getPosition().z);

    Ogre::Entity *ghostEntInky = _sceneMgr->createEntity("ghostInky","ghost.mesh");
    ghostEntInky->setMaterialName("materialInkyCian");
    Ogre::SceneNode *nodeGhostInky = _sceneMgr->createSceneNode("nodeGhostInky");
    nodeGhostInky->attachObject(ghostEntInky);
    nodeGhostClyde->addChild(nodeGhostInky);
    nodeGhostInky->setPosition(nodeGhostInky->getPosition().x - 2.5,nodeGhostInky->getPosition().y,nodeGhostInky->getPosition().z);

    Ogre::Entity *ghostEntPinky = _sceneMgr->createEntity("ghostPinky","ghost.mesh");
    ghostEntPinky->setMaterialName("materialPinkyRosa");
    Ogre::SceneNode *nodeGhostPinky = _sceneMgr->createSceneNode("nodeGhostPinky");
    nodeGhostPinky->attachObject(ghostEntPinky);
    nodeGhostInky->addChild(nodeGhostPinky);
    nodeGhostPinky->setPosition(nodeGhostPinky->getPosition().x - 2.5,nodeGhostPinky->getPosition().y,nodeGhostPinky->getPosition().z);

    Ogre::Entity *ghostEntBlinky = _sceneMgr->createEntity("ghostBlinky","ghost.mesh");
    ghostEntBlinky->setMaterialName("materialBlinkyRojo");
    Ogre::SceneNode *nodeGhostBlinky = _sceneMgr->createSceneNode("nodeGhostBlinky");
    nodeGhostBlinky->attachObject(ghostEntBlinky);
    nodeGhostPinky->addChild(nodeGhostBlinky);
    nodeGhostBlinky->setPosition(nodeGhostBlinky->getPosition().x - 2.5,nodeGhostBlinky->getPosition().y,nodeGhostBlinky->getPosition().z);



    Ogre::Light* light = _sceneMgr->createLight("Light1");
    light->setType(Ogre::Light::LT_DIRECTIONAL);
    light->setDirection(Ogre::Vector3(-1,-1,0));
    nodePacmanIntro->attachObject(light);


    Ogre::Entity *cerezaEnt = _sceneMgr->createEntity("Cereza","Cereza.mesh");
    Ogre::SceneNode *nodeCerezaIntro = _sceneMgr->createSceneNode("nodeCerezaIntro");
    nodeCerezaIntro->attachObject(cerezaEnt);
    _sceneMgr->getRootSceneNode()->addChild(nodeCerezaIntro);

}