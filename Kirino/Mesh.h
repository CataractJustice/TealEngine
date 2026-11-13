#pragma once
#include "VertexArrayObject.h"
namespace Kirino
{
	struct Mesh
	{
		unsigned int vertexOffset, indexOffset, indexCount;
		struct BoundingBox 
		{
			float minX, minY, minZ, maxX, maxY, maxZ;
		};
		BoundingBox boundingBox;
		VertexArrayObject* vao;
	};
}
