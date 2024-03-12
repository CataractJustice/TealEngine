#pragma once
#include "GameNode/GameNode3D.h"
#include "Graphics/Shader/ShaderProgram.h"
#include "FileSystem/loadStrFromFile.h"
#include "libs/glm/glm.hpp"
#include "GameNode/Component.h"
#include "VertexBuffer.h"
using namespace glm;
#define TE_MAX_BONES_PER_VERTEX 4
namespace TealEngine
{
	enum MESH_ATTRIBS
	{
		VERTICES,
		NORMAL,
		TANGET,
		UV,
		COLOR,
		BONE_ID0,
		BONE_ID1,
		BONE_ID2,
		BONE_ID3,
		BONE_WEIGHT0,
		BONE_WEIGHT1,
		BONE_WEIGHT2,
		BONE_WEIGHT3,
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

		vector<int> bone0Ids;
		vector<int> bone1Ids;
		vector<int> bone2Ids;
		vector<int> bone3Ids;
		vector<float> bone0Weights;
		vector<float> bone1Weights;
		vector<float> bone2Weights;
		vector<float> bone3Weights;

		vector<unsigned int> indices;
		std::string name;
		
	public:
		bool validation();
		void apply();

		//Input
		inline void setVertices	(const std::vector<vec3>& vertices)			{ this->vertices = vertices; };
		inline void setNormals	(const std::vector<vec3>& normals)			{ this->normals = normals; };
		inline void setTangents	(const std::vector<vec3>& tangents)			{ this->tangents = tangents; };
		inline void setUVs		(const std::vector<vec2>& UVs)				{ this->UVs = UVs; };
		inline void setColors	(const std::vector<vec4>& colors)			{ this->colors = colors; };
		inline void setIndices	(const std::vector<unsigned int>& indices)	{ this->indices = indices; };
		inline void setBone0Ids	(const std::vector<float>& bones);
		inline void setBone1Ids	(const std::vector<float>& bones);
		inline void setBone2Ids	(const std::vector<float>& bones);
		inline void setBone4Ids	(const std::vector<float>& bones);
		inline void setBone3Weights(const std::vector<float>& weights) { this->bone0Weights = weights; };
		inline void setBone0Weights(const std::vector<float>& weights) { this->bone1Weights = weights; };
		inline void setBone1Weights(const std::vector<float>& weights) { this->bone2Weights = weights; };
		inline void setBone2Weights(const std::vector<float>& weights) { this->bone3Weights = weights; };

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