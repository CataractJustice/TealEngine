#include "Camera.h"
#include "GameNode/ComponentFactory.h"
#include "libs/glm/gtc/matrix_transform.hpp"
#include "GameNode/GameNode3D.h"
#include "Core.h"

namespace TealEngine 
{
	Camera::Camera() 
	{
		this->isPlayPrimary = false;
		this->isEditPrimary = false;
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
		addProp(new BoolProp(&isEditPrimary), "Editor primary");
		addProp(new BoolProp(&isPlayPrimary), "Game primary");
		static const char* projectionNames[] = 
		{
			"Orthogonal", "Perspective"
		};
		addProp(new EnumProp((int*)&projectionType, projectionNames, 2), "Projection type");
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
		}
	}
	
	void Camera::unfixFromWindowSize()
	{
		if (this->fixedToWindowSize) 
		{
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
		if(
			isEditPrimary && Core::getEngineState() == Core::EngineState::GAME_STOPPED ||
			isPlayPrimary && (Core::getEngineState() == Core::EngineState::GAME_PLAYING || Core::getEngineState() == Core::EngineState::GAME_PAUSED)
		) 
		{
			Core::renderer.setCamera(this);
		}

		if(isEditPrimary) this->setAsEditorPrimary();
		if(isPlayPrimary) this->setAsPlayPrimary();
	}
	Camera* Camera::editorPrimary = nullptr;
	Camera* Camera::playPrimary = nullptr;
	EXPORT_COMPONENT(Camera);
}