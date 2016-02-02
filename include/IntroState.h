/*********************************************************************n
 * Módulo 1. Curso de Experto en Desarrollo de Videojuegos
 * Autor: David Vallejo Fernández    David.Vallejo@uclm.es
 *
 * Código modificado a partir de Managing Game States with OGRE
 * http://www.ogre3d.org/tikiwiki/Managing+Game+States+with+OGRE
 * Inspirado en Managing Game States in C++
 * http://gamedevgeek.com/tutorials/managing-game-states-in-c/
 *
 * You can redistribute and/or modify this file under the terms of the
 * GNU General Public License ad published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * and later version. See <http://www.gnu.org/licenses/>.
 *
 * This file is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.  
 *********************************************************************/

#ifndef IntroState_H
#define IntroState_H

#include <Ogre.h>
#include <OIS/OIS.h>
#include "GameState.h"
#include "sounds.h"
#include "MyGUI.h"
#include "MyGUI_OgrePlatform.h"

using namespace std;
using namespace Ogre;

class IntroState : public Ogre::Singleton<IntroState>, public GameState
{
public:
    IntroState()
    { }

    ~IntroState();

    void enter();

    void exit();

    void pause();

    void resume();

    bool keyPressed(const OIS::KeyEvent &e);

    bool keyReleased(const OIS::KeyEvent &e);

    bool mouseMoved(const OIS::MouseEvent &e);

    bool mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id);

    bool mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id);

    bool frameStarted(const Ogre::FrameEvent &evt);

    bool frameEnded(const Ogre::FrameEvent &evt);

    // Heredados de Ogre::Singleton.
    static IntroState &getSingleton();

    static IntroState *getSingletonPtr();

protected:
    Ogre::Root *_root;
    Ogre::SceneManager *_sceneMgr;
    Ogre::Viewport *_viewport;
    Ogre::Camera *_camera;

    bool _exitGame;

private:
    Ogre::Rectangle2D *_rect; // Para mostrar un fondo
    Ogre::AnimationState *_animStatePacman; // Para animar boca pacman
    Ogre::AnimationState *_animStateCampana;
    Ogre::AnimationState *_animStateCereza;
    Ogre::AnimationState *_animStateFresa;
    Ogre::AnimationState *_animStateGalaxian;
    Ogre::AnimationState *_animStateLlave;
    Ogre::AnimationState *_animStateManzana;
    Ogre::AnimationState *_animStateNaranja;
    Ogre::AnimationState *_animStateUvas;
    Ogre::AnimationState *_animStateClyde;
    Ogre::AnimationState *_animStateInky;
    Ogre::AnimationState *_animStatePinky;
    Ogre::AnimationState *_animStateBlinky;
    Ogre::AnimationState *_animOjosClyde;
    Ogre::AnimationState *_animOjosInky;
    Ogre::AnimationState *_animOjosPinky;
    Ogre::AnimationState *_animOjosBlinky;

    Ogre::Real _posXminima; // solo para saber cuando terminar la intro
    Ogre::Real _posXmaxima;
    bool _deIda;




    void mostrarFondo();
    void createScene();
    void gestionaAnimaciones(Ogre::AnimationState *&anim, Ogre::Real deltaT, const String &nombreEnt, const String &nombreAnim);

    TextureUnitState *CreateTextureFromImgWithoutStretch(const String &texName, Real texSize, const String &imgName);

};

#endif
