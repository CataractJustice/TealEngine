#pragma once
#include "GameNode/Component.h"
#include "Graphics/Texture.h"
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
		bool fixToWindowSize;
		float fov, aspect, zNear, zFar, width, height;
		bool fixedToWindowSize = false;
		CameraProjectionType projectionType;
		std::function<void()> viewFromHere;
		static Camera* playPrimary;
		static Camera* editorPrimary;
		
		FrustumPlanes frustumPlanes;
	protected:
		bool isPlayPrimary;
		bool isEditPrimary;
	public:
		Camera();
		~Camera();
		static Camera* getPrimary();
		void setPerspectiveProjection(float fov, float aspect, float zNear, float zFar);
		void setOrthoProjection(float width, float height, float zNear, float zFar);
		void resize(int width, int height);
		glm::mat4 getPV()const;
		glm::mat4 getProjection()const;
		float getNear()const;
		float getFar()const;
		float getAspect()const;
		float getFOV()const;
		void update() override;
		inline float getWidth() const { return width; }
		inline float getHeight() const { return height; }
		void onPropSet(const std::string& propName) override;
		void onPropRefresh() override;
		CameraProjectionType getProjectionType();
		void onWindowResize(int width, int height) override;
		FrustumPlanes getFrustumPlanes() const;
		void setPlayPrimary(bool isPlayPrimary);
		void setFixToWindowSize(bool fix);
	};
}
