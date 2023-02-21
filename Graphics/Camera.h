#pragma once
#include "GameNode/Component.h"
#include "Texture/Texture.h"
#include "EventSystem/WindowEvents/WindowResizeEvent.h"
#include "Graphics/Graphics.h"
#include <functional>
namespace TealEngine{

	enum CameraProjectionType
	{
		ORTHO_CAMERA_PROJECTION,
		PERSPECTIVE_CAMERA_PROJECTION,
		CAMERA_PROJECTION_NUM
	};

	class Camera : public Component
	{
	private:
		glm::mat4 projection;
		float fov, aspect, zNear, zFar, width, height;
		bool fixedToWindowSize = false;
		CameraProjectionType projectionType;
		std::function<void()> viewFromHere;
		bool playPrimary;
		bool editPrimary;
	public:
		Texture renderTexture;
		Camera();
		~Camera();
		void setPerspectiveProjection(float fov, float aspect, float zNear, float zFar);
		void setOrthoProjection(float width, float height, float zNear, float zFar);
		void fixToWindowSize();
		void unfixFromWindowSize();
		void resize(int width, int height);
		void resizeEvent(Event* e);
		glm::mat4 getPV();
		glm::mat4 getProjection();
		float getNear()const;
		float getFar()const;
		float getAspect()const;
		float getFOV()const;
		void onPropSet(const std::string& propName) override;
		CameraProjectionType getProjectionType();
	};
}