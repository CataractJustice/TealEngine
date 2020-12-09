#pragma once
#define GLEW_STATIC
#include "MeshUtil.h"

namespace TealEngine
{
	namespace BasicMeshes {
		Mesh square, cube;

		namespace Cube
		{

			Mesh getCubeFace(CubeFace face)
			{
				Mesh faceMesh;
				faceMesh.setIndices({ 2, 1, 0, 0, 3, 2 });
				faceMesh.setUVs({ vec2(0.0f, 0.0f), vec2(1.0f, 0.0f), vec2(1.0f, 1.0f), vec2(0.0f, 1.0f) });
				switch (face)
				{
				case LEFT:
					faceMesh.setVertices({ vec3(-1.0, -1.0, 1.0), vec3(-1.0, -1.0, -1.0), vec3(-1.0, 1.0, -1.0), vec3(-1.0, 1.0, 1.0) });
					faceMesh.setNormals(vector<vec3>(4, vec3(-1.0f, 0.0f, 0.0f)));
					break;
				case RIGHT:
					faceMesh.setVertices({ vec3(1.0, -1.0, -1.0), vec3(1.0, -1.0, 1.0), vec3(1.0, 1.0, 1.0), vec3(1.0, 1.0, -1.0) });
					faceMesh.setNormals(vector<vec3>(4, vec3(1.0f, 0.0f, 0.0f)));
					break;
				case TOP:
					faceMesh.setVertices({ vec3(1.0, 1.0, -1.0), vec3(1.0, 1.0, 1.0), vec3(-1.0, 1.0, 1.0), vec3(-1.0, 1.0, -1.0) });
					faceMesh.setNormals(vector<vec3>(4, vec3(0.0f, 1.0f, 0.0f)));
					break;
				case BOTTOM:
					faceMesh.setVertices({ vec3(-1.0, -1.0, 1.0), vec3(1.0, -1.0, 1.0), vec3(1.0, -1.0, -1.0), vec3(-1.0, -1.0, -1.0) });
					faceMesh.setNormals(vector<vec3>(4, vec3(0.0f, -1.0f, 0.0f)));
					break;
				case FRONT:
					faceMesh.setVertices({ vec3(1.0, -1.0, 1.0), vec3(-1.0, -1.0, 1.0), vec3(-1.0, 1.0, 1.0), vec3(1.0, 1.0, 1.0) });
					faceMesh.setNormals(vector<vec3>(4, vec3(0.0f, 0.0f, 1.0f)));
					break;
				case BACK:
					faceMesh.setVertices({ vec3(-1.0, -1.0, -1.0), vec3(1.0, -1.0, -1.0), vec3(1.0, 1.0, -1.0), vec3(-1.0, 1.0, -1.0) });
					faceMesh.setNormals(vector<vec3>(4, vec3(0.0f, 0.0f, -1.0f)));
					break;
				}

				return faceMesh;
			}
		}

		void init()
		{
			square.enableAllAttribs();
			square.addMesh(Cube::getCubeFace(Cube::BACK));
			square.apply();

			cube.enableAllAttribs();
			cube.addMesh(Cube::getCubeFace(Cube::LEFT));
			cube.addMesh(Cube::getCubeFace(Cube::RIGHT));
			cube.addMesh(Cube::getCubeFace(Cube::BOTTOM));
			cube.addMesh(Cube::getCubeFace(Cube::TOP));
			cube.addMesh(Cube::getCubeFace(Cube::FRONT));
			cube.addMesh(Cube::getCubeFace(Cube::BACK));
			cube.apply();
		}
	}
}