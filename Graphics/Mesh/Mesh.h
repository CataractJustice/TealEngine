#pragma once
#include "GameNode/GameNode3D.h"
#include "Graphics/Shader/ShaderProgram.h"
#include "Filesystem/loadStrFromFile.h"
#include "libs/glm/glm.hpp"
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

	class SharedMesh : public GameNode3D
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
		unsigned int VBO, EBO;
		unsigned int usage;

	protected:
		vector<vec3> vertices;
		vector<vec3> normals;
		vector<vec3> tangets;
		vector<vec2> UVs;
		vector<vec4> colors;
		vector<unsigned int> indices;
		
	public:

		bool validation();
		void apply();

		//Input
		void setVertices(std::vector<vec3> vertices);
		void setNormals(std::vector<vec3> normals);
		void setTangets(std::vector<vec3> tangets);
		void setUVs(std::vector<vec2> UVs);
		void setColors(std::vector<vec4> colors);
		void setIndices(std::vector<unsigned int> indices);

		//Output
		SharedMesh* sharedMesh();
		vector<vec3> getVertices();
		vector<vec3> getTriangles();
		vector<vec3> getNormals();
		vector<vec3> getTangets();
		vector<vec2> getUVs();
		vector<vec4> getColors();
		vector<unsigned int> getIndices();
		unsigned int getLength() override;
		
		//Modify
		void addMesh(Mesh mesh);
		void addMesh(Mesh mesh, mat4 transform, bool noRotation = false);
		void calcNormals();
		void calcTangets();
		void clear();
		void load(string path);
		void removeDoubles();
		void splitTriangles();
		void addLODIndex(unsigned int index);
		void popLODIndex();
		void setLODIndex(unsigned int LOD, unsigned int index);
		~Mesh();
		Mesh(unsigned int usage = -1);
	};
}