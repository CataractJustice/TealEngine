#include "GameNode3D.h"
#include "DefaultTags.h"
#include "Component.h"
#include "libs/imgui/imgui.h"
#include "ComponentFactory.h"
#include "libs/glm/gtc/type_ptr.hpp"
#include <fstream>
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

	GameNode3D* GameNode3D::nodeFromJson(const Json& json) 
	{
		GameNode3D* node = new GameNode3D();
		
		auto componentsIt = json.find("components");
		
		if(componentsIt != json.cend()) 
		{
			for(const Json& componentJson : *componentsIt)
			{
				Component* comp = Component::fromJson(componentJson);
				if(comp) 
				{
					node->attachComponent(comp);
				}
			}
		}
		
		if(json.find("nodes") != json.cend()) 
		{
			const Json& subNodes = json["nodes"];
			for(const Json& subNodeJson : subNodes) 
			{
				GameNode3D* subNode = nodeFromJson(subNodeJson);
				node->addChild(subNode);
			}
		}
		if(json.find("name") != json.cend()) 
		{
			node->rename(json["name"]);
		}

		if(json.find("id") != json.cend()) 
		{
			node->setId(json["id"]);
		}
		
		if(json.find("transform") != json.cend()) 
		{
			node->transformProp->set(json["transform"]);
		}
		return node;
	}

	Json GameNode3D::toJson()
	{
		Json json;
		json["name"] = this->name;
		json["id"] = this->id;
		json["transform"] = this->transformProp->get();
		json["filepath"] = this->loadedFromFilePath;
		std::vector<Json> componentsJson;
		for(Component* comp : components) 
		{
			componentsJson.push_back(comp->toJson());
		}
		json["components"] = componentsJson;

		std::vector<Json> nodesJson;
		for(GameNode* node : childNodes) 
		{
			nodesJson.push_back(((GameNode3D*)node)->toJson());
		}
		json["nodes"] = nodesJson;
		return json;
	}

	GameNode3D* GameNode3D::loadNodeFromJsonFile(const std::filesystem::path& path) 
	{
		std::ifstream file(path);
		GameNode3D* node = GameNode3D::nodeFromJson(Json::parse(file));
		node->setNodeFilePath(path);
		return node;
	}

	void GameNode3D::displayProps() 
	{
		this->transformProp->display("Transform");
		GameNode::displayProps();
	}
}