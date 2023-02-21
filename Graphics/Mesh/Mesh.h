#pragma once
#include "GameNode/GameNode3D.h"
#include "Graphics/Shader/ShaderProgram.h"
#include "FileSystem/loadStrFromFile.h"
#include "libs/glm/glm.hpp"
#include "GameNode/Component.h"
#include "VertexBuffer.h"
using namespace glm;
namespace TealEngine
{
	enum MESH_ATTRIBS
	{
		VERTICES,
		NORMAL,
		TANGET,
		UV,
		COLOR,

		MESH_ATTRIBS_SIZE
	};

	class SharedMesh
	{
	protected:
		unsigned int VAO;
		unsigned int* ilength;
		std::vector<unsigned int> LODs;
		unsigned int LODsCount;
		//layouts
		//- - vertice position
		//0 - vertice normal
		//1 - vertice tanget
		//2 - vertice UV
		//3 - vertice color
		bool attribs[MESH_ATTRIBS_SIZE];
	public:
		//M
		void enableAttrib(MESH_ATTRIBS target);
		void enableAllAttribs();
		void disableAttrib(MESH_ATTRIBS target);
		//O
		void render(unsigned int LOD = 0, unsigned int mode = -1);
		virtual unsigned int getLength();
		unsigned int getLODIndex(unsigned int LOD);
		unsigned int getLODCount();
	};

	class Mesh : public SharedMesh
	{
	private:
		unsigned int EBO;
		VertexBuffer VB;

	protected:
		vector<vec3> vertices;
		vector<vec3> normals;
		vector<vec3> tangents;
		vector<vec3> bitangents;
		vector<vec2> UVs;
		vector<vec4> colors;
		vector<unsigned int> indices;
		std::string name;
		
	public:
		bool validation();
		void apply();

		//Input
		void setVertices(std::vector<vec3> vertices);
		void setNormals(std::vector<vec3> normals);
		void setTangents(std::vector<vec3> tangents);
		void setUVs(std::vector<vec2> UVs);
		void setColors(std::vector<vec4> colors);
		void setIndices(std::vector<unsigned int> indices);

		//Output
		SharedMesh* sharedMesh();
		vector<vec3> getVertices();
		vector<vec3> getTriangles();
		vector<vec3> getNormals();
		vector<vec3> getTangents();
		vector<vec2> getUVs();
		vector<vec4> getColors();
		vector<unsigned int> getIndices();
		unsigned int getLength() override;
		
		//Modify
		void addMesh(Mesh* mesh);
		void addMesh(Mesh* mesh, mat4 transform, bool noRotation = false);
		void calcNormals();
		void calcTangents();
		void clear();
		void load(string path);
		void removeDoubles();
		void splitTriangles();
		void addLODIndex(unsigned int index);
		void popLODIndex();
		void setLODIndex(unsigned int LOD, unsigned int index);
		~Mesh();
		Mesh(unsigned int usage = -1);

		inline void setName(const std::string& name) { this->name = name; };
		inline std::string getName() { return this->name; };
	};
}