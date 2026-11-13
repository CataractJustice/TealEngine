#include "Mesh3DBuilder.h"
#include <GL/glew.h>
#include <cstddef>

unsigned short encodeNormal(glm::vec3 normal) 
{
	
	glm::vec3 absN = glm::abs(normal);
	float sum = absN.x + absN.y + absN.z;
	float invSum = 1.0f / sum;
	glm::vec2 enc;
	if (normal.z >= 0) {
		enc = glm::vec2(normal.x, normal.y) * invSum;
	} else {
		enc = glm::vec2(
			(normal.x >= 0.0f) ? 1.0f - absN.y * invSum : -1.0f + absN.y * invSum,
			(normal.y >= 0.0f) ? 1.0f - absN.x * invSum : -1.0f + absN.x * invSum
		);
	}
	return ((unsigned short)(enc.x * 255.0f)) | ((unsigned short)(enc.y * 255.0f) << 8);
}
unsigned int encodeUV(glm::vec2 uv) 
{
	unsigned int x = static_cast<unsigned int>(uv.x * 65535.0f) & 0xffff;
	unsigned int y = static_cast<unsigned int>(uv.y * 65535.0f) & 0xffff;
	return (x | (y << 16));
};
namespace Kirino
{
	Mesh3DBuilder::Mesh3DBuilder() 
	{
		this->mesh.boundingBox = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
		this->mesh.indexCount = 0;
		this->mesh.indexOffset = 0;
		this->mesh.vao = nullptr;
		this->mesh.vertexOffset = 0;
		this->ebo = 0;
		this->vbo = 0;
	}
	
	void Mesh3DBuilder::buildFromVectors(
		const std::vector<glm::vec3>& vertices,
		const std::vector<glm::vec2>& uvs,
		const std::vector<glm::vec3>& normals,
		const std::vector<unsigned int>& indices
	) 
	{
		buildDataFromVectors(vertices, uvs, normals, indices);
		uploadData();
	}

	void Mesh3DBuilder::uploadData(GLuint usage) 
	{
		if(vao == 0) 
		{
			vao.create();
		}
		glBindVertexArray(vao);
		glGenBuffers(1, &vbo);
		glGenBuffers(1, &ebo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex3D) * vertData.size(), vertData.data(), usage);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), indices.data(), usage);
		vao.vertexAttribPointer(VertexArrayObject::VertexAttribPointer(true, vbo, 0, 3, GL_FLOAT, false, sizeof(Vertex3D), 0, 0));
		vao.vertexAttribPointer(VertexArrayObject::VertexAttribPointer(true, vbo, 1, 2, GL_FLOAT, false, sizeof(Vertex3D), (void*)(sizeof(float) * 3), 0));
		vao.vertexAttribPointer(VertexArrayObject::VertexAttribPointer(true, vbo, 2, 3, GL_FLOAT, false, sizeof(Vertex3D), (void*)(sizeof(float) * 5), 0));
		vao.vertexAttribPointer(VertexArrayObject::VertexAttribPointer(true, vbo, 3, 3, GL_FLOAT, false, sizeof(Vertex3D), (void*)(sizeof(float) * 8), 0));
		vao.vertexAttribPointer(VertexArrayObject::VertexAttribPointer(true, vbo, 4, 3, GL_FLOAT, false, sizeof(Vertex3D), (void*)(sizeof(float) * 11), 0));
		vao.updateAttribs();	
		mesh.vao = &vao;
	}

	void Mesh3DBuilder::buildDataFromVectors(
		const std::vector<glm::vec3>& vertices,
		const std::vector<glm::vec2>& uvs,
		const std::vector<glm::vec3>& normals,
		const std::vector<unsigned int>& indices
	) 
	{
		this->indices = indices;
		std::vector<glm::vec3> tangents;
		std::vector<glm::vec3> bitangents;
		vertData.clear();
		//vertData.reserve(vertices.size());
		calcTangentsAndBitangetns(vertices, uvs, normals, indices, tangents, bitangents);
		for(int i = 0; i < vertices.size(); i++) 
		{
			Vertex3D vertex;
			vertex.x = vertices[i].x;
			vertex.y = vertices[i].y;
			vertex.z = vertices[i].z;
			vertex.uv = uvs[i];
			vertex.tangent = tangents[i];
			vertex.bitangent = bitangents[i];
			vertex.normal = normals[i];
			vertData.push_back(vertex);
			mesh.boundingBox.maxX = std::max(mesh.boundingBox.maxX, vertices[i].x);
			mesh.boundingBox.maxY = std::max(mesh.boundingBox.maxY, vertices[i].y);
			mesh.boundingBox.maxZ = std::max(mesh.boundingBox.maxZ, vertices[i].z);
			mesh.boundingBox.minX = std::min(mesh.boundingBox.minX, vertices[i].x);
			mesh.boundingBox.minY = std::min(mesh.boundingBox.minY, vertices[i].y);
			mesh.boundingBox.minZ = std::min(mesh.boundingBox.minZ, vertices[i].z);
		}
		mesh.indexCount = indices.size();
		mesh.indexOffset = 0;
		mesh.vertexOffset = 0;
	}

	void Mesh3DBuilder::calcTangentsAndBitangetns(
		const std::vector<glm::vec3>& vertices,
		const std::vector<glm::vec2>& uvs,
		const std::vector<glm::vec3>& normals,
		const std::vector<unsigned int>& indices,
		std::vector<glm::vec3>& tangentsOut,
		std::vector<glm::vec3>& bitangentsOut
	)
	{
		tangentsOut.resize(normals.size());
		bitangentsOut.resize(normals.size());
		for (int i = 0; i < vertices.size(); i++) tangentsOut[i] = glm::vec3(0.0f);
		for (int i = 0; i < vertices.size(); i++) bitangentsOut[i] = glm::vec3(0.0f);
		for (unsigned int i = 0; i < indices.size(); i += 3)
		{
			int index = indices[i];
			glm::vec3 deltaPos[2];
			deltaPos[0] = vertices[indices[i + 1]] - vertices[index];
			deltaPos[1] = vertices[indices[i + 2]] - vertices[index];
			glm::vec2 deltaUV[2];
			deltaUV[0] = uvs[indices[i + 1]] - uvs[index];
			deltaUV[1] = uvs[indices[i + 2]] - uvs[index];
			if((deltaUV[0].x * deltaUV[1].y - deltaUV[1].x * deltaUV[0].y) == 0.0f) 
			{
				continue;
			}
			GLfloat r = 1.0f / (deltaUV[0].x * deltaUV[1].y - deltaUV[1].x * deltaUV[0].y);
			glm::vec3 tangent = (deltaPos[0] * deltaUV[1].y - deltaPos[1] * deltaUV[0].y) * r;
			tangentsOut[index] += tangent;
			tangentsOut[indices[i + 1]] += tangent;
			tangentsOut[indices[i + 2]] += tangent;
			
			glm::vec3 btangent = (deltaPos[0] * deltaUV[1].x - deltaPos[1] * deltaUV[0].x) * r;
			bitangentsOut[index] += btangent;
			bitangentsOut[indices[i + 1]] += btangent;
			bitangentsOut[indices[i + 2]] += btangent;
		}
		for (int i = 0; i < vertices.size(); i++) tangentsOut[i] = normalize(tangentsOut[i]);
		for (int i = 0; i < vertices.size(); i++) bitangentsOut[i] = normalize(bitangentsOut[i]);
	}

	void Mesh3DBuilder::calcNormals(
		const std::vector<glm::vec3>& vertices,
		const std::vector<unsigned int>& indices,
		std::vector<glm::vec3>& normalsOut
	) 
	{
		normalsOut.resize(vertices.size());
		for (int i = 0; i < vertices.size(); i++) normalsOut[i] = glm::vec3(0.0f);
		for (unsigned int i = 0; i < indices.size(); i += 3)
		{
			int index = indices[i];
			glm::vec3 deltaPos[2];
			deltaPos[0] = vertices[indices[i + 1]] - vertices[index];
			deltaPos[1] = vertices[indices[i + 2]] - vertices[index];
			glm::vec3 normal = glm::normalize(glm::cross(deltaPos[0], deltaPos[1]));
			normalsOut[index] += normal;
			normalsOut[indices[i + 1]] += normal;
			normalsOut[indices[i + 2]] += normal;
		}
		for (int i = 0; i < vertices.size(); i++) normalsOut[i] = normalize(normalsOut[i]);
	}
} // namespace Kirino
