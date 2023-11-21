#include "TransformProp.h"
#include "libs/glm/gtc/type_ptr.hpp"

namespace TealEngine 
{
	TransformProp::TransformProp(Transform* transformPtr)
	{
		this->propptr = transformPtr;
	}
	
	Json TransformProp::get() 
	{
		Transform& transform = this->valueref<Transform>();
		glm::vec3 pos = transform.getPosition();
		glm::vec3 scale = transform.getScale();
		glm::quat rotation = transform.getRotation();
		Json json;
		json["P"] = {pos.x, pos.y, pos.z};
		json["S"] = {scale.x, scale.y, scale.z};
		json["R"] = {rotation.x, rotation.y, rotation.z, rotation.w};
 		return json;
	}
	void TransformProp::set(const Json& json) 
	{
		Transform& transform = this->valueref<Transform>();
		auto posIt = json.find("P");
		auto sclIt = json.find("S");
		auto rotIt = json.find("R");
		if(posIt != json.cend()) 
		{
			transform.setPosition({posIt->operator[](0).get<float>(), posIt->operator[](1).get<float>(), posIt->operator[](2).get<float>()});
		}

		if(sclIt != json.cend()) 
		{
			transform.setScale({sclIt->operator[](0).get<float>(), sclIt->operator[](1).get<float>(), sclIt->operator[](2).get<float>()});
		}

		if(rotIt != json.cend()) 
		{
			transform.setRotation({rotIt->operator[](3).get<float>(), rotIt->operator[](0).get<float>(), rotIt->operator[](1).get<float>(), rotIt->operator[](2).get<float>()});
		}
	}
	bool TransformProp::display(const char* label) 
	{
		Transform& transform = this->valueref<Transform>();

		ImGui::TextUnformatted(label);
		int edited = 0;
		glm::vec3 pos = transform.getPosition();
		glm::vec3 scale = transform.getScale();
		glm::vec3 rotation = transform.getXYZRotation();
		if(ImGui::DragFloat3((std::string("Position#") + label).c_str(), glm::value_ptr(pos), 0.01f)) 
		{
			edited++;
			transform.setPosition(pos);
		}

		if(ImGui::DragFloat3((std::string("Scale#") + label).c_str(), glm::value_ptr(scale), 0.01f)) 
		{
			edited++;
			transform.setScale(scale);
		}

		if(ImGui::DragFloat3((std::string("Rotation#") + label).c_str(), glm::value_ptr(rotation), 0.005f)) 
		{
			edited++;
			transform.setRotation(rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
			transform.rotate(rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
			transform.rotate(rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
		}

		return edited;
	}
}