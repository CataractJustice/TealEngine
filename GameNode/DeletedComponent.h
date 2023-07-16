#include "Component.h"
#include "ComponentFactory.h"
#include <list>
namespace TealEngine 
{
	class DeletedComponent : public Component 
	{
		private:
		std::list<Json> propsSetList;
		public:
		virtual bool setProp(const Json& json) override;
		void setDeletedComponentName(const std::string& name);
		void onAttach() override;
	};
}