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

#ifndef RecordsState_H
#define RecordsState_H

#include <Ogre.h>
#include <OIS/OIS.h>
#include <string>
#include "GameState.h"
#include "MyGUI.h"
#include "MyGUI_OgrePlatform.h"


using namespace std;
using namespace Ogre;
class RecordsState : public Ogre::Singleton<RecordsState>, public GameState
{
 public:
  RecordsState () {}
  ~RecordsState();

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
  static RecordsState& getSingleton ();
  static RecordsState* getSingletonPtr ();


 protected:
  Ogre::Root* _root;
  Ogre::SceneManager* _sceneMgr;
  Ogre::Viewport* _viewport;
  Ogre::Camera* _camera;

  bool _exitGame;

  MyGUI::EditBox* high_score_txt;  
  MyGUI::TextBox* score_positions_txt;  
  MyGUI::TextBox* score_points_txt;  
  MyGUI::TextBox* score_names_txt;  
  MyGUI::VectorWidgetPtr layout;

  
private:
  private:
  Ogre::Rectangle2D* _rect; // Para mostrar un fondo
  void mostrarFondo();
  Ogre::TextureUnitState* CreateTextureFromImgWithoutStretch(const String& texName, Real texSize, const String& imgName);
  void createScene();
  void createMyGui();
  void loadRecords();
  
};

#endif
