#include "InstancedRenderer.h"
#include "GameNode/GameNode3D.h"
#include "Core.h"
/*namespace TealEngine 
{
	unsigned int Mesh3DInstancesRenderer::addInstance(Mesh3DInstance* instance) 
	{
		this->instances.push_back(instance);
		return instances.size() - 1;
	}
	void Mesh3DInstancesRenderer::removeInstance(unsigned int instanceId) 
	{
		this->instances[instanceId] = instances.back();
		this->instances.pop_back();
	}

	void Mesh3DInstancesRenderer::render() 
	{
		for(int i = 0; i < instances.size(); i++) 
		{
			//Transform& t = instances[i]->getParentOfType<GameNode3D>()->getWorldTransform();
			matrixData[i].model = t.getMatrix();
			matrixData[i].normalModel = t.getNormalsModel();
		}
		//glBindBuffer(GL_ARRAY_BUFFER, instanceMatrixGLBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Mesh3DInstancesRenderer::InstanceData) * matrixData.size(), nullptr, GL_STATIC_DRAW);
	}

	Mesh3DInstancesRenderer::Mesh3DInstancesRenderer() 
	{
		//glGenBuffers(1, &instanceMatrixGLBuffer);
	}
}*/