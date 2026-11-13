#include "Renderer.h"
#include <unordered_map>
#include <GL/glew.h>
#include <iostream>


namespace Kirino
{
	Renderer::Renderer() 
	{
		dirtyDrawEntriesVectorCache = false;
	}

	void Renderer::emplace(const MeshInstance& instance) 
	{
		if(instance.mesh->indexCount == 0) return;
		dirtyDrawEntriesVectorCache = true;
		DrawEntry entry;
		entry.meshInstance = instance;
		auto it = this->drawEntries.find(entry);
		if(it != drawEntries.cend())
		{
			DrawEntry* mutableIt = const_cast<DrawEntry*>(&(*it));
			int commandIndex = -1;
			for(int i = 0; i < it->commands.size(); i++) 
			{
				if(
					it->commands[i].baseInstance == instance.baseInstance &&
					it->commands[i].baseVertex == instance.mesh->vertexOffset &&
					it->commands[i].count == instance.mesh->indexCount &&
					it->commands[i].firstIndex == instance.mesh->indexOffset
				) 
				{
					commandIndex = i;
					break;
				}
			}
			glBindBuffer(GL_DRAW_INDIRECT_BUFFER, it->indirectBuffer);
			if(commandIndex != -1) 
			{
				mutableIt->commands[commandIndex].instanceCount++;
				glBufferSubData(GL_DRAW_INDIRECT_BUFFER, sizeof(DrawElementsIndirectCommand) * commandIndex, sizeof(DrawElementsIndirectCommand), &it->commands[commandIndex]);
			}
			else 
			{
				DrawElementsIndirectCommand command;
				command.baseInstance = instance.baseInstance;
				command.baseVertex = instance.mesh->vertexOffset;
				command.count = instance.mesh->indexCount;
				command.firstIndex = instance.mesh->indexOffset;
				command.instanceCount = 1;
				mutableIt->commands.push_back(command);
				glBufferData(GL_DRAW_INDIRECT_BUFFER, sizeof(DrawElementsIndirectCommand) * it->commands.size(), it->commands.data(), GL_STATIC_DRAW);
			}
		}
		else 
		{
			DrawEntry drawEntry;
			glGenBuffers(1, &drawEntry.indirectBuffer);
			drawEntry.meshInstance = instance;
			drawEntry.commands = {};
			DrawElementsIndirectCommand command;
			command.baseInstance = instance.baseInstance;
			command.baseVertex = instance.mesh->vertexOffset;
			command.count = instance.mesh->indexCount;
			command.firstIndex = instance.mesh->indexOffset;
			command.instanceCount = 1;
			drawEntry.commands.push_back(command);
			glBindBuffer(GL_DRAW_INDIRECT_BUFFER, drawEntry.indirectBuffer);
			glBufferData(GL_DRAW_INDIRECT_BUFFER, sizeof(DrawElementsIndirectCommand) * drawEntry.commands.size(), drawEntry.commands.data(), GL_STATIC_DRAW);
			drawEntries.emplace(drawEntry);
		}
	}

	void Renderer::erase(const MeshInstance& instance) 
	{
		dirtyDrawEntriesVectorCache = true;
		DrawEntry entry;
		entry.meshInstance = instance;
		auto it = this->drawEntries.find(entry);
		if(it != drawEntries.cend())
		{
			DrawEntry* mutableIt = const_cast<DrawEntry*>(&(*it));
			for(int i = 0; i < it->commands.size(); i++) 
			{
				if(
					it->commands[i].baseInstance == instance.baseInstance &&
					it->commands[i].baseVertex == instance.mesh->vertexOffset &&
					it->commands[i].count == instance.mesh->indexCount &&
					it->commands[i].firstIndex == instance.mesh->indexOffset
				) 
				{
					mutableIt->commands[i].instanceCount--;
					glBindBuffer(GL_DRAW_INDIRECT_BUFFER, it->indirectBuffer);
					if(it->commands[i].instanceCount == 0) 
					{
						mutableIt->commands.erase(mutableIt->commands.begin() + i);
						if(it->commands.size() == 0) 
						{
							glDeleteBuffers(1, &it->indirectBuffer);
							drawEntries.erase(entry);
							return;
						}
						else 
						{
							glBufferData(GL_DRAW_INDIRECT_BUFFER, sizeof(DrawElementsIndirectCommand) * it->commands.size(), it->commands.data(), GL_STATIC_DRAW);
							return;
						}
					}
					else 
					{
						glBufferSubData(GL_DRAW_INDIRECT_BUFFER, sizeof(DrawElementsIndirectCommand) * i, sizeof(DrawElementsIndirectCommand), &it->commands[i]);
						return;
					}
					break;
				}
			}
		}
	}

	void Renderer::render(Material* material) 
	{
		uboBindings.reset();
		textureBindings.reset();
		int lastVao = -1;
		if(material) 
		{
			material->useProgram();
			material->setupUniforms();
			material->setupUniformBlocks(uboBindings);
			material->setupTextures(textureBindings);
		}
		const Material* lastMaterial = nullptr;
		if(dirtyDrawEntriesVectorCache) 
		{
			drawEntriesVectorCache.clear();
			for(const auto& draw : drawEntries) 
			{
				drawEntriesVectorCache.push_back(draw);
			}
			dirtyDrawEntriesVectorCache = false;
		}
		for(const auto& draw : drawEntriesVectorCache) 
		{
			if(lastVao != *draw.meshInstance.mesh->vao) 
			{
				glBindVertexArray(*draw.meshInstance.mesh->vao);
				lastVao = *draw.meshInstance.mesh->vao;
			}
			if(!material && (lastMaterial != draw.meshInstance.material)) 
			{
				if(!lastMaterial || lastMaterial->getShaderProgram()->getId() != draw.meshInstance.material->getShaderProgram()->getId()) 
				{
					draw.meshInstance.material->useProgram();
				}
				draw.meshInstance.material->setupUniforms();
				draw.meshInstance.material->setupUniformBlocks(uboBindings);
				draw.meshInstance.material->setupTextures(textureBindings);
				lastMaterial = draw.meshInstance.material;
			}
			if(draw.commands.size() <= 3) 
			{
				for(auto& cmd : draw.commands) 
				{
					glDrawElementsInstancedBaseVertexBaseInstance(draw.meshInstance.mode, cmd.count, GL_UNSIGNED_INT, 0, cmd.instanceCount, cmd.baseVertex, cmd.baseInstance);
				}
			}
			else
			{
				glBindBuffer(GL_DRAW_INDIRECT_BUFFER, draw.indirectBuffer);
				glMultiDrawElementsIndirect(draw.meshInstance.mode, GL_UNSIGNED_INT, nullptr, draw.commands.size(), 0);
			}
		}
	}
} // namespace Kirino
