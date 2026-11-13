#include "Camera.h"
#include "GameNode/ComponentFactory.h"
#include "libs/glm/gtc/matrix_transform.hpp"
#include "GameNode/GameNode3D.h"
#include "Core.h"
#include "Graphics/Shapes/Line3D.h"

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
		addProp(new BoolProp(&fixToWindowSize), "Fix to window size");
		static const char* projectionNames[] = 
		{
			"Orthogonal", "Perspective"
		};
		addProp(new EnumProp((int*)&projectionType, projectionNames, 2), "Projection type");
		this->viewFromHere = std::function<void()>([this] () 
		{

		});

		addProp(new VoidFunctionProp(&viewFromHere), "View from here");

		width = 640.0f;
		height = 480.0f;
		this->resize(width, height);
	}

	Camera::~Camera() 
	{
		if(this == playPrimary) playPrimary = nullptr;
		if(this == editorPrimary) editorPrimary = nullptr;
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

	void Camera::resize(int width, int height) 
	{
		this->width = width;
		this->height = height;
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
	
	glm::mat4 Camera::getPV() const
	{
		Transform t = this->getParentOfTypeConst<GameNode3D>()->getWorldTransform();
		return projection * lookAt(t.getPosition(), t.getPosition() + t.forward(), t.up());
	}

	glm::mat4 Camera::getProjection()const
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

	void Camera::onPropRefresh() 
	{
		resize(width, height);
	}

	Camera* Camera::getPrimary() 
	{ 
		return Core::getEngineState() == Core::EngineState::GAME_STOPPED ? editorPrimary : playPrimary; 
	}

	void Camera::update() 
	{
		if(isPlayPrimary) playPrimary = this;
		if(isEditPrimary) editorPrimary = this;
		if(GameNode::getSelectedNode() == this->getParent()) 
		{
			if(projectionType == CameraProjectionType::PERSPECTIVE_CAMERA_PROJECTION) 
			{
				FrustumPlanes frustum;
				const float halfVSide = zFar * tanf(glm::radians(fov) * .5f);
				const float halfHSide = halfVSide * aspect;
				const float nearHalfVSide = zNear * tanf(glm::radians(fov) * .5f);
				const float nearHalfHSide = nearHalfVSide * aspect;
				const Transform& t = this->getParentOfTypeConst<GameNode3D>()->getWorldTransform();
				const glm::vec3 frontMultFar = zFar * t.forward();
				const glm::vec3 frontMultNear = zNear * t.forward();
				const float normalLength = 32.0f;
				const glm::vec4 color = glm::vec4(0.0f, 1.0f, 0.5f, 1.0f);
				const glm::vec4 color2 = glm::vec4(0.0f, 0.5f, 1.0f, 1.0f);
				const float thickness = 0.05f;
				frustum.nearNormal = -t.forward();
				frustum.farNormal = t.forward();
				frustum.leftNormal = -glm::normalize(glm::cross(frontMultFar - t.right() * halfHSide, t.up()));
				frustum.rightNormal = -glm::normalize(glm::cross(t.up(), frontMultFar + t.right() * halfHSide));
				frustum.upNormal = -glm::normalize(glm::cross(frontMultFar + t.up() * halfVSide, t.right()));
				frustum.bottomNormal = -glm::normalize(glm::cross(t.right(), frontMultFar - t.up() * halfVSide));
				frustum.nearD = glm::dot(frustum.nearNormal, t.getPosition() + zNear * t.forward());
				frustum.farD = glm::dot(frustum.farNormal, t.getPosition() + zFar * t.forward());
				frustum.leftD = glm::dot(frustum.leftNormal, t.getPosition());
				frustum.rightD = glm::dot(frustum.rightNormal, t.getPosition());
				frustum.upD = glm::dot(frustum.upNormal, t.getPosition());
				frustum.bottomD = glm::dot(frustum.bottomNormal, t.getPosition());

				Core::shapesRenderer.pushShape(Line3D(t.getPosition() + frontMultFar, t.getPosition() + frontMultFar + t.forward() * normalLength, color, thickness));
				Core::shapesRenderer.pushShape(Line3D(t.getPosition() + t.forward() * zNear, t.getPosition() + t.forward() * zNear - t.forward() * normalLength, color, thickness));
				
				Core::shapesRenderer.pushShape(Line3D(
					t.getPosition() + (frontMultFar + t.right() * halfHSide) / 2.0f, 
					t.getPosition() + (frontMultFar + t.right() * halfHSide) / 2.0f + frustum.rightNormal * normalLength, 
					color, thickness));
				Core::shapesRenderer.pushShape(Line3D(
					t.getPosition() + (frontMultFar - t.right() * halfHSide) / 2.0f, 
					t.getPosition() + (frontMultFar - t.right() * halfHSide) / 2.0f + frustum.leftNormal * normalLength, 
					color, thickness));
				Core::shapesRenderer.pushShape(Line3D(
					t.getPosition() + (frontMultFar + t.up() * halfVSide) / 2.0f, 
					t.getPosition() + (frontMultFar + t.up() * halfVSide) / 2.0f + frustum.upNormal * normalLength, 
					color, thickness));
				Core::shapesRenderer.pushShape(Line3D(
					t.getPosition() + (frontMultFar - t.up() * halfVSide) / 2.0f, 
					t.getPosition() + (frontMultFar - t.up() * halfVSide) / 2.0f + frustum.bottomNormal * normalLength, 
					color, thickness));

				/////////////////////////////////////////////
				Core::shapesRenderer.pushShape(Line3D(
					t.getPosition() + frontMultFar + t.right() * halfHSide + t.up() * halfVSide, 
					t.getPosition() + frontMultFar - t.right() * halfHSide + t.up() * halfVSide, 
					color2, thickness));
				Core::shapesRenderer.pushShape(Line3D(
					t.getPosition() + frontMultFar + t.right() * halfHSide - t.up() * halfVSide, 
					t.getPosition() + frontMultFar - t.right() * halfHSide - t.up() * halfVSide, 
					color2, thickness));
				
				Core::shapesRenderer.pushShape(Line3D(
					t.getPosition() + frontMultFar + t.right() * halfHSide + t.up() * halfVSide, 
					t.getPosition() + frontMultFar + t.right() * halfHSide - t.up() * halfVSide, 
					color2, thickness));
				Core::shapesRenderer.pushShape(Line3D(
					t.getPosition() + frontMultFar - t.right() * halfHSide + t.up() * halfVSide, 
					t.getPosition() + frontMultFar - t.right() * halfHSide - t.up() * halfVSide, 
					color2, thickness));

				/////////////////////////////////////////////
				Core::shapesRenderer.pushShape(Line3D(
					t.getPosition() + frontMultNear + t.right() * nearHalfHSide + t.up() * nearHalfVSide, 
					t.getPosition() + frontMultNear - t.right() * nearHalfHSide + t.up() * nearHalfVSide, 
					color2, thickness));
				Core::shapesRenderer.pushShape(Line3D(
					t.getPosition() + frontMultNear + t.right() * nearHalfHSide - t.up() * nearHalfVSide, 
					t.getPosition() + frontMultNear - t.right() * nearHalfHSide - t.up() * nearHalfVSide, 
					color2, thickness));
				
				Core::shapesRenderer.pushShape(Line3D(
					t.getPosition() + frontMultNear + t.right() * nearHalfHSide + t.up() * nearHalfVSide, 
					t.getPosition() + frontMultNear + t.right() * nearHalfHSide - t.up() * nearHalfVSide, 
					color2, thickness));
				Core::shapesRenderer.pushShape(Line3D(
					t.getPosition() + frontMultNear - t.right() * nearHalfHSide + t.up() * nearHalfVSide, 
					t.getPosition() + frontMultNear - t.right() * nearHalfHSide - t.up() * nearHalfVSide, 
					color2, thickness));

				/////////////////////////////////////////////
				Core::shapesRenderer.pushShape(Line3D(
					t.getPosition() + frontMultNear + t.right() * nearHalfHSide + t.up() * nearHalfVSide, 
					t.getPosition() + frontMultFar + t.right() * halfHSide + t.up() * halfVSide,
					color2, thickness));
				Core::shapesRenderer.pushShape(Line3D(
					t.getPosition() + frontMultNear - t.right() * nearHalfHSide - t.up() * nearHalfVSide, 
					t.getPosition() + frontMultFar - t.right() * halfHSide - t.up() * halfVSide, 
					color2, thickness));
				
				Core::shapesRenderer.pushShape(Line3D(
					t.getPosition() + frontMultNear + t.right() * nearHalfHSide - t.up() * nearHalfVSide, 
					t.getPosition() + frontMultFar + t.right() * halfHSide - t.up() * halfVSide, 
					color2, thickness));
				Core::shapesRenderer.pushShape(Line3D(
					t.getPosition() + frontMultNear - t.right() * nearHalfHSide + t.up() * nearHalfVSide, 
					t.getPosition() + frontMultFar - t.right() * halfHSide + t.up() * halfVSide, 
					color2, thickness));
			}
			else 
			{
				FrustumPlanes frustum;
				const Transform& t = this->getParentOfTypeConst<GameNode3D>()->getWorldTransform();
				frustum.nearNormal = -t.forward();
				frustum.farNormal = t.forward();
				frustum.leftNormal = -t.right();
				frustum.rightNormal = t.right();
				frustum.upNormal = t.up();
				frustum.bottomNormal = -t.up();
				frustum.nearD = glm::dot(frustum.nearNormal, t.getPosition() + zNear);
				frustum.farD = glm::dot(frustum.farNormal, t.getPosition() + zFar);
				frustum.leftD = glm::dot(frustum.leftNormal, t.getPosition()) + width / 2.0f;
				frustum.rightD = glm::dot(frustum.rightNormal, t.getPosition()) + width / 2.0f;
				frustum.upD = glm::dot(frustum.upNormal, t.getPosition()) + height / 2.0f;
				frustum.bottomD = glm::dot(frustum.bottomNormal, t.getPosition()) + height / 2.0f;
			}
		}
	}

	void Camera::onWindowResize(int width, int height) 
	{
		if(fixToWindowSize) 
		{
			resize(width, height);
		}
	}

	FrustumPlanes Camera::getFrustumPlanes() const 
	{
		if(projectionType == CameraProjectionType::PERSPECTIVE_CAMERA_PROJECTION) 
		{
			FrustumPlanes frustum;
			const float halfVSide = zFar * tanf(glm::radians(fov) * .5f);
			const float halfHSide = halfVSide * aspect;
			const float nearHalfVSide = zNear * tanf(glm::radians(fov) * .5f);
			const float nearHalfHSide = nearHalfVSide * aspect;
			const Transform& t = this->getParentOfTypeConst<GameNode3D>()->getWorldTransform();
			const glm::vec3 frontMultFar = zFar * t.forward();
			const glm::vec3 frontMultNear = zNear * t.forward();
			const float thickness = 0.05f;
			frustum.nearNormal = -t.forward();
			frustum.farNormal = t.forward();
			frustum.leftNormal = -glm::normalize(glm::cross(frontMultFar - t.right() * halfHSide, t.up()));
			frustum.rightNormal = -glm::normalize(glm::cross(t.up(), frontMultFar + t.right() * halfHSide));
			frustum.upNormal = -glm::normalize(glm::cross(frontMultFar + t.up() * halfVSide, t.right()));
			frustum.bottomNormal = -glm::normalize(glm::cross(t.right(), frontMultFar - t.up() * halfVSide));
			frustum.nearD = glm::dot(frustum.nearNormal, t.getPosition() + zNear * t.forward());
			frustum.farD = glm::dot(frustum.farNormal, t.getPosition() + zFar * t.forward());
			frustum.leftD = glm::dot(frustum.leftNormal, t.getPosition());
			frustum.rightD = glm::dot(frustum.rightNormal, t.getPosition());
			frustum.upD = glm::dot(frustum.upNormal, t.getPosition());
			frustum.bottomD = glm::dot(frustum.bottomNormal, t.getPosition());
			return frustum;
		}
		else 
		{
			FrustumPlanes frustum;
			const Transform& t = this->getParentOfTypeConst<GameNode3D>()->getWorldTransform();
			frustum.nearNormal = -t.forward();
			frustum.farNormal = t.forward();
			frustum.leftNormal = -t.right();
			frustum.rightNormal = t.right();
			frustum.upNormal = t.up();
			frustum.bottomNormal = -t.up();
			frustum.nearD = glm::dot(frustum.nearNormal, t.getPosition()) - zNear;
			frustum.farD = glm::dot(frustum.farNormal, t.getPosition()) + zFar;
			frustum.leftD = glm::dot(frustum.leftNormal, t.getPosition()) + width / 2.0f;
			frustum.rightD = glm::dot(frustum.rightNormal, t.getPosition()) + width / 2.0f;
			frustum.upD = glm::dot(frustum.upNormal, t.getPosition()) + height / 2.0f;
			frustum.bottomD = glm::dot(frustum.bottomNormal, t.getPosition()) + height / 2.0f;
			return frustum;
		}
	}
	void Camera::setPlayPrimary(bool isPlayPrimary) 
	{
		this->isPlayPrimary = isPlayPrimary;
	}
	void Camera::setFixToWindowSize(bool fix) 
	{
		fixToWindowSize = fix;
		resize(Graphics::window->getScreenWidth(), Graphics::window->getScreenHeight());
	}

	Camera* Camera::editorPrimary = nullptr;
	Camera* Camera::playPrimary = nullptr;
	EXPORT_COMPONENT(Camera);
}
