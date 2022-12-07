#pragma once
#include "Graphics/Mesh/Mesh.h"
#include "Graphics/Mesh/MeshRenderer.h"
#include "Graphics/Camera.h"
#include "Graphics/FrameBuffer/FrameBuffer.h"

namespace TealEngine {
	class ForwardRenderer
	{
	protected:
		Camera* activeCamera;
		ShaderProgram shader;
		Texture depthTexture;
		bool depthTest, stencilTest, colorClear, depthClear, stencilClear;
		glm::vec4 clearColor;
		void applyConfig();
		void renderModels(GameNode* scene, ShaderProgram* shader = nullptr, MeshRenderer::RenderStage stage = (MeshRenderer::RenderStage)0);
		void renderModels(list<MeshRenderer*> meshList, ShaderProgram* shader = nullptr, MeshRenderer::RenderStage stage = (MeshRenderer::RenderStage)0);
	public:
		FrameBuffer fb;
		virtual void resize(GLuint width, GLuint height);
		virtual void render(GameNode* scene, ShaderProgram* material = NULL, bool depthOnly = false);
		virtual void render(list<MeshRenderer*> meshList, ShaderProgram* material = NULL, bool depthOnly = false);
		void setDepthTest(bool value);
		void setStencilTest(bool value);
		void setColorClear(bool value);
		void setDepthClear(bool value);
		void setStencilClear(bool value);
		void setClearColor(glm::vec4 color);
		void setCamera(Camera* camera);
		Camera* getActiveCamera();
	};
}