/*This source code copyrighted by Lazy Foo' Productions (2004-2013)
and may not be redistributed without written permission.*/

//The headers
#include "sounds.h"
#include <iostream>

using namespace std;
sounds * sounds::singleton = NULL;

bool sounds::init()
{
    if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 ) return false;
    if( Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 4096 ) == -1 ) return false;
    SDL_WM_SetCaption( "Monitor Music", NULL );
    return true;
}

bool sounds::load_files()
{
    //Load the music
    //music[0] = Mix_LoadMUS( "beat.wav" );
    //if( music == NULL ) return false;

    //Load the sound effects
  //  effect[0] = Mix_LoadWAV( "scratch.wav" );
   // effect[1] = Mix_LoadWAV( "high.wav" );
   // effect[2] = Mix_LoadWAV( "medium.wav" );
   // effect[3] = Mix_LoadWAV( "low.wav" );

    //If there was a problem loading the sound effects
    //if( ( scratch == NULL ) || ( high == NULL ) || ( med == NULL ) || ( low == NULL ) ) return false;

    //If everything loaded fine
    return true;
}

void sounds::clean_up()
{
    it_map_effect it_effect;
    it_map_music it_music;
    for (it_effect=map_effect.begin();it_effect!=map_effect.end();it_effect++)Mix_FreeChunk(it_effect->second);
    for (it_music=map_music.begin();it_music!=map_music.end();it_music++)Mix_FreeMusic(it_music->second);

    Mix_CloseAudio();
    SDL_Quit();
};

int sounds::halt_effects()
{
  return Mix_HaltChannel(-1);
}
int sounds::play_effect(string id)
{
   Mix_Chunk * e = NULL;
   e= map_effect[id];
   if (e!=NULL)
       return Mix_PlayChannel( -1, e, 0 );
   else
       return -1; //TBD
}

int sounds::playing_music()
{
  return Mix_PlayingMusic();
}

int sounds::play_music(string id)
{
   cout << __FUNCTION__<< " " << id << endl;
   Mix_Music * e = NULL;
   e= map_music[id];
   cout << __FUNCTION__<<endl;

   if (e!=NULL)
   {
   cout << __FUNCTION__<<endl;
      return Mix_PlayMusic(e, -1 );
   }
   else return -1; //TBD
}

void sounds::pause_music()
{
   cout << __FUNCTION__ << endl;
   Mix_PauseMusic();
}

int sounds::paused_music()
{
   cout << __FUNCTION__ << endl;
  return Mix_PausedMusic();
}

void sounds::resume_music()
{
   cout << __FUNCTION__ << endl;
  return Mix_ResumeMusic();
}

int sounds::halt_music()
{
   cout << __FUNCTION__ << endl;
  return Mix_HaltMusic();
}

int sounds::halt_effect()
{
    return Mix_HaltChannel(-1);
}

sounds::sounds()
{
  init();
}

sounds::~sounds()
{
  clean_up();
}

bool sounds::load_xml (char * file)
{
    FILE *fp;
    mxml_node_t *tree;
    mxml_node_t *base_path;
    mxml_node_t *musics;
    mxml_node_t *node;

    fp = fopen("sounds.xml", "r");
    tree = mxmlLoadFile(NULL, fp, MXML_TEXT_CALLBACK);
    base_path = mxmlFindElement(tree, tree, "base_path", NULL,NULL,MXML_DESCEND);
    this->base_path= string(mxmlGetText(base_path,NULL));

    musics = mxmlFindElement(tree, tree, "effects", NULL,NULL,MXML_DESCEND);
    for (node = mxmlFindElement(musics, musics, "sound", NULL, NULL, MXML_DESCEND);
         node != NULL;
         node = mxmlFindElement(node,musics, "sound", NULL, NULL, MXML_DESCEND))
    {
      new_sound(node,EFFECT);
      new_sound(node,MUSIC);
    }

    mxmlDelete(tree);
    fclose(fp);

    return true;
}

void sounds::print()
{
    it_map_effect it_effect;
    it_map_music it_music;
    for (it_effect=map_effect.begin();it_effect!=map_effect.end();it_effect++)cout << "EFFECT ITEM " << it_effect->first<<endl;
    for (it_music=map_music.begin();it_music!=map_music.end();it_music++)cout << "MUSIC ITEM "<< it_music->first<<endl;
}

void sounds::new_sound(mxml_node_t * node, int type)
{
      mxml_node_t *id;
      mxml_node_t *filename;
      Mix_Music * music;
      Mix_Chunk * effect;
      string path;
      string id_str;

      id = mxmlFindElement(node,node, "id", NULL,NULL,MXML_DESCEND);
      filename =  mxmlFindElement(node, node ,"file", NULL,NULL,MXML_DESCEND);
      path=this->base_path+"/"+string(mxmlGetText(filename,NULL));
      id_str=string(mxmlGetText(id,NULL));

      if (type==MUSIC)
      {
        cout << "MUSIC" << path << " "<< id_str<< endl;
        music= Mix_LoadMUS( path.c_str() );
        map_music[id_str]=music;
      }
      else if (type==EFFECT)
      {
        cout << "EFFECT " << path << " " <<id_str<< endl;
        effect= Mix_LoadWAV( path.c_str() );
        map_effect[id_str]=effect;
      }
}

sounds * sounds::getInstance()
{
  if (singleton == NULL) 
  {
    singleton = new sounds();
  }
  return singleton;
}
