#include "RenderUtil.h"
#include "Graphics/Graphics.h"
#include "Graphics/Mesh/MeshUtil.h"
#include "Core.h"

namespace TealEngine {
	namespace Render
	{
		using namespace Graphics;
		glm::mat4 VP_matrix;
		glm::mat4 V_matrix;

		void renderShader(Kirino::Material* shader)
		{
			Kirino::UBOBindings uboBindings;
			uboBindings.reset();
			Kirino::TextureBindings textureBindings;
			textureBindings.reset();
			shader->useProgram();
			shader->setupUniforms();
			shader->setupTextures(textureBindings);
			shader->setupUniformBlocks(uboBindings);
			glBindVertexArray(*BasicMeshes::screenSquare.vao);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}

		void renderTexture(unsigned int texture)
		{
			static Kirino::Material screenImageMat = Kirino::Material(&Core::shadersManager.get("ScreenImage"));
			screenImageMat.setTexture("textureImage", &texture, 1, 0);
			renderShader(&screenImageMat);
		}
	}
}