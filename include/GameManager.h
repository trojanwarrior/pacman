/*********************************************************************
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

#ifndef GameManager_H
#define GameManager_H

#include <stack>
#include <Ogre.h>
#include <OgreSingleton.h>
#include <OIS/OIS.h>

#include "InputManager_.h"



class GameState; // <----- Declaración en avanzada de la clase GameState (Forward declaration)
//La declaración avanzada de la clase GameState es para evitar referencias circulares entre clases y así el compilador
//pueda compilar sin dar error. Es decir, la clase GameManager hace uso de la clase Gamestate y la clase GameState hace
//uso de la clase GameManager provocando una referencia circular que hace que el compilador falle al intentar compilar.
//Para evitarlo se hace una declaración avanzada de la clase GameState en GameManager. Así el compilador es capaz
//de compilar la clase GameManager sin conocer la definición de la clase GameState. Vamos, un lío de cojones :D


class GameManager : public Ogre::FrameListener, public Ogre::Singleton<GameManager>, public OIS::KeyListener, public OIS::MouseListener
{
 public:
  GameManager ();
  ~GameManager (); // Limpieza de todos los estados.

  // Para el estado inicial.
  void start (GameState* state);
  
  // Funcionalidad para transiciones de estados.
  void changeState (GameState* state);
  void pushState (GameState* state);
  void popState ();
  
  // Heredados de Ogre::Singleton.
  static GameManager& getSingleton ();
  static GameManager* getSingletonPtr ();

 protected:
  Ogre::Root* _root;
  Ogre::SceneManager* _sceneManager;
  Ogre::RenderWindow* _renderWindow;

  // Funciones de configuración.
  void loadResources ();
  bool configure ();
  
  // Heredados de FrameListener.
  bool frameStarted (const Ogre::FrameEvent& evt);
  bool frameEnded (const Ogre::FrameEvent& evt);

 private:
  // Funciones para delegar eventos de teclado
  // y ratón en el estado actual.
  bool keyPressed (const OIS::KeyEvent &e);
  bool keyReleased (const OIS::KeyEvent &e);

  bool mouseMoved (const OIS::MouseEvent &e);
  bool mousePressed (const OIS::MouseEvent &e, OIS::MouseButtonID id);
  bool mouseReleased (const OIS::MouseEvent &e, OIS::MouseButtonID id);
  
  // Gestor de eventos de entrada.
  InputManager_ *_inputMgr;
  // Estados del juego.
  std::stack<GameState*> _states;
};

#endif
