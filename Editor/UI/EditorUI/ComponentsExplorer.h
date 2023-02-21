#include "GameNode/Component.h"

namespace TealEngine 
{
	class ComponentsExplorer : public Component
	{
		public:
		void imGuiRender(const std::string& windowName) override;
	};
}