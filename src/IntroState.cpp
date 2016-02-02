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
    _animStateClyde = NULL;
    _animStateBlinky = NULL;
    _animStateCampana = NULL;
    _animStateCereza = NULL;
    _animStateFresa = NULL;
    _animStateGalaxian = NULL;
    _animStateLlave = NULL;
    _animStateManzana = NULL;
    _animStateNaranja = NULL;
    _animStateUvas = NULL;
    _animStateInky = NULL;
    _animStatePinky = NULL;
    _animOjosClyde = NULL;
    _animOjosInky = NULL;
    _animOjosPinky = NULL;
    _animOjosBlinky = NULL;
    _animBocaClyde = NULL;
    _animBocaInky = NULL;
    _animBocaPinky = NULL;
    _animBocaBlinky = NULL;

    _deIda = true;
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
    static Ogre::Vector3 sentido = Ogre::Vector3(-1,0,0);

    gestionaAnimaciones(_animStatePacman,deltaT,"Pacman","pakupaku");
    gestionaAnimaciones(_animStateClyde,deltaT,"ghostClyde","mareo2");
    gestionaAnimaciones(_animStateInky,deltaT,"ghostInky","mareo2");
    gestionaAnimaciones(_animStatePinky,deltaT,"ghostPinky","mareo2");
    gestionaAnimaciones(_animStateBlinky,deltaT,"ghostBlinky","mareo2");

    if (_deIda)
    {
        gestionaAnimaciones(_animOjosClyde,deltaT,"ojosClyde","piupiu");
        gestionaAnimaciones(_animOjosInky,deltaT,"ojosInky","piupiu");
        gestionaAnimaciones(_animOjosPinky,deltaT,"ojosPinky","piupiu");
        gestionaAnimaciones(_animOjosBlinky,deltaT,"ojosBlinky","piupiu");

        //if (_sceneMgr->getSceneNode("nodePacmanIntro")->convertLocalToWorldPosition(_sceneMgr->getSceneNode("nodePacmanIntro")->getPosition()).x < _posXminima)
        if (_sceneMgr->getSceneNode("nodePacmanIntro")->getPosition().x < _posXminima)
        {
            _deIda = false;
            sentido *= -1;
            _sceneMgr->getSceneNode("nodePacmanIntro")->yaw(Ogre::Degree(180),Ogre::Node::TS_PARENT);
            _sceneMgr->getEntity("ghostClyde")->setMaterialName("materialCagaosAzul");
            _sceneMgr->getEntity("ghostInky")->setMaterialName("materialCagaosAzul");
            _sceneMgr->getEntity("ghostPinky")->setMaterialName("materialCagaosAzul");
            _sceneMgr->getEntity("ghostBlinky")->setMaterialName("materialCagaosAzul");
            _sceneMgr->getEntity("ojosClyde")->setMaterialName("materialOjosGhost");
            _sceneMgr->getEntity("ojosInky")->setMaterialName("materialOjosGhost");
            _sceneMgr->getEntity("ojosPinky")->setMaterialName("materialOjosGhost");
            _sceneMgr->getEntity("ojosBlinky")->setMaterialName("materialOjosGhost");
            _animOjosClyde->setEnabled(false); _animOjosClyde = NULL;
            _animOjosInky->setEnabled(false); _animOjosInky = NULL;
            _animOjosPinky->setEnabled(false); _animOjosPinky = NULL;
            _animOjosBlinky->setEnabled(false); _animOjosBlinky = NULL;
            _sceneMgr->getSceneNode("nodeGhostClyde")->setPosition(_sceneMgr->getSceneNode("nodeGhostClyde")->getPosition() * Ogre::Vector3(-1,0,0));
            _sceneMgr->getSceneNode("nodeGhostInky")->setPosition(_sceneMgr->getSceneNode("nodeGhostInky")->getPosition() * Ogre::Vector3(-1,0,0));
            _sceneMgr->getSceneNode("nodeGhostPinky")->setPosition(_sceneMgr->getSceneNode("nodeGhostPinky")->getPosition() * Ogre::Vector3(-1,0,0));
            _sceneMgr->getSceneNode("nodeGhostBlinky")->setPosition(_sceneMgr->getSceneNode("nodeGhostBlinky")->getPosition() * Ogre::Vector3(-1,0,0));
            _sceneMgr->getSceneNode("nodeBocaClyde")->setVisible(true);
            _sceneMgr->getSceneNode("nodeBocaInky")->setVisible(true);
            _sceneMgr->getSceneNode("nodeBocaPinky")->setVisible(true);
            _sceneMgr->getSceneNode("nodeBocaBlinky")->setVisible(true);

        }
    }
    else
    {
        gestionaAnimaciones(_animOjosClyde,deltaT,"ojosClyde","cagaos");
        gestionaAnimaciones(_animOjosInky,deltaT,"ojosInky","cagaos");
        gestionaAnimaciones(_animOjosPinky,deltaT,"ojosPinky","cagaos");
        gestionaAnimaciones(_animOjosBlinky,deltaT,"ojosBlinky","cagaos");
        gestionaAnimaciones(_animBocaClyde,deltaT,"bocaClyde","panico");
        gestionaAnimaciones(_animBocaInky,deltaT,"bocaInky","panico");
        gestionaAnimaciones(_animBocaPinky,deltaT,"bocaPinky","panico");
        gestionaAnimaciones(_animBocaBlinky,deltaT,"bocaBlinky","panico");


        //if (_sceneMgr->getSceneNode("nodeGhostBlinky")->convertLocalToWorldPosition(_sceneMgr->getSceneNode("nodeGhostBlinky")->getPosition()).x > _posXmaxima)
        if (_sceneMgr->getSceneNode("nodePacmanIntro")->getPosition().x > _posXmaxima)
        {
            changeState(MenuState::getSingletonPtr());
            return true;
        }
    }


    cout << "sentido " << sentido << endl;
    Ogre::Vector3 v(_sceneMgr->getSceneNode("nodePacmanIntro")->getPosition());
    v += (sentido * deltaT * velocidad);
    cout << v << endl;
    _sceneMgr->getSceneNode("nodePacmanIntro")->setPosition(v);

    return true;
}

void IntroState::gestionaAnimaciones(Ogre::AnimationState *&anim, Ogre::Real deltaT, const String &nombreEnt, const String &nombreAnim)
{
    if (!anim )
    {
        anim = _sceneMgr->getEntity(nombreEnt)->getAnimationState(nombreAnim);
        anim->setEnabled(true);
        anim->setLoop(true);
        anim->setTimePosition(0.0);
    }
    else
        anim->addTime(deltaT);

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
                                                              "pacman_traditional_1024x768.jpg")->_getTexturePtr();;

    Real windowW = 1024;
    Real windowH = 768;

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
    Ogre::Entity *ojosEntClyde = _sceneMgr->createEntity("ojosClyde","ojosGhost.mesh");
    Ogre::SceneNode *nodeOjosClyde = _sceneMgr->createSceneNode("nodeOjosClyde");
    nodeOjosClyde->attachObject(ojosEntClyde);
    nodeGhostClyde->addChild(nodeOjosClyde);
    nodeOjosClyde->setInheritOrientation(false);

    Ogre::Entity *ghostEntInky = _sceneMgr->createEntity("ghostInky","ghost.mesh");
    ghostEntInky->setMaterialName("materialInkyCian");
    Ogre::SceneNode *nodeGhostInky = _sceneMgr->createSceneNode("nodeGhostInky");
    nodeGhostInky->attachObject(ghostEntInky);
    nodeGhostClyde->addChild(nodeGhostInky);
    nodeGhostInky->setPosition(nodeGhostInky->getPosition().x - 2.5,nodeGhostInky->getPosition().y,nodeGhostInky->getPosition().z);
    Ogre::Entity *ojosEntInky = _sceneMgr->createEntity("ojosInky","ojosGhost.mesh");
    Ogre::SceneNode *nodeOjosInky = _sceneMgr->createSceneNode("nodeOjosInky");
    nodeOjosInky->attachObject(ojosEntInky);
    nodeGhostInky->addChild(nodeOjosInky);
    nodeOjosInky->setInheritOrientation(false);


    Ogre::Entity *ghostEntPinky = _sceneMgr->createEntity("ghostPinky","ghost.mesh");
    ghostEntPinky->setMaterialName("materialPinkyRosa");
    Ogre::SceneNode *nodeGhostPinky = _sceneMgr->createSceneNode("nodeGhostPinky");
    nodeGhostPinky->attachObject(ghostEntPinky);
    nodeGhostInky->addChild(nodeGhostPinky);
    nodeGhostPinky->setPosition(nodeGhostPinky->getPosition().x - 2.5,nodeGhostPinky->getPosition().y,nodeGhostPinky->getPosition().z);
    Ogre::Entity *ojosEntPinky = _sceneMgr->createEntity("ojosPinky","ojosGhost.mesh");
    Ogre::SceneNode *nodeOjosPinky = _sceneMgr->createSceneNode("nodeOjosPinky");
    nodeOjosPinky->attachObject(ojosEntPinky);
    nodeGhostPinky->addChild(nodeOjosPinky);
    nodeOjosPinky->setInheritOrientation(false);


    Ogre::Entity *ghostEntBlinky = _sceneMgr->createEntity("ghostBlinky","ghost.mesh");
    ghostEntBlinky->setMaterialName("materialBlinkyRojo");
    Ogre::SceneNode *nodeGhostBlinky = _sceneMgr->createSceneNode("nodeGhostBlinky");
    nodeGhostBlinky->attachObject(ghostEntBlinky);
    nodeGhostPinky->addChild(nodeGhostBlinky);
    nodeGhostBlinky->setPosition(nodeGhostBlinky->getPosition().x - 2.5,nodeGhostBlinky->getPosition().y,nodeGhostBlinky->getPosition().z);
    Ogre::Entity *ojosEntBlinky = _sceneMgr->createEntity("ojosBlinky","ojosGhost.mesh");
    Ogre::SceneNode *nodeOjosBlinky = _sceneMgr->createSceneNode("nodeOjosBlinky");
    nodeOjosBlinky->attachObject(ojosEntBlinky);
    nodeGhostBlinky->addChild(nodeOjosBlinky);
    nodeOjosBlinky->setInheritOrientation(false);

    //_posXmaxima = _sceneMgr->getSceneNode("nodePacmanIntro")->convertWorldToLocalPosition(_sceneMgr->getSceneNode("nodePacmanIntro")->getPosition()).x;
    _posXmaxima = _sceneMgr->getSceneNode("nodePacmanIntro")->_getDerivedPosition().x + _sceneMgr->getSceneNode("nodeGhostBlinky")->_getDerivedPosition().x / 1.80;
    _posXminima = _posXmaxima * -1;


    Ogre::SceneNode *nodeBocaClyde = _sceneMgr->createSceneNode("nodeBocaClyde");
    Ogre::SceneNode *nodeBocaInky = _sceneMgr->createSceneNode("nodeBocaInky");
    Ogre::SceneNode *nodeBocaPinky = _sceneMgr->createSceneNode("nodeBocaPinky");
    Ogre::SceneNode *nodeBocaBlinky = _sceneMgr->createSceneNode("nodeBocaBlinky");
    Ogre::Entity *entBocaClyde = _sceneMgr->createEntity("bocaClyde","bocaGhost.mesh");
    Ogre::Entity *entBocaInky = _sceneMgr->createEntity("bocaInky","bocaGhost.mesh");
    Ogre::Entity *entBocaPinky = _sceneMgr->createEntity("bocaPinky","bocaGhost.mesh");
    Ogre::Entity *entBocaBlinky = _sceneMgr->createEntity("bocaBlinky","bocaGhost.mesh");
    nodeBocaClyde->attachObject(entBocaClyde);
    nodeBocaInky->attachObject(entBocaInky);
    nodeBocaPinky->attachObject(entBocaPinky);
    nodeBocaBlinky->attachObject(entBocaBlinky);
    nodeBocaClyde->setVisible(false);
    nodeBocaInky->setVisible(false);
    nodeBocaPinky->setVisible(false);
    nodeBocaBlinky->setVisible(false);
    nodeGhostClyde->addChild(nodeBocaClyde);
    nodeGhostInky->addChild(nodeBocaInky);
    nodeGhostPinky->addChild(nodeBocaPinky);
    nodeGhostBlinky->addChild(nodeBocaBlinky);

    cout << "maxima " << _posXmaxima;
    cout << "minima " << _posXminima;



    Ogre::Light* light = _sceneMgr->createLight("Light1");
    light->setType(Ogre::Light::LT_DIRECTIONAL);
    light->setDirection(Ogre::Vector3(-1,-1,0));
    nodePacmanIntro->attachObject(light);


//    Ogre::Entity *cerezaEnt = _sceneMgr->createEntity("Cereza","Cereza.mesh");
//    Ogre::SceneNode *nodeCerezaIntro = _sceneMgr->createSceneNode("nodeCerezaIntro");
//    nodeCerezaIntro->attachObject(cerezaEnt);
//    _sceneMgr->getRootSceneNode()->addChild(nodeCerezaIntro);




}