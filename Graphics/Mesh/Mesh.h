#pragma once
#include <glew.h>
#include <GLFW/glfw3.h>
#include "../../GameNode/GameNode3D.h"
#include "../Shader/ShaderProgram.h"
#include "../../Filesystem/loadStrFromFile.h"
#include "../../libs/glm/glm.hpp"
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
		GLuint VAO;
		GLuint* ilength;
		std::vector<GLuint> LODs;
		GLuint LODsCount;
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
		void render(GLuint LOD = 0, GLuint mode = GL_TRIANGLES);
		virtual GLuint getLength();
		GLuint getLODIndex(GLuint LOD);
		GLuint getLODCount();
	};

	class Mesh : public SharedMesh
	{
	private:
		GLuint VBO, EBO;
		GLenum usage;

	protected:
		vector<vec3> vertices;
		vector<vec3> normals;
		vector<vec3> tangets;
		vector<vec2> UVs;
		vector<vec4> colors;
		vector<GLuint> indices;
		
	public:

		bool validation();
		void apply();

		//Input
		void setVertices(std::vector<vec3> vertices);
		void setNormals(std::vector<vec3> normals);
		void setTangets(std::vector<vec3> tangets);
		void setUVs(std::vector<vec2> UVs);
		void setColors(std::vector<vec4> colors);
		void setIndices(std::vector<GLuint> indices);

		//Output
		SharedMesh* sharedMesh();
		vector<vec3> getVertices();
		vector<vec3> getTriangles();
		vector<vec3> getNormals();
		vector<vec3> getTangets();
		vector<vec2> getUVs();
		vector<vec4> getColors();
		vector<GLuint> getIndices();
		GLuint getLength() override;
		
		//Modify
		void addMesh(Mesh mesh);
		void addMesh(Mesh mesh, mat4 transform, bool noRotation = false);
		void calcNormals();
		void calcTangets();
		void clear();
		void load(string path);
		void removeDoubles();
		void splitTriangles();
		void updateAABB();
		void addLODIndex(GLuint index);
		void popLODIndex();
		void setLODIndex(GLuint LOD, GLuint index);
		~Mesh();
		Mesh(GLenum usage = GL_STATIC_DRAW);
	};
}