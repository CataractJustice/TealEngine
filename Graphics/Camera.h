#pragma once
#include "GameNode/Component.h"
#include "Texture/Texture.h"
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
		static Camera* playPrimary;
		static Camera* editorPrimary;
	protected:
		bool isPlayPrimary;
		bool isEditPrimary;
	public:
		Texture renderTexture;
		Camera();
		~Camera();
		void setPerspectiveProjection(float fov, float aspect, float zNear, float zFar);
		void setOrthoProjection(float width, float height, float zNear, float zFar);
		void fixToWindowSize();
		void unfixFromWindowSize();
		void resize(int width, int height);
		glm::mat4 getPV();
		glm::mat4 getProjection();
		float getNear()const;
		float getFar()const;
		float getAspect()const;
		float getFOV()const;
		void onPropSet(const std::string& propName) override;
		inline void setAsEditorPrimary() { this->isEditPrimary = true; if(Camera::editorPrimary && Camera::editorPrimary != this) Camera::editorPrimary->isEditPrimary = false; Camera::editorPrimary = this;};
		inline void setAsPlayPrimary() { this->isPlayPrimary = true; if(Camera::playPrimary && Camera::playPrimary != this) Camera::editorPrimary->isPlayPrimary = false; Camera::playPrimary = this;};
		CameraProjectionType getProjectionType();
	};
}