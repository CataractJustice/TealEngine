#include "GameNode/Component.h"

namespace TealEngine 
{
	class CameraViewport : public Component
	{
		private:
		std::string windowName;
		int textureId;
		public:
		CameraViewport(const std::string& windowName);
		void imGuiRender(const std::string& windowName) override;
	};
}