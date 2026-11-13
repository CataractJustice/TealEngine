#include "GameNode3D.h"
#include "DefaultTags.h"
#include "Component.h"
#include "libs/imgui/imgui.h"
#include "ComponentFactory.h"
#include "libs/glm/gtc/type_ptr.hpp"
#include <fstream>
#include "Core.h"
using namespace glm;

namespace TealEngine
{
	GameNode3D::GameNode3D()
	{
		addTag(GAME_NODE_3D_TAG);
		hasGameNode3DParent = false;
		this->transformProp = new TransformProp(&this->getRelativeTransform());
	}

	GameNode3D::~GameNode3D() 
	{
		if (!onDestroyHasBeenCalled)
			if (components.size())
				for (Component* comp : components)
					comp->onDestroy();
		onDestroyHasBeenCalled = true;
		delete transformProp;
	}

	void GameNode3D::setParent(GameNode* parent)
	{
		GameNode::setParent(parent);
		if (parent) 
			hasGameNode3DParent = parent->hasTag(GAME_NODE_3D_TAG);
		else 
			hasGameNode3DParent = false;
	}

	const Transform& GameNode3D::getWorldTransform() const
	{
		if (hasGameNode3DParent)
		{
			const Transform& parentTransform = ((GameNode3D*)parent)->getWorldTransform();
			bool parentTransformModified = parentTransform.getLastModifyStamp() != parentWorldTransformLastModifyStamp;
			bool transformModified = transformLastModifyStamp != transform.getLastModifyStamp();
			if (parentTransformModified || transformModified)
			{
				parentWorldTransformLastModifyStamp = parentTransform.getLastModifyStamp();
				transformLastModifyStamp = transform.getLastModifyStamp();
				
				bool shouldUpdate = false;
				
				if (parentTransformModified)
					if (!(lastParentWorldTransform == parentTransform)) shouldUpdate = true;
				
				if (!shouldUpdate && transformModified)
					if (!(lastTransfrom == transform)) shouldUpdate = true;
				
				if (shouldUpdate)
				{
					lastWorldTransform = transform * parentTransform;
					lastTransfrom = transform;
					lastParentWorldTransform = parentTransform;
				}
			}
			return lastWorldTransform;
		}
		else
			return transform;
	}

	void GameNode3D::setWorldTransform(Transform transform)
	{
		if (dynamic_cast<GameNode3D*>(parent))
			this->transform = transform / ((GameNode3D*)parent)->getWorldTransform();
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

	Json GameNode3D::toJson()
	{
		Json json = GameNode::toJson();
		json["type"] = "3d";
		json["transform"] = this->transformProp->get();
		return json;
	}

	void GameNode3D::displayProps() 
	{
		this->transformProp->display("Transform");
		GameNode::displayProps();
	}
}