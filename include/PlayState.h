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

#ifndef PlayState_H
#define PlayState_H


#include <Ogre.h>
#include <OIS/OIS.h>
#include <string>
#include "GameState.h"
#include "MyGUI.h"
#include "MyGUI_OgrePlatform.h"
#include "pacman.h"
#include "OgreBulletDynamicsRigidBody.h"
#include "OgreBulletDynamicsWorld.h"
#include "Utils/OgreBulletCollisionsMeshToShapeConverter.h"
#include "graphml_boost.h"
#include "phantom.h"
#include "phantomFactory.h"
#include "Fruit.h"
#include "FruitFactory.h"
#include <vector>
#include "pill.h"
#include "OgreUtil.h"

#define PACMAN_START_NODE "pacmanstart"
#define PHANTOM_START_NODE "phantomstart"
#define BIGPILL_NODE "bigpill"
#define REGULAR_NODE "regular"








using namespace std;
using namespace Ogre;
using namespace OgreBulletDynamics;
using namespace OgreBulletCollisions;
class PlayState : public Ogre::Singleton<PlayState>, public GameState
{
 public:
  PlayState () {}
  ~PlayState();

  void enter ();
  void exit ();
  void pause ();
  void resume ();

  bool keyPressed (const OIS::KeyEvent &e);
  bool keyReleased (const OIS::KeyEvent &e);

  bool mouseMoved (const OIS::MouseEvent &e);
  bool mousePressed (const OIS::MouseEvent &e, OIS::MouseButtonID id);
  bool mouseReleased (const OIS::MouseEvent &e, OIS::MouseButtonID id);

  bool frameStarted (const Ogre::FrameEvent& evt);
  bool frameEnded (const Ogre::FrameEvent& evt);

  // Heredados de Ogre::Singleton.
  static PlayState& getSingleton ();
  static PlayState* getSingletonPtr ();
  void set_lives (int lives);
  int  get_lives ();

  void set_score (int score);
  int  get_score ();

  void  game_over ();
  void  win ();
  void handleCollision(btCollisionObject *body0, btCollisionObject *body1);
  inline graphml_boost::nodo_props getGraphNode(int idNode){return graphLevel->getGraphNode(idNode);};
  inline graphml_boost::ruta_t calculateRoute(int idOrg, int idDest){graphLevel->rutaMasCortaBFS(idOrg);return graphLevel->getRuta(idOrg,idDest);};
  inline Pacman* getPacman(){return _pacman;};
  int getFarNode();
  int getPacmanNode();
  void unloadLayout();
 protected:
  Ogre::Root* _root;
  Ogre::SceneManager* _sceneMgr;
  Ogre::Viewport* _viewport;
  Ogre::Camera* _camera;
  Pacman *_pacman;
  std::vector<Phantom>* _phantoms;
  std::vector<Pill>_pills;
  std::vector<Fruit>* _frutas;
  int _fruta_aleatoria; // id de una fruta del vector _frutas. Será la que aparezca.


  int _pacmanDir;
  DynamicsWorld* _world;
  DebugDrawer* _debugDrawer;
  graphml_boost *graphLevel;

  bool _exitGame;
  MyGUI::VectorWidgetPtr layout;
  MyGUI::EditBox* high_score_txt;
  MyGUI::EditBox* score_txt;
  MyGUI::EditBox* lives_txt;
  MyGUI::EditBox* message_txt;
  MyGUI::EditBox* user_name_txt;
  MyGUI::ImageBox* message_wall;
  int lives;
  int score;
  bool paused;
  long int timeAfraid;
private:

  void createScene();
  void createFloor();
  void createLevel();
  void createLight();
  void paintPills(bool bigpill);
  void createPacman();
  void createPhantoms();
  void createFruits();
  void setPhantomsAfraid(bool afraid);

};

#endif
