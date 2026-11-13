#pragma once
#include "Kirino/Mesh.h"
#include <vector>
#include "glm/glm.hpp"
#include <GL/glew.h>
namespace Kirino 
{
#pragma pack(push, 1)
	struct Vertex3D 
	{
		float x, y, z;
		//unsigned int uv, normTB, normN;
		glm::vec2 uv;
		glm::vec3 tangent, bitangent, normal;
	};
#pragma pack(pop)

	class Mesh3DBuilder
	{
		private:
		Mesh mesh;
		unsigned int vbo, ebo;
		VertexArrayObject vao;
		public:
		Mesh3DBuilder();
		std::vector<Vertex3D> vertData;
		std::vector<unsigned int> indices;
		void buildFromVectors(
			const std::vector<glm::vec3>& vertices,
			const std::vector<glm::vec2>& uvs,
			const std::vector<glm::vec3>& normals,
			const std::vector<unsigned int>& indices
		);
		void clearData();
		void uploadData(GLuint usage = GL_STATIC_DRAW);
		void buildDataFromVectors(
			const std::vector<glm::vec3>& vertices,
			const std::vector<glm::vec2>& uvs,
			const std::vector<glm::vec3>& normals,
			const std::vector<unsigned int>& indices
		);
		inline const Mesh& getMesh() const { return mesh; }
		static void calcTangentsAndBitangetns(
			const std::vector<glm::vec3>& vertices,
			const std::vector<glm::vec2>& uvs,
			const std::vector<glm::vec3>& normals,
			const std::vector<unsigned int>& indices,
			std::vector<glm::vec3>& tangentsOut,
			std::vector<glm::vec3>& bitangentsOut
		);
		static void calcNormals(
			const std::vector<glm::vec3>& vertices,
			const std::vector<unsigned int>& indices,
			std::vector<glm::vec3>& normalsOut
		);
	};
}