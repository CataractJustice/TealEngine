#include "GameNode/GameNode3D.h"
#include "GameNodeTreeTests.h"
using namespace TealEngine;


	int GameNodeTreeTests::independentBasicTest() 
	{
		GameNode* root = new GameNode();
		GameNode* subNode = new GameNode();
		root->addChild(subNode);
		//Check parrent pointers
		assert(subNode->getParent() == root);
		assert(root->getParent() == nullptr);
		//check hierarchy depth values
		assert(root->getHierarchyDepth() == 0);
		assert(subNode->getHierarchyDepth() == 1);
		//detatch sub node from root
		root->removeChild(subNode);
		//sub node now must be a root node with no parrents
		assert(subNode->getHierarchyDepth() == 0);
		assert(subNode->getParent() == nullptr);
		//
		assert(GameNode::isNodeExist(root) == true);
		assert(GameNode::isNodeExist(subNode) == true);
		//re attach sub node
		root->addChild(subNode);
		//this should delete both nodes
		delete root;
		assert(GameNode::isNodeExist(root) == false);
		assert(GameNode::isNodeExist(subNode) == false);
		return 0;
	}
	int GameNodeTreeTests::independentSubTreeTest() 
	{
//                   root
//                     |
//                  subNode
//	  		   	   /	    \
//       subSubNode1         subSubNode2
//
		GameNode* root = new GameNode();
		GameNode* subNode = new GameNode();
		GameNode* subSubNode1 = new GameNode();
		GameNode* subSubNode2 = new GameNode();
		root->addChild(subNode);
		subNode->addChild(subSubNode1);
		subNode->addChild(subSubNode2);

		assert(root->getHierarchyDepth() == 0);
		assert(subNode->getHierarchyDepth() == 1);
		assert(subSubNode1->getHierarchyDepth() == 2);
		assert(subSubNode2->getHierarchyDepth() == 2);

		subNode->destroy();
		GameNode::cleanUp();

		assert(root->getChilds().size() == 0);
		assert(GameNode::isNodeExist(subNode) == false);
		assert(GameNode::isNodeExist(subSubNode1) == false);
		assert(GameNode::isNodeExist(subSubNode2) == false);

		delete root;
		return 0;
	}

void GameNodeTreeTests::run() 
{
	independentBasicTest();
	independentSubTreeTest();
}