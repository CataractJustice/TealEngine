#include "GameNode/Component.h"

namespace TealEngine 
{
	class TreeDisplay : public Component
	{
		private:
		std::string windowName;
		public:
		TreeDisplay(const std::string& windowName);
		void imGuiRender(const std::string& windowName) override;
	};
}