#pragma once
#include "GameNode/GameNode3D.h"
#include "Texture/Texture.h"
#include "EventSystem/WindowEvents/WindowResizeEvent.h"
#include "Graphics/Graphics.h"
namespace TealEngine{

	enum CameraProjectionType
	{
		ORTHO_CAMERA_PROJECTION,
		PERSPECTIVE_CAMERA_PROJECTION
	};

	class Camera : public GameNode3D
	{
	private:
		glm::mat4 projection;
		float fov, aspect, zNear, zFar, width, height;
		bool fixedToWindowSize = true;
		EventListener* windowResizeListener;
		CameraProjectionType projectionType;
	public:
		Texture renderTexture;
		void setPerspectiveProjection(float fov, float aspect, float zNear, float zFar)
		{
			this->projection = glm::perspective(glm::radians(fov), aspect, zNear, zFar);
			this->fov = fov;
			this->aspect = aspect;
			this->zFar = zFar;
			this->zNear = zNear;
			this->width = 0.0;
			this->height = 0.0;
			
			this->projectionType = PERSPECTIVE_CAMERA_PROJECTION;
		}

		void setOrthoProjection(float width, float height, float zNear, float zFar)
		{
			this->projection = glm::ortho(-width/2.0f, width/2.0f, -height/2.0f, height/2.0f, zNear, zFar);
			this->fov = 0.0;
			this->aspect = width / height;
			this->zFar = zFar;
			this->zNear = zNear;
			this->width = width;
			this->height = height;
			this->projectionType = ORTHO_CAMERA_PROJECTION;
		}

		void fixToWindowSize()
		{
			if (!this->fixedToWindowSize) 
			{
				this->addEventListener(WINDOW_RESIZE, eventListenerBind(&Camera::resizeEvent, this));
				Graphics::window->WindowResize.subscribe(this->getEventListener(WINDOW_RESIZE));
				this->fixedToWindowSize = true;
			}
		}

		void resize(int width, int height) 
		{
			this->renderTexture.create(width, height);
			switch (projectionType)
			{
			case TealEngine::ORTHO_CAMERA_PROJECTION:
				setOrthoProjection(float(width), float(height), zNear, zFar);
				break;
			case TealEngine::PERSPECTIVE_CAMERA_PROJECTION:
				setPerspectiveProjection(fov, float(width)/float(height), zNear, zFar);
				break;
			default:
				break;
			}
		}

		void resizeEvent(Event* e) 
		{
			if (e->getType() == EventType::WINDOW_RESIZE) 
			{
				WindowResizeEvent* wre = (WindowResizeEvent*)e;
				resize(wre->width, wre->height);
			}
			else 
			{
				throw;
			}
		}

		glm::mat4 getPV()
		{
			Transform t = this->getWorldTransform();
			return projection * lookAt(t.getPosition(), t.getPosition() + t.forward(), t.up());
		}

		glm::mat4 getProjection()
		{
			return this->projection;
		}

		float getNear() 
		{
			return zNear;
		}
		float getFar() 
		{
			return zFar;
		}
		float getAspect() 
		{
			return aspect;
		}
		float getFOV() 
		{
			return fov;
		}
		CameraProjectionType getProjectionType() 
		{
			return this->projectionType;
		}
	};
}