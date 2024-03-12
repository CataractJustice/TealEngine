#include "GameNode3D.h"
#include "DefaultTags.h"
#include "Component.h"
#include "libs/imgui/imgui.h"
#include "ComponentFactory.h"
#include "libs/glm/gtc/type_ptr.hpp"
#include <fstream>
#include "Graphics/Renderer/Shapes/Line3D.h"
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
	}

	void GameNode3D::setParent(GameNode* parent)
	{
		GameNode::setParent(parent);
		if (parent) 
			hasGameNode3DParent = parent->hasTag(GAME_NODE_3D_TAG);
		else 
			hasGameNode3DParent = false;
	}

	Transform& GameNode3D::getWorldTransform()
	{
		if (hasGameNode3DParent)
		{
			Transform& parentTransform = ((GameNode3D*)parent)->getWorldTransform();
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

	void GameNode3D::render(ShaderProgram* shader, unsigned int stages) 
	{
		if(this == GameNode::getSelectedNode()) 
		{
			Transform& wTransform = this->getWorldTransform();
			glm::vec3 position = wTransform.getPosition();
			Core::shapesRenderer.pushShape(Line3D(position, position+wTransform.right()*1.5f, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), 0.02f));
			Core::shapesRenderer.pushShape(Line3D(position, position+wTransform.up()*1.5f, glm::vec4(0.0f, 1.0f, 0.0f, 1.0f), 0.02f));
			Core::shapesRenderer.pushShape(Line3D(position, position+wTransform.forward()*1.5f, glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), 0.02f));
		}
		GameNode::render(shader, stages);
	}
}