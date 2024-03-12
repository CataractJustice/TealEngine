#include "GameNode/ComponentProp.h"
#include "GameNode/Component.h"
namespace TealEngine
{
	template<class T>
	class ComponentRefProp : public IProp 
	{
		private:
		
		public:
		ComponentRefProp(T** componentPtr) 
		{
			this->propptr = componentPtr;
		}
		void set(const Json& json) override
		{
			const auto& it = json.find("path");
			if(it == json.cend()) 
			{
				this->valueref<Component*>() = nullptr;
				return;
			}
			std::vector<int>path = it->get<std::vector<int>>();
			GameNode* nodeIt = this->ownerComp->getParent();
			for(auto i : path) 
			{
				if(i == -1) 
				{
					nodeIt = nodeIt->getParent();
				}
				else 
				{
					nodeIt = nodeIt->getChildByIndex(i);
				}
				if(nodeIt == nullptr) break;
			}
			if(nodeIt == nullptr) 
			{
				this->valueref<Component*>() = nullptr;
				return;
			}
			this->valueref<Component*>() = nodeIt->getComponentByIndex(json["index"]);
		}
		Json get() override
		{
			Json json;
			if(this->valueref<Component*>() == nullptr) 
			{
				return json;
			}
			json["path"] = this->ownerComp->getParent()->getRelativeIndexPathTo(this->valueref<Component*>()->getParent());
			json["index"] = this->valueref<Component*>()->getIndex();
			return json;
		}
		bool display(const char* label) override
		{
			T* component = nullptr;
			if(propptr) 
			{
				if(Component::isValidComponent(valueref<T*>())) 
				{
					component = valueref<T*>();
				}
			}

			if(component) 
			{
				ImGui::Text("%s - %s:%d (%s)", label, component->getName().c_str(), component->getIndex(), component->getParent()->getName().c_str());	
				if(ImGui::IsMouseDoubleClicked(0) && ImGui::IsItemClicked(0)) 
				{
					GameNode::setSelectedNode(component->getParent());
				}
			} 
			else 
			{
				ImGui::Text("%s - %s", label, "<Drop component here>");
			}
			if(ImGui::BeginDragDropTarget()) 
			{
				if(const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ComponentInstance")) 
				{
					this->valueref<Component*>() = (dynamic_cast<T*>(*(Component**)payload->Data));
					return true;
				}
				ImGui::EndDragDropTarget();
			}

			return false;
		}
	};
} // namespace TealEngine
