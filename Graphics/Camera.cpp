#include "Camera.h"
#include "GameNode/ComponentFactory.h"
#include "libs/glm/gtc/matrix_transform.hpp"
#include "GameNode/GameNode3D.h"
#include "Core.h"

namespace TealEngine 
{
	Camera::Camera() 
	{
		this->playPrimary = false;
		this->editPrimary = false;
		this->fov = 90.0f;
		this->zNear = 0.01f;
		this->zFar = 1000.0f;
		this->width = 640.0f;
		this->height = 480.0f;
		this->aspect = width / height;
		setPerspectiveProjection(fov, aspect, zNear, zFar);
		addProp(new FloatProp(&fov), "FOV");
		addProp(new FloatProp(&zNear), "Near plane");
		addProp(new FloatProp(&zFar), "Far plane");
		addProp(new FloatProp(&width), "Ortho width");
		addProp(new FloatProp(&height), "Ortho height");
		static const char* projectionNames[] = 
		{
			"Orthogonal", "Perspective"
		};
		addProp(new EnumProp((int*)&projectionType, projectionNames, 2), "Projection type");
		addProp(new BoolProp(&playPrimary), "Editor primary camera");
		addProp(new BoolProp(&editPrimary), "Play primary camera");
		this->viewFromHere = std::function<void()>([this] () 
		{
			Core::renderer.setCamera(this);
		});

		addProp(new VoidFunctionProp(&viewFromHere), "View from here");

		width = 640.0f;
		height = 480.0f;
		this->resize(width, height);
	}

	Camera::~Camera() 
	{
		if(Core::renderer.getActiveCamera() == this) Core::renderer.setCamera(nullptr);
	}

	void Camera::setPerspectiveProjection(float fov, float aspect, float zNear, float zFar)
	{
		this->projection = glm::perspective(glm::radians(fov), aspect, zNear, zFar);
		this->fov = fov;
		this->aspect = aspect;
		this->zFar = zFar;
		this->zNear = zNear;
		
		this->projectionType = PERSPECTIVE_CAMERA_PROJECTION;
	}

	void Camera::setOrthoProjection(float width, float height, float zNear, float zFar)
	{
		this->projection = glm::ortho(-width/2.0f, width/2.0f, -height/2.0f, height/2.0f, zNear, zFar);
		this->aspect = width / height;
		this->zFar = zFar;
		this->zNear = zNear;
		this->width = width;
		this->height = height;
		this->projectionType = ORTHO_CAMERA_PROJECTION;
	}

	void Camera::fixToWindowSize()
	{
		if (!this->fixedToWindowSize) 
		{
			this->getParent()->addEventListener(WINDOW_RESIZE, eventListenerBind(&Camera::resizeEvent, this));
			Graphics::window->WindowResize.subscribe(this->getParent()->getEventListener(WINDOW_RESIZE));
			this->fixedToWindowSize = true;
		}
	}
	
	void Camera::unfixFromWindowSize()
	{
		if (this->fixedToWindowSize) 
		{
			Graphics::window->WindowResize.unsubscribe(this->getParent()->getEventListener(WINDOW_RESIZE));
			this->fixedToWindowSize = false;
		}
	}

	void Camera::resize(int width, int height) 
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

	void Camera::resizeEvent(Event* e) 
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
	
	glm::mat4 Camera::getPV() 
	{
		Transform t = this->getParentOfType<GameNode3D>()->getWorldTransform();
		return projection * lookAt(t.getPosition(), t.getPosition() + t.forward(), t.up());
	}

	glm::mat4 Camera::getProjection()
	{
		return this->projection;
	}

	float Camera::getNear() const
	{
		return zNear;
	}
	
	float Camera::getFar() const
	{
		return zFar;
	}

	float Camera::getAspect() const
	{
		return aspect;
	}
	
	float Camera::getFOV() const
	{
		return fov;
	}
	
	CameraProjectionType Camera::getProjectionType() 
	{
		return this->projectionType;
	}

	void Camera::onPropSet(const std::string& propName) 
	{
		resize(width, height);
	}
	
	EXPORT_COMPONENT(Camera);
}