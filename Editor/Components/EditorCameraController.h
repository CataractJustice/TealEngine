#include "GameNode/Component.h"

namespace TealEngine 
{
	class EditorCameraController : public Component 
	{
		private:
		glm::vec2 view;
		public:
		float zoomSpeed;
		float lookSensitivity;
		float strifeSpeed;
		float zoomFactor;

		void editorUpdate() override;
		EditorCameraController();
	};
}