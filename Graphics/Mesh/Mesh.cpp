#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Mesh.h"
#include "System/Debug.h"
#include <fstream>
#include <limits>
namespace TealEngine
{
	void SharedMesh::enableAttrib(MESH_ATTRIBS target)
	{
		this->attribs[target] = true;
	}

	void SharedMesh::enableAllAttribs() 
	{
		for (int i = 0; i < MESH_ATTRIBS_SIZE; i++)
			enableAttrib((MESH_ATTRIBS)i);
	}

	void SharedMesh::disableAttrib(MESH_ATTRIBS target)
	{
		this->attribs[target] = false;
	}

	void SharedMesh::render(unsigned int LOD, unsigned int mode)
	{
		glBindVertexArray(VAO);
		glDrawElements(mode, getLODIndex(LOD + 1) - getLODIndex(LOD), GL_UNSIGNED_INT, (void*)(getLODIndex(LOD) * sizeof(unsigned int)));
	}

	unsigned int SharedMesh::getLength() 
	{
		return *this->ilength;
	}

	unsigned int SharedMesh::getLODIndex(unsigned int LOD)
	{
		if (LOD == 0)
		{
			return 0;
		}
		else if (LOD > LODs.size())
		{
			return this->getLength();
		}
		else 
		{
			return LODs[LOD - 1];
		}
	}
	unsigned int SharedMesh::getLODCount()
	{
		return LODs.size() + 1;
	}

	bool Mesh::validation()
	{
		//normal
		if (normals.size() > 0 && vertices.size() != normals.size())
		{
			throw "Vertices and normals length do not match up.\n";
		}

		//tanget
		if (tangents.size() > 0 && vertices.size() != tangents.size())
		{
			throw "Vertices and tangents length do not match up.\n";
		}

		//UV
		if (UVs.size() != 0 && vertices.size() != UVs.size())
		{
			throw "Vertices and UVs length do not match up.\n";
		}

		//color
		if (colors.size() > 0 && vertices.size() != colors.size())
		{
			throw "Vertices and colors length do not match up.\n";
		}

		//indices
		for (unsigned int i = 0; i < indices.size(); i++)
			if (indices[i] >= vertices.size())
			{
				throw "Indice out of vertices bounds.\n";
				break;
			}

		return true;
	}

	Mesh::Mesh(unsigned int usage)
	{
		ilength = new GLuint(0);
		this->LODsCount = 0;
		this->EBO = 0;
	}

	void Mesh::apply()
	{
		if (validation())
		{
			vector<float> verticesFloat;
			vector<float> normalsFloat;
			vector<float> tangentsFloat;
			vector<float> UVsFloat;
			vector<float> colorsFloat;
			vector<float> bitangentsFloat;

			for(int i = 0; i < vertices.size(); i++) 
			{
				verticesFloat.push_back(vertices[i].x);
				verticesFloat.push_back(vertices[i].y);
				verticesFloat.push_back(vertices[i].z);
				if(normals.size()) 
				{
					normalsFloat.push_back(normals[i].x);
					normalsFloat.push_back(normals[i].y);
					normalsFloat.push_back(normals[i].z);
				}
				if(tangents.size())
				{
					tangentsFloat.push_back(tangents[i].x);
					tangentsFloat.push_back(tangents[i].y);
					tangentsFloat.push_back(tangents[i].z);
				}
				if(UVs.size())
				{
					UVsFloat.push_back(UVs[i].x);
					UVsFloat.push_back(UVs[i].y);
				}
				if(colors.size()) 
				{
					colorsFloat.push_back(colors[i].x);
					colorsFloat.push_back(colors[i].y);
					colorsFloat.push_back(colors[i].z);
					colorsFloat.push_back(colors[i].w);
				}
				if(bitangents.size())
				{
					bitangentsFloat.push_back(bitangents[i].x);
					bitangentsFloat.push_back(bitangents[i].y);
					bitangentsFloat.push_back(bitangents[i].z);
				}
			}

			VB.clearVertexAttributes();
			if(this->vertices.size()) 
			{
				VertexAttribute& VA = VB.addVertexAttribute(0, 3);
				VA.data = verticesFloat.data();
				VA.vertices = verticesFloat.size() / 3;
			}
			if(this->normals.size()) 
			{
				VertexAttribute& VA = VB.addVertexAttribute(1, 3);
				VA.data = normalsFloat.data();
				VA.vertices = normalsFloat.size() / 3;
			}
			if(this->tangents.size()) 
			{
				VertexAttribute& VA = VB.addVertexAttribute(2, 3);
				VA.data = tangentsFloat.data();
				VA.vertices = tangentsFloat.size() / 3;
			}
			if(this->UVs.size()) 
			{
				VertexAttribute& VA = VB.addVertexAttribute(3, 2);
				VA.data = UVsFloat.data();
				VA.vertices = UVsFloat.size() / 2;
			}
			if(this->colors.size()) 
			{
				VertexAttribute& VA = VB.addVertexAttribute(4, 4);
				VA.data = colorsFloat.data();
				VA.vertices = colorsFloat.size() / 4;
			}
			if(this->bitangents.size()) 
			{
				VertexAttribute& VA = VB.addVertexAttribute(5, 3);
				VA.data = bitangentsFloat.data();
				VA.vertices = bitangentsFloat.size() / 3;
			}
			VB.setBufferUsage(GL_STATIC_DRAW);
			VB.build();
			if (!EBO) 
			{
				glGenBuffers(1, &EBO);
			}
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(), indices.data(), GL_STATIC_DRAW);

			*this->ilength = this->indices.size();
			this->VAO = VB.getVAO();
		}
	}

	//I
	void Mesh::setVertices(std::vector<vec3> vertices)
	{
		this->vertices = vertices;
	}

	void Mesh::setNormals(std::vector<vec3> normals)
	{
		this->normals = normals;
	}

	void Mesh::setTangents(std::vector<vec3> tangents)
	{
		this->tangents = tangents;
	}

	void Mesh::setUVs(std::vector<vec2> UVs)
	{
		this->UVs = UVs;
	}

	void Mesh::setColors(std::vector<vec4> colors)
	{
		this->colors = colors;
	}

	void Mesh::setIndices(std::vector<GLuint> indices)
	{
		this->indices = indices;
	}

	void Mesh::load(string path)
	{
		vector<vec3> vertices;
		vector<vec2> UVs;
		vector<vec3> normals;
		
		std::ifstream file(path);

		while (true) 
		{
			vec3 v3;

			if (file.eof()) 
			{
				break;
			}

			std::string word;
			file >> word;

			if(word == "v") 
			{
				file >> v3.x;
				file >> v3.y;
				file >> v3.z;
				vertices.push_back(v3);
			}
			else
			if(word == "vt") 
			{
				file >> v3.x;
				file >> v3.y;
				UVs.push_back(glm::vec2(v3.x, v3.y));
			}
			else
			if(word == "vn") 
			{
				file >> v3.x;
				file >> v3.y;
				file >> v3.z;
				normals.push_back(v3);
			}
			else
			if(word == "f") 
			{
				for (int i = 0; i < 3; i++)
				{
					char triangleStr[256];
					file >> triangleStr;
					int indices[3];
					char* c = triangleStr;
					char* i1 = triangleStr;
					while(*c != '/') c++;
					*c = 0;
					char* i2 = ++c;
					while(*c != '/') c++;
					*c = 0;
					char* i3 = ++c;

					indices[0] = std::atoi(i1) - 1;
					indices[1] = std::atoi(i2) - 1;
					indices[2] = std::atoi(i3) - 1;
					
					this->indices.push_back(this->indices.size());
					this->vertices.push_back(vertices[indices[0]]);
					this->UVs.push_back(UVs[indices[1]]);
					this->normals.push_back(normals[indices[2]]);
				}
			}
		}
		
		//this->removeDoubles();
		//this->calcNormals();
		this->calcTangents();
		this->apply();
	}

	void Mesh::clear()
	{
		this->vertices.clear();
		this->normals.clear();
		this->tangents.clear();
		this->UVs.clear();
		this->colors.clear();
		this->indices.clear();
		this->LODs.clear();
		apply();
	}

	void Mesh::addMesh(Mesh* mesh)
	{
		unsigned offset = this->vertices.size();
		for (int i = 0; i < mesh->indices.size(); i++) this->indices.push_back(mesh->indices[i] + offset);
		if (this->attribs[MESH_ATTRIBS::VERTICES]) for (int i = 0; i < mesh->vertices.size(); i++) this->vertices.push_back(mesh->vertices[i]);
		if (this->attribs[MESH_ATTRIBS::NORMAL])  for (int i = 0; i < mesh->normals.size(); i++) this->normals.push_back(mesh->normals[i]);
		if (this->attribs[MESH_ATTRIBS::TANGET]) for (int i = 0; i < mesh->tangents.size(); i++) this->tangents.push_back(mesh->tangents[i]);
		if (this->attribs[MESH_ATTRIBS::UV]) for (int i = 0; i < mesh->UVs.size(); i++) this->UVs.push_back(mesh->UVs[i]);
		if (this->attribs[MESH_ATTRIBS::COLOR]) for (int i = 0; i < mesh->colors.size(); i++) this->colors.push_back(mesh->colors[i]);
	}

	void Mesh::addMesh(Mesh* mesh, mat4 transform, bool noRotation)
	{
		unsigned offset = this->vertices.size();
		for (int i = 0; i < mesh->indices.size(); i++)
			this->indices.push_back(mesh->indices[i] + offset);
		if (this->attribs[MESH_ATTRIBS::VERTICES])for (int i = 0; i < mesh->vertices.size(); i++) {vertices.push_back(vec3 (transform * vec4(mesh->vertices[i], 1.0f)));}
		if (this->attribs[MESH_ATTRIBS::NORMAL]) {
			if(noRotation)
			{
				for (int i = 0; i < mesh->normals.size(); i++) normals.push_back(mesh->normals[i]);
				if (this->attribs[MESH_ATTRIBS::TANGET])
				{
					for (int i = 0; i < mesh->tangents.size(); i++) tangents.push_back(mesh->tangents[i]);
				}
			}
			else 
			{
				mat4 nmodel = glm::transpose(glm::inverse(transform));
				for (int i = 0; i < mesh->normals.size(); i++) normals.push_back(vec3(nmodel * vec4(mesh->normals[i], 0.0f)));
				if (this->attribs[MESH_ATTRIBS::TANGET])
				{
					for (int i = 0; i < mesh->tangents.size(); i++) tangents.push_back(vec3(nmodel * vec4(mesh->tangents[i], 0.0f)));
				}
			}
		}
		if (this->attribs[MESH_ATTRIBS::UV])this->UVs.insert(this->UVs.end(), mesh->UVs.begin(), mesh->UVs.end());
		if (this->attribs[MESH_ATTRIBS::COLOR])this->colors.insert(this->colors.end(), mesh->colors.begin(), mesh->colors.end());
	}

	//M
	void Mesh::calcTangents()
	{
		if (validation())
		{
			std::vector<vec3> t;
			std::vector<vec3> b;
			for (int i = 0; i < vertices.size(); i++) t.push_back(vec3(0.0f));
			for (int i = 0; i < vertices.size(); i++) b.push_back(vec3(0.0f));
			for (unsigned int i = 0; i < indices.size(); i += 3)
			{
				int index = indices[i];
				vec3 deltaPos[2];
				deltaPos[0] = vertices[indices[i + 1]] - vertices[index];
				deltaPos[1] = vertices[indices[i + 2]] - vertices[index];

				vec2 deltaUV[2];
				deltaUV[0] = UVs[indices[i + 1]] - UVs[index];
				deltaUV[1] = UVs[indices[i + 2]] - UVs[index];

				GLfloat r = 1.0f / (deltaUV[0].x * deltaUV[1].y - deltaUV[1].x * deltaUV[0].y);
				vec3 tangent = (deltaPos[0] * deltaUV[1].y - deltaPos[1] * deltaUV[0].y) * r;
				t[index] += tangent;
				t[indices[i + 1]] += tangent;
				t[indices[i + 2]] += tangent;
				
				vec3 btangent = (deltaPos[0] * deltaUV[1].x - deltaPos[1] * deltaUV[0].x) * r;
				b[index] += btangent;
				b[indices[i + 1]] += btangent;
				b[indices[i + 2]] += btangent;
			}
			for (int i = 0; i < vertices.size(); i++) t[i] = normalize(t[i]);
			for (int i = 0; i < vertices.size(); i++) b[i] = normalize(b[i]);
			this->tangents = t;
			this->bitangents = b;
			if (!validation())
			{
				TE_DEBUG_ERROR("Indices should include all vertices")
			}
		}
	}

	void Mesh::calcNormals()
	{
		if (validation())
		{
			std::vector<vec3> n;
			for (int i = 0; i < vertices.size(); i++) n.push_back(vec3(0.0f));
			for (unsigned int i = 0; i < indices.size(); i += 3)
			{
				int index = indices[i];
				vec3 deltaPos[2];
				deltaPos[0] = vertices[indices[i + 1]] - vertices[index];
				deltaPos[1] = vertices[indices[i + 2]] - vertices[index];

				vec3 normal = glm::normalize(glm::cross(deltaPos[0], deltaPos[1]));
				n[index] += normal;
				n[indices[i + 1]] += normal;
				n[indices[i + 2]] += normal;
			}
			for (int i = 0; i < vertices.size(); i++) n[i] = normalize(n[i]);
			this->normals = n;
			if (!validation())
			{
				TE_DEBUG_ERROR("Indices should ilclude all vertices");
			}
		}
	}

	void Mesh::removeDoubles() 
	{
		//this->updateAABB();
		//AAQTree<GLuint> aaqtree(this->aabb.from, this->aabb.to, 8, 64);
		//for (int i = 0; i < this->vertices.size(); i++)
		//{
		//	if (!aaqtree.addItem(i, AABB(vertices[i] - vec3(0.0005f), vec3(0.001f))))
		//		int a = 0;
		//}
		//
		//vector<AAQTree<GLuint>> tree = aaqtree.getTree();
		//
		//vector<bool> skip_verices(this->vertices.size(), false);
		//vector<GLuint> new_indices(this->vertices.size(), 0);
		//vector<vec3> new_vertices;
		//vector<vec2> new_UVs;
		//vector<vec3> new_normals;
		//for (int i = 0; i < tree.size(); i++) 
		//{
		//	vector<GLuint> verticeIndices = tree[i].getItems();
		//	for (int j = 0; j < verticeIndices.size(); j++)
		//	{
		//		if (!skip_verices[verticeIndices[j]])
		//		{
		//			for (int l = j + 1; l < verticeIndices.size(); l++)
		//			{
		//				if (
		//					vertices[verticeIndices[j]] == vertices[verticeIndices[l]] &&
		//					UVs[verticeIndices[j]] == UVs[verticeIndices[l]] &&
		//					normals[verticeIndices[j]] == normals[verticeIndices[l]]
		//					)
		//				{
		//					skip_verices[verticeIndices[l]] = true;
		//					new_indices[verticeIndices[l]] = new_vertices.size();
		//				}
		//			}
		//			new_indices[verticeIndices[j]] = new_vertices.size();
		//			new_vertices.push_back(vertices[verticeIndices[j]]);
		//			new_UVs.push_back(UVs[verticeIndices[j]]);
		//			new_normals.push_back(normals[verticeIndices[j]]);
		//			skip_verices[verticeIndices[j]] = true;
		//		}
		//	}
		//}
		//
		//
		//for (int i = 0; i < this->indices.size(); i++) 
		//{
		//	this->indices[i] = new_indices[this->indices[i]];
		//}
		//
		//this->vertices = new_vertices;
		//this->UVs = new_UVs;
		//this->normals = new_normals;
	}


	void Mesh::splitTriangles()
	{
		std::vector<vec3> p, n, t;
		std::vector<vec2> uv;
		std::vector<vec4> c;
		std::vector<int> i;
		for (int j = 0; j < this->indices.size(); j++)
		{
			i.push_back(j);
			int ind = this->indices[j];
			p.push_back(this->vertices[ind]);
			if (!this->normals.empty())n.push_back(this->normals[ind]);
			if (!this->tangents.empty())t.push_back(this->tangents[ind]);
			if (!this->UVs.empty())uv.push_back(this->UVs[ind]);
			if (!this->colors.empty())c.push_back(this->colors[ind]);
		}
		this->vertices = p;
		this->normals = n;
		this->tangents = t;
		this->UVs = uv;
		this->colors = c;
	}

	void Mesh::addLODIndex(unsigned int index) 
	{
		LODs.push_back(index);
	}

	void Mesh::popLODIndex() 
	{
		LODs.pop_back();
	}

	void Mesh::setLODIndex(unsigned int LOD, unsigned int index) 
	{
		LODs[LOD] = index;
	}

	//O
	vector<vec3> Mesh::getVertices() { return vertices; }
	vector<vec3> Mesh::getNormals() { return normals; }
	vector<vec3> Mesh::getTangents() { return tangents; }
	vector<vec2> Mesh::getUVs() { return UVs; }
	vector<vec4> Mesh::getColors() { return colors; }
	vector<unsigned int> Mesh::getIndices() { return indices; }
	vector<vec3> Mesh::getTriangles() 
	{
		vector<vec3> t;
		for (unsigned int i : this->indices)
			t.push_back(vertices[i]);
		return t;
	}
	SharedMesh* Mesh::sharedMesh()
	{
		return this;
	}
	GLuint Mesh::getLength()
	{
		return indices.size();
	};

	Mesh::~Mesh()
	{
		if(EBO)
			glDeleteBuffers(1, &EBO);
	}
}