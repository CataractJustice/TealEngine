#pragma once
#include "Mesh.h"

namespace TealEngine
{
	namespace BasicMeshes {
		extern Mesh square;
		extern Mesh cube;

		namespace Cube
		{
			enum CubeFace
			{
				RIGHT,
				LEFT,
				TOP,
				BOTTOM,
				FRONT,
				BACK
			};
			Mesh getCubeFace(CubeFace face);
		}

		void init();
	}
}