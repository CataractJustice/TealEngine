#pragma once
#include "GameNode/Component.h"
#include "Kirino/Mesh.h"
#include "Kirino/Material.h"
#include "Kirino/MeshInstance.h"
#include "Kirino/Renderer.h"
#include <vector>
#include "glm/glm.hpp"
#include "Graphics/Camera.h"
#include <set>
#include <map>

namespace TealEngine{
	class Mesh3DRendererInstancedList;

	class Mesh3DRendererInstanced : public Component 
	{ 	
		private:
		const Kirino::Material* material;
		const Kirino::Mesh* mesh;
		unsigned int modifyFrame;
		unsigned int hash;
		public:
		void onChange();
		unsigned int instanceIndex;
		unsigned int baseInstance;
		Mesh3DRendererInstanced();
		~Mesh3DRendererInstanced();
		void onAttach() override;
		void onPropSet(const std::string& propName) override;
		void setMaterial(const Kirino::Material* material);
		void setMesh(const Kirino::Mesh* mesh);
		inline const Kirino::Mesh* getMesh() const { return mesh; };
		inline const Kirino::Material* getMaterial() const { return material; };
		inline unsigned int getHash() const { return hash; }
		inline unsigned int getModFrame() const { return modifyFrame; }
	};

	

	class Mesh3DRendererInstancedList 
	{
		private:
		Kirino::Renderer kRenderer;
		#pragma pack(push, 1)
		struct InstanceData 
		{
			glm::mat4 model;
			glm::mat3 normalModel;
		};
		#pragma pack(pop)
		struct BlockBaseData 
		{
			unsigned int instanceCount;
			unsigned int baseIndex;
			int lastTransform;
			bool dirtyMatrix;
			Mesh3DRendererInstanced* renderer;
			Kirino::Mesh mesh;
			const Kirino::Material* material;
		};

		std::map<Kirino::VertexArrayObject*, Kirino::VertexArrayObject> vboPtrToModVboMap;
		std::map<Mesh3DRendererInstanced*, unsigned int> rendererToInstanceIndex;
		std::map<Mesh3DRendererInstanced*, unsigned int> rendererHashes;
		std::vector<BlockBaseData> blockBases;
		std::vector<InstanceData> instanceMatrixBufferVector;
		unsigned int instanceMatrixGLBuffer;
		unsigned int instanceMatrixBufferSize;
		
		std::set<Mesh3DRendererInstanced*> unactiveRendererSet;
		std::set<Mesh3DRendererInstanced*> activeRendererSet; 

		void instanceInsertMeshRenderer(Mesh3DRendererInstanced* renderer);
		void instanceRemoveMeshRenderer(Mesh3DRendererInstanced* renderer);
				                                                                                                                             
		public:
		void uploadDirtyMatrices();
		Mesh3DRendererInstancedList(unsigned int instanceMatrixBufferSize = 65536);
		void addMeshRenderer(Mesh3DRendererInstanced* renderer);
		void removeMeshRenderer(Mesh3DRendererInstanced* renderer);
		void updateList();
		void setCamera(Camera* camera);
		void render(Kirino::Material* material = nullptr);
		void init();
	};
}