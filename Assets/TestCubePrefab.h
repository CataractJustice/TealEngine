#pragma once
#include "../GameNode/Prefab/Prefab.h"
#include "../GameNode/GameNode3D.h"
#include "../Graphics/Mesh/Mesh.h"
#include "../Graphics/Mesh/MeshRenderer.h"
#include "../Graphics/Mesh/MeshUtil.h"
#include "../Resources.h"
namespace TealEngine 
{
	class TestCubePrefab : public Prefab
	{
	public:
		static Entity* create(unsigned int serverSide, TStruct* params)
		{
			Entity* node = new Entity();
			node->rename("TestCube");
			SharedMesh* mesh = new SharedMesh(*BasicMeshes::cube.sharedMesh());
			Mesh3DRenderer* meshRenderer = new Mesh3DRenderer();
			if (!serverSide)
			{
				ShaderProgram* shader;
				shader = new ShaderProgram(Resources::getMaterial("testShader"));
				meshRenderer->setShader(shader);
				mesh->addChild(meshRenderer);
				node->addChild(mesh);
			}

			if (params) 
			{
				if (params->getFieldsCount() > 0)
				{
					Transform transform;
					transform.setPosition(glm::vec3(params->getFieldValue<float>(0), params->getFieldValue<float>(1), params->getFieldValue<float>(2)));
					node->setTransform(transform);
				}
			}
			return node;
		}
	};
}