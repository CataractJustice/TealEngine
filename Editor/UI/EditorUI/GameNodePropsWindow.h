#include "GameNode/Component.h"

namespace TealEngine 
{
	class GameNodePropsWindow : public Component
	{
		public:
		void imGuiRender(const std::string& windowName) override;
	};
}