#include "EditorCameraController.h"
#include "System/Input.h"
#include "GameNode/GameNode3D.h"
#include "GameNode/ComponentFactory.h"

namespace TealEngine 
{
	void EditorCameraController::editorUpdate() 
	{
		Transform& transform = ((GameNode3D*)this->getParent())->getRelativeTransform();
		
		transform.translate(transform.forward() * this->zoomSpeed * zoomFactor * (float)Input::Mouse::getDeltaScrollPos() * 0.03f);

		zoomFactor *= 1.0f - (float)Input::Mouse::getDeltaScrollPos() / 15.0f;
		zoomFactor = zoomFactor < 1.0f ? 1.0f : zoomFactor;

		glm::vec2 deltaMouse = Input::Mouse::getDeltaMousePos();

		if(Input::Mouse::isButtonPressed(GLFW_MOUSE_BUTTON_MIDDLE))
			transform.translate((transform.right() * deltaMouse.x * strifeSpeed * zoomFactor + transform.up() * deltaMouse.y * strifeSpeed * zoomFactor) * 0.002f);

		if(Input::Mouse::isButtonPressed(GLFW_MOUSE_BUTTON_RIGHT))
			this->view += deltaMouse * this->lookSensitivity * glm::vec2(1.0f, -1.0f) * 0.001f;
		transform.setRotation(view.x, glm::vec3(0.0f, 1.0f, 0.0f));
		transform.rotate(view.y, glm::vec3(1.0f, 0.0f, 0.0f));
		
	}

	EditorCameraController::EditorCameraController() 
	{
		addProp(new FloatProp(&lookSensitivity), "Look sensitivity");
		addProp(new FloatProp(&zoomSpeed), "Zoom speed");
		addProp(new FloatProp(&strifeSpeed), "Strife speed");
		lookSensitivity = 1.0f;
		zoomSpeed = 1.0f;
		zoomFactor = 1.0f;
		strifeSpeed = 1.0f;
		this->view = glm::vec2(0.0f, 0.0f);
	}
}