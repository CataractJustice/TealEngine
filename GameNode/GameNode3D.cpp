#include "GameNode3D.h"
#include "DefaultTags.h"
using namespace glm;

namespace TealEngine
{
	GameNode3D::GameNode3D()
	{
		addTag(GAME_NODE_3D_TAG);
		hasGameNode3DParrent = false;
	}

	void GameNode3D::setParrent(GameNode* parrent)
	{
		GameNode::setParrent(parrent);
		if (parrent) 
			hasGameNode3DParrent = parrent->hasTag(GAME_NODE_3D_TAG);
		else 
			hasGameNode3DParrent = false;
	}

	Transform& GameNode3D::getWorldTransform()
	{
		if (hasGameNode3DParrent)
		{
			Transform& parrentTransform = ((GameNode3D*)parrent)->getWorldTransform();
			bool parrentTransformModified = parrentTransform.getLastModifyStamp() != parrentWorldTransformLastModifyStamp;
			bool transformModified = transformLastModifyStamp != transform.getLastModifyStamp();
			if (parrentTransformModified || transformModified)
			{
				parrentWorldTransformLastModifyStamp = parrentTransform.getLastModifyStamp();
				transformLastModifyStamp = transform.getLastModifyStamp();
				
				bool shouldUpdate = false;
				
				if (parrentTransformModified)
					if (!(lastParrentWorldTransform == parrentTransform)) shouldUpdate = true;
				
				if (!shouldUpdate && transformModified)
					if (!(lastTransfrom == transform)) shouldUpdate = true;
				
				if (shouldUpdate)
				{
					lastWorldTransform = transform * parrentTransform;
					lastTransfrom = transform;
					lastParrentWorldTransform = parrentTransform;
				}
			}
			return lastWorldTransform;
		}
		else
			return transform;
	}

	void GameNode3D::setWorldTransform(Transform transform)
	{
		if (dynamic_cast<GameNode3D*>(parrent))
			this->transform = transform / ((GameNode3D*)parrent)->getWorldTransform();
		else
			this->transform = transform;
	}

	void GameNode3D::addTransform(const Transform& transform)
	{
		this->transform *= (Transform&)transform;
	}

	Transform& GameNode3D::getRelativeTransform()
	{
		return transform;
	}

	void GameNode3D::setRelativeTransform(const Transform& transform)
	{
		this->transform = transform;
	}
}