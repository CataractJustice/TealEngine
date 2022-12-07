#include "GameNode3D.h"
#include "DefaultTags.h"
#include "Component.h"
#include "libs/imgui/imgui.h"
using namespace glm;

namespace TealEngine
{
	GameNode3D::GameNode3D()
	{
		addTag(GAME_NODE_3D_TAG);
		hasGameNode3DParrent = false;
	}

	GameNode3D::~GameNode3D() 
	{
		if (!onDestroyHasBeenCalled)
			if (components.size())
				for (Component* comp : components)
					comp->onDestroy();
		onDestroyHasBeenCalled = true;
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

	void GameNode3D::displayNodeTree(bool windowBegin)
	{
		if (windowBegin)
		{
			ImGui::Begin("Node tree");
		}
		if (ImGui::TreeNode(((this->name.length() ? this->name : std::string("unnamed3D")) + " [" + std::to_string(id) + "]").c_str()))
		{
			ImGui::Checkbox("Active", &active);

			float pos[3] = { transform.getX(), transform.getY(), transform.getZ() };
			float oldPos[3] = { transform.getX(), transform.getY(), transform.getZ() };
			ImGui::InputFloat3("Position", pos);
			if(memcmp(pos, oldPos, sizeof(float[3])))
			{
				transform.setPosition(glm::vec3(pos[0], pos[1], pos[2]));
			}

			glm::vec3 oldScale = transform.getScale();
			float scale[3] = { oldScale.x, oldScale.y, oldScale.z };
			ImGui::InputFloat3("Scale", scale);
			if (scale[0] != oldScale.x || scale[1] != oldScale.y || scale[2] != oldScale.z)
			{
				transform.setScale(glm::vec3(scale[0], scale[1], scale[2]));
			}

			glm::quat oldRotation = transform.getRotation();
			float rotation[3] = { oldRotation.x, oldRotation.y, oldRotation.z };
			ImGui::InputFloat3("Roatation", rotation);
			if (rotation[0] != oldRotation.x || rotation[1] != oldRotation.y || rotation[2] != oldRotation.z)
			{
				transform.setRotation(glm::quat(glm::vec3(rotation[0], rotation[1], rotation[2])));
			}

			if (ImGui::TreeNode("Components"))
			{
				for (Component* comp : components)
				{
					comp->explorerDisplay();
				}
				ImGui::TreePop();
			}
			for (GameNode* node : this->childNodes)
			{
				node->displayNodeTree(false);
			}
			ImGui::TreePop();
		}
		if (windowBegin)
		{
			ImGui::End();
		}
	}
}