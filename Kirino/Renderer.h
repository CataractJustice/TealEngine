#pragma once
#include <initializer_list>
#include <vector>
#include <set>
#include <utility>
#include "MeshInstance.h"
#include "TextureBindings.h"
#include "UBOBindings.h"
#include "Material.h"
namespace Kirino 
{
	struct DrawElementsIndirectCommand {
		unsigned int  count;
		unsigned int  instanceCount;
		unsigned int  firstIndex;
		unsigned int  baseVertex;
		unsigned int  baseInstance;
	};

	class Renderer
	{
		private:
		struct DrawEntry 
		{
			std::vector<DrawElementsIndirectCommand> commands;
			MeshInstance meshInstance;
			unsigned int indirectBuffer;
		};
		TextureBindings textureBindings;
		UBOBindings uboBindings;
		struct DrawEntryComparator 
		{
			inline bool operator()(const DrawEntry& lvp, const DrawEntry& rvp) const 
			{
				const MeshInstance& lv = lvp.meshInstance;
				const MeshInstance& rv = rvp.meshInstance;
				/*
				1 - sort by shader used so
				2 - sort by material used
				4 - sort by vao
				3 - sort by mode used
				*/
				if(lv.material->getShaderProgram()->getId() != rv.material->getShaderProgram()->getId()) return lv.material->getShaderProgram()->getId() > rv.material->getShaderProgram()->getId();
				if(lv.material != rv.material) return lv.material > rv.material;
				if(*lv.mesh->vao != *rv.mesh->vao) return *lv.mesh->vao > *rv.mesh->vao;
				return lv.mode > rv.mode;
			}
		};
		std::set<DrawEntry, DrawEntryComparator> drawEntries;
		std::vector<DrawEntry> drawEntriesVectorCache;
		bool dirtyDrawEntriesVectorCache;
		public:
		Renderer();
		void emplace(const MeshInstance& instance);
		void erase(const MeshInstance& instance);
		
		void render(Material* material = nullptr);
	};
}