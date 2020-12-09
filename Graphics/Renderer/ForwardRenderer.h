#pragma once
#include "Graphics/Mesh/Mesh.h"
#include "Graphics/Mesh/MeshRenderer.h"
#include "Graphics/Camera.h"
#include "Graphics/FrameBuffer/FrameBuffer.h"

namespace TealEngine {
	class ForwardRenderer
	{
	protected:
		list<MeshRenderer*> meshList;
		Camera* activeCamera;
		ShaderProgram shader;
		Texture depthTexture;
		bool depthTest, stencilTest, colorClear, depthClear, stencilClear;
		glm::vec4 clearColor;
		void applyConfig();
		void renderModels(ShaderProgram* shader = nullptr);
		void renderModels(list<MeshRenderer*> meshList, ShaderProgram* shader = nullptr);
	public:
		FrameBuffer fb;
		virtual void resize(GLuint width, GLuint height);
		virtual void render(list<MeshRenderer*> meshList, ShaderProgram* material = NULL, bool depthOnly = false);
		virtual void render(ShaderProgram* material = NULL);
		virtual void push(MeshRenderer* mesh);
		virtual void push(GameNode* scene);
		virtual void pop(MeshRenderer* mesh);
		virtual void pop(GameNode* mesh);
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