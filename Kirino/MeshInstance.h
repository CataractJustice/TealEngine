#pragma once
#include "Mesh.h"
#include "Material.h"
namespace Kirino
{
	struct MeshInstance
	{
		Mesh *mesh;
		const Material *material;
		unsigned int mode;
		unsigned int baseInstance;
	};
} // namespace Kirino
