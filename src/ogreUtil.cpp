#include "OgreUtil.h"

using namespace Ogre;

void OgreUtil::destroyAllAttachedMovableObjects( SceneNode* node )
{
   if(!node) return;

   // Destroy all the attached objects
   SceneNode::ObjectIterator itObject = node->getAttachedObjectIterator();

   while ( itObject.hasMoreElements() )
     try {


       node->getCreator()->destroyMovableObject(itObject.getNext());
     }
     catch(...) {
       //Empty catch, to skip OgreBullet trying to delete
     }

   // Recurse to child SceneNodes
   SceneNode::ChildNodeIterator itChild = node->getChildIterator();

   while ( itChild.hasMoreElements() )
   {
      SceneNode* pChildNode = static_cast<SceneNode*>(itChild.getNext());
      destroyAllAttachedMovableObjects( pChildNode );
   }
}

void OgreUtil::destroySceneNode( Ogre::SceneNode* node )
{
   if(!node) return;
   destroyAllAttachedMovableObjects(node);
   node->removeAndDestroyAllChildren();
   node->getCreator()->destroySceneNode(node);
}
