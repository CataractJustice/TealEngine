#include "GameNode/Component.h"

namespace TealEngine 
{
	class ProjectPropertiesWindow : public Component
	{
		public:
		void imGuiRender(const std::string& windowName) override;
	};
}