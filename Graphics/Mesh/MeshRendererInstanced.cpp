#include <GL/glew.h>
#include "MeshRendererInstanced.h"
#include "GameNode/GameNode3D.h"
#include "Core.h"

namespace TealEngine 
{
	unsigned int Mesh3DRendererInstancedListRendererHash(const Mesh3DRendererInstanced* renderer) 
	{
		const Kirino::Mesh& mesh = *renderer->getMesh();
		const Kirino::Material& material = *renderer->getMaterial();
		const Kirino::Material* matptr = &material;
		//sum fields multiplied by random numbers
		unsigned int hash = 
		mesh.indexCount * 0x8f3af412
		+ mesh.indexOffset * 0xaf4e885d
		+ *mesh.vao * 0x64359812
		+ mesh.vertexOffset * 0xfaded420
		+ (*(unsigned int*)&matptr) * 0x4418dead
		+ material.getShaderProgram()->getId() * 0x413612ff; 
		return hash;
	}
	
	Mesh3DRendererInstanced::Mesh3DRendererInstanced() 
	{
		this->addProp(new StringKeyMapValuePointerProp<Kirino::Material>(&material, &Core::materialsManager.getLoadedMap()), "Material");
		this->addProp(new StringKeyMapValuePointerProp<Kirino::Mesh>(&mesh, &Core::modelsManager.getLoadedMap()), "Mesh");
		mesh = nullptr;
		material = nullptr;
	}

	void Mesh3DRendererInstanced::onAttach() 
	{

	}

	Mesh3DRendererInstanced::~Mesh3DRendererInstanced() 
	{
	}

	void Mesh3DRendererInstanced::onPropSet(const std::string& propName) 
	{
		onChange();
	}

	void Mesh3DRendererInstanced::setMaterial(const Kirino::Material* material) 
	{
		this->material = material;
		onChange();
	}
	void Mesh3DRendererInstanced::setMesh(const Kirino::Mesh* mesh) 
	{
		this->mesh = mesh;
		onChange();
	}

	void Mesh3DRendererInstanced::onChange() 
	{
//		if(mesh && mesh->indexCount == 0) mesh = nullptr;
		if(mesh && material && mesh->vao) 
		{
			hash = Mesh3DRendererInstancedListRendererHash(this);
			modifyFrame = Core::getFrameId();
		}
	}

	bool compareMeshRenderers(const Mesh3DRendererInstanced* lv, const Mesh3DRendererInstanced* rv) 
	{
		const Kirino::Mesh& lmesh = *lv->getMesh();
		const Kirino::Material& lmaterial = *lv->getMaterial();
		const Kirino::Mesh& rmesh = *rv->getMesh();
		const Kirino::Material& rmaterial = *rv->getMaterial();
		return 
			lmesh.indexCount == rmesh.indexCount &&
			lmesh.indexOffset == rmesh.indexOffset &&
			*lmesh.vao == *rmesh.vao &&
			lmesh.vertexOffset == rmesh.vertexOffset &&
			&lmaterial == &rmaterial; 
		;
	}

	Mesh3DRendererInstancedList::Mesh3DRendererInstancedList(unsigned int instanceMatrixBufferSize) : instanceMatrixBufferSize(instanceMatrixBufferSize) 
	{
		blockBases = std::vector<BlockBaseData>(instanceMatrixBufferSize);
		instanceMatrixBufferVector = std::vector<InstanceData>(instanceMatrixBufferSize);
	}

	void Mesh3DRendererInstancedList::init() 
	{
		glGenBuffers(1, &instanceMatrixGLBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, instanceMatrixGLBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Mesh3DRendererInstancedList::InstanceData) * instanceMatrixBufferSize, nullptr, GL_STATIC_DRAW);
	}

	void Mesh3DRendererInstancedList::instanceInsertMeshRenderer(Mesh3DRendererInstanced* renderer) 
	{
		/*
			FIX ME:
			this code will co into infinite loop if there's no free spaces
			Fix it later and remove this comment
		*/
		unsigned int baseInstance = renderer->getHash() % instanceMatrixBufferSize;
		baseInstance = blockBases[baseInstance].baseIndex;
		//search for a instance block with same mesh and material that also has a free space next to it
		//or for a free space
		while(blockBases[baseInstance].renderer && (!compareMeshRenderers(blockBases[baseInstance].renderer, renderer) || blockBases[baseInstance + blockBases[baseInstance].instanceCount].renderer != nullptr)) 
		{
			baseInstance += blockBases[baseInstance].instanceCount;
			baseInstance %= instanceMatrixBufferSize;
		}
		//if it is not nullptr that means we found instance block with the same mesh and materal as inserting renderer and it has a free space at base + count
		if(blockBases[baseInstance].renderer != nullptr) 
		{
			//found an empty place right next to an existing instance block
			blockBases[baseInstance + blockBases[baseInstance].instanceCount].renderer = renderer;
			blockBases[baseInstance + blockBases[baseInstance].instanceCount].baseIndex = baseInstance;
			blockBases[baseInstance + blockBases[baseInstance].instanceCount].dirtyMatrix = true;
			blockBases[baseInstance + blockBases[baseInstance].instanceCount].lastTransform = -1;
			blockBases[baseInstance + blockBases[baseInstance].instanceCount].material = renderer->getMaterial();
			blockBases[baseInstance + blockBases[baseInstance].instanceCount].mesh = *renderer->getMesh();
			blockBases[baseInstance + blockBases[baseInstance].instanceCount].mesh.vao = renderer->getMesh()->vao;
			rendererToInstanceIndex[renderer] = baseInstance + blockBases[baseInstance].instanceCount;
			
			Kirino::MeshInstance instance;
			instance.baseInstance = baseInstance;
			instance.material = blockBases[baseInstance + blockBases[baseInstance].instanceCount].material;
			instance.mesh = &blockBases[baseInstance + blockBases[baseInstance].instanceCount].mesh;
			instance.mode = GL_TRIANGLES;
			kRenderer.emplace(instance);
			blockBases[baseInstance].instanceCount++;
		}
		//Else we have not found any instance blocks with same mesh and materials with the free space next to it and we can start a new block 
		else 
		{
			blockBases[baseInstance].renderer = renderer;
			blockBases[baseInstance].mesh = *renderer->getMesh();
			blockBases[baseInstance].mesh.vao = renderer->getMesh()->vao;
			blockBases[baseInstance].material = renderer->getMaterial();
			blockBases[baseInstance].baseIndex = baseInstance;
			blockBases[baseInstance].dirtyMatrix = true;
			blockBases[baseInstance].lastTransform = -1;
			blockBases[baseInstance].instanceCount = 1;
			rendererToInstanceIndex[renderer] = baseInstance;

			Kirino::MeshInstance instance;
			instance.baseInstance = baseInstance;
			instance.material = renderer->getMaterial();
			instance.mesh = &blockBases[baseInstance].mesh;
			instance.mode = GL_TRIANGLES;
			kRenderer.emplace(instance);
		}
 	}
	void Mesh3DRendererInstancedList::instanceRemoveMeshRenderer(Mesh3DRendererInstanced* renderer) 
	{
		unsigned int index = rendererToInstanceIndex[renderer];
		unsigned int baseIndex = blockBases[index].baseIndex;
		rendererToInstanceIndex.erase(renderer);
		//erase one mesh from kirino renderer
		Kirino::MeshInstance instance;
		instance.baseInstance = baseIndex;
		instance.material = blockBases[baseIndex].material;
		instance.mesh = &blockBases[baseIndex].mesh;
		instance.mode = GL_TRIANGLES;
		kRenderer.erase(instance);
		//replace removed instance with the last instance in the block
		if(blockBases[baseIndex].instanceCount > 1) 
		{
			blockBases[index].dirtyMatrix = true;
			blockBases[index].lastTransform = -1;
			blockBases[index].mesh = blockBases[baseIndex + blockBases[baseIndex].instanceCount - 1].mesh;
			blockBases[index].renderer = blockBases[baseIndex + blockBases[baseIndex].instanceCount - 1].renderer;
			rendererToInstanceIndex[blockBases[baseIndex + blockBases[baseIndex].instanceCount - 1].renderer] = index;
		}
		blockBases[baseIndex + blockBases[baseIndex].instanceCount - 1].dirtyMatrix = false;
		blockBases[baseIndex + blockBases[baseIndex].instanceCount - 1].lastTransform = -1;
		blockBases[baseIndex + blockBases[baseIndex].instanceCount - 1].renderer = nullptr;
		blockBases[baseIndex].instanceCount--;
	}

	void Mesh3DRendererInstancedList::addMeshRenderer(Mesh3DRendererInstanced* renderer) 
	{
		this->unactiveRendererSet.emplace(renderer);
		if(renderer->getMaterial() && renderer->getMesh()) 
		{
			this->rendererHashes[renderer] = renderer->getHash();
		}
		//copy vbo and add pointers to instance data
		if(renderer->getMaterial() && renderer->getMesh()) 
		{
			//vboPtrToModVboMap[renderer->getMesh()->vao] = *renderer->getMesh()->vao;
			Kirino::VertexArrayObject& vao = *renderer->getMesh()->vao;//vboPtrToModVboMap[renderer->getMesh()->vao];
			vao.vertexAttribPointer(Kirino::VertexArrayObject::VertexAttribPointer(true, this->instanceMatrixGLBuffer, 9, 4, GL_FLOAT, false, sizeof(InstanceData), 0, 1));
			vao.vertexAttribPointer(Kirino::VertexArrayObject::VertexAttribPointer(true, this->instanceMatrixGLBuffer, 10, 4, GL_FLOAT, false, sizeof(InstanceData), (void*)(sizeof(float) * 4), 1));
			vao.vertexAttribPointer(Kirino::VertexArrayObject::VertexAttribPointer(true, this->instanceMatrixGLBuffer, 11, 4, GL_FLOAT, false, sizeof(InstanceData), (void*)(sizeof(float) * 8), 1));
			vao.vertexAttribPointer(Kirino::VertexArrayObject::VertexAttribPointer(true, this->instanceMatrixGLBuffer, 12, 4, GL_FLOAT, false, sizeof(InstanceData), (void*)(sizeof(float) * 12), 1));
			vao.vertexAttribPointer(Kirino::VertexArrayObject::VertexAttribPointer(true, this->instanceMatrixGLBuffer, 13, 3, GL_FLOAT, false, sizeof(InstanceData), (void*)(sizeof(float) * 16), 1));
			vao.vertexAttribPointer(Kirino::VertexArrayObject::VertexAttribPointer(true, this->instanceMatrixGLBuffer, 14, 3, GL_FLOAT, false, sizeof(InstanceData), (void*)(sizeof(float) * 19), 1));
			vao.vertexAttribPointer(Kirino::VertexArrayObject::VertexAttribPointer(true, this->instanceMatrixGLBuffer, 15, 3, GL_FLOAT, false, sizeof(InstanceData), (void*)(sizeof(float) * 22), 1));
			vao.updateAttribs();
		}
	}

	void Mesh3DRendererInstancedList::removeMeshRenderer(Mesh3DRendererInstanced* renderer) 
	{
		this->unactiveRendererSet.erase(renderer);
		if(activeRendererSet.find(renderer) != activeRendererSet.cend()) 
		{
			instanceRemoveMeshRenderer(renderer);
		}
		this->activeRendererSet.erase(renderer);
		this->rendererHashes.erase(renderer);
	}

	void Mesh3DRendererInstancedList::updateList() 
	{
		
		std::vector<Mesh3DRendererInstanced*> newActive;
		for(Mesh3DRendererInstanced* renderer : unactiveRendererSet) 
		{
			bool isActive = renderer->isActive();
			isActive = isActive && renderer->getMaterial() && renderer->getMesh();
			isActive = isActive && renderer->getMesh()->indexCount;
			//fix me:
			//add another list of renderers with no data for this block
			if(isActive) 
			{
				this->rendererHashes[renderer] = renderer->getHash();
				//vboPtrToModVboMap[renderer->getMesh()->vao] = *renderer->getMesh()->vao;
				Kirino::VertexArrayObject& vao = *renderer->getMesh()->vao;
				vao.vertexAttribPointer(Kirino::VertexArrayObject::VertexAttribPointer(true, this->instanceMatrixGLBuffer, 9, 4, GL_FLOAT, false, sizeof(InstanceData), 0, 1));
				vao.vertexAttribPointer(Kirino::VertexArrayObject::VertexAttribPointer(true, this->instanceMatrixGLBuffer, 10, 4, GL_FLOAT, false, sizeof(InstanceData), (void*)(sizeof(float) * 4), 1));
				vao.vertexAttribPointer(Kirino::VertexArrayObject::VertexAttribPointer(true, this->instanceMatrixGLBuffer, 11, 4, GL_FLOAT, false, sizeof(InstanceData), (void*)(sizeof(float) * 8), 1));
				vao.vertexAttribPointer(Kirino::VertexArrayObject::VertexAttribPointer(true, this->instanceMatrixGLBuffer, 12, 4, GL_FLOAT, false, sizeof(InstanceData), (void*)(sizeof(float) * 12), 1));
				vao.vertexAttribPointer(Kirino::VertexArrayObject::VertexAttribPointer(true, this->instanceMatrixGLBuffer, 13, 3, GL_FLOAT, false, sizeof(InstanceData), (void*)(sizeof(float) * 16), 1));
				vao.vertexAttribPointer(Kirino::VertexArrayObject::VertexAttribPointer(true, this->instanceMatrixGLBuffer, 14, 3, GL_FLOAT, false, sizeof(InstanceData), (void*)(sizeof(float) * 19), 1));
				vao.vertexAttribPointer(Kirino::VertexArrayObject::VertexAttribPointer(true, this->instanceMatrixGLBuffer, 15, 3, GL_FLOAT, false, sizeof(InstanceData), (void*)(sizeof(float) * 22), 1));
				vao.updateAttribs();
			}
			if(isActive)
			{
				newActive.push_back(renderer);
				instanceInsertMeshRenderer(renderer);
			}
		}
		std::vector<Mesh3DRendererInstanced*> newUnactive;
		for(Mesh3DRendererInstanced* renderer : activeRendererSet) 
		{
			bool isActive = renderer->isActive();
			isActive = isActive && renderer->getMaterial() && renderer->getMesh();
			if(!isActive) 
			{
				unactiveRendererSet.emplace(renderer);
				instanceRemoveMeshRenderer(renderer);
				newUnactive.push_back(renderer);
			}
			else if((Core::getFrameId() - renderer->getModFrame() <= 1) && rendererHashes[renderer] != renderer->getHash())
			{
				instanceRemoveMeshRenderer(renderer);
				rendererHashes[renderer] = renderer->getHash();
				if(vboPtrToModVboMap.find(renderer->getMesh()->vao) == vboPtrToModVboMap.cend()) 
				{
					//vboPtrToModVboMap[renderer->getMesh()->vao] = *renderer->getMesh()->vao;
					Kirino::VertexArrayObject& vao = *renderer->getMesh()->vao;//vboPtrToModVboMap[renderer->getMesh()->vao];
					vao.vertexAttribPointer(Kirino::VertexArrayObject::VertexAttribPointer(true, this->instanceMatrixGLBuffer, 9, 4, GL_FLOAT, false, sizeof(InstanceData), 0, 1));
					vao.vertexAttribPointer(Kirino::VertexArrayObject::VertexAttribPointer(true, this->instanceMatrixGLBuffer, 10, 4, GL_FLOAT, false, sizeof(InstanceData), (void*)(sizeof(float) * 4), 1));
					vao.vertexAttribPointer(Kirino::VertexArrayObject::VertexAttribPointer(true, this->instanceMatrixGLBuffer, 11, 4, GL_FLOAT, false, sizeof(InstanceData), (void*)(sizeof(float) * 8), 1));
					vao.vertexAttribPointer(Kirino::VertexArrayObject::VertexAttribPointer(true, this->instanceMatrixGLBuffer, 12, 4, GL_FLOAT, false, sizeof(InstanceData), (void*)(sizeof(float) * 12), 1));
					vao.vertexAttribPointer(Kirino::VertexArrayObject::VertexAttribPointer(true, this->instanceMatrixGLBuffer, 13, 3, GL_FLOAT, false, sizeof(InstanceData), (void*)(sizeof(float) * 16), 1));
					vao.vertexAttribPointer(Kirino::VertexArrayObject::VertexAttribPointer(true, this->instanceMatrixGLBuffer, 14, 3, GL_FLOAT, false, sizeof(InstanceData), (void*)(sizeof(float) * 19), 1));
					vao.vertexAttribPointer(Kirino::VertexArrayObject::VertexAttribPointer(true, this->instanceMatrixGLBuffer, 15, 3, GL_FLOAT, false, sizeof(InstanceData), (void*)(sizeof(float) * 22), 1));
					vao.updateAttribs();
				}
				instanceInsertMeshRenderer(renderer);
			}
		}
		for(Mesh3DRendererInstanced* renderer : newActive) 
		{
			this->activeRendererSet.emplace(renderer);
			this->unactiveRendererSet.erase(renderer);
		}
		for(Mesh3DRendererInstanced* renderer : newUnactive) 
		{
			this->activeRendererSet.erase(renderer);
		}
	}

	void Mesh3DRendererInstancedList::uploadDirtyMatrices() 
	{
		int maxGap = 4;

		for(const auto[renderer, index] : rendererToInstanceIndex) 
		{
			int start = index;
			int size = 0;
			int gap = maxGap;
			for(int i = 0; i < instanceMatrixBufferSize - index; i++) 
			{
				gap++;
				if(blockBases[index + i].renderer == nullptr) 
				{
					if(gap > maxGap) break;
					continue;
				}
				const Transform& t = ((GameNode3D*)blockBases[index + i].renderer->getParent())->getWorldTransform();
				if(t.getLastModifyStamp() != blockBases[index + i].lastTransform) 
				{
					instanceMatrixBufferVector[index + i].model = t.getMatrix();
					instanceMatrixBufferVector[index + i].normalModel = glm::mat3(t.getNormalsModel());
					blockBases[index + i].lastTransform = t.getLastModifyStamp();
					gap = 0;
					size++;
				}
				else 
				{
					if(gap > maxGap) break;
					continue;
				}
			}
			if(size) 
			{
				glBufferSubData(GL_ARRAY_BUFFER, sizeof(InstanceData) * start, sizeof(InstanceData) * size, instanceMatrixBufferVector.data() + start);	
			}
		}
	}

	void Mesh3DRendererInstancedList::render(Kirino::Material* material) 
	{
		Profiler::ProfilerPoint renderProf(&Core::profiler, "Kirino render");
		kRenderer.render(material);
	}
}