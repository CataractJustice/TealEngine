#include "MonospaceBitmapText.h"
#include "GameNode/GameNodeGUI.h"
#include "Graphics/Renderer/RenderUtil.h"
#include "Core.h"
#include "libs/glm/glm.hpp"
#include "libs/glm/gtc/type_ptr.hpp"
#include "GameNode/ComponentFactory.h"
namespace TealEngine 
{
	Kirino::Material* MonospaceBitmapText::material = nullptr;
	MonospaceBitmapText::MonospaceBitmapText() 
	{
		addProp(new ColorProp(glm::value_ptr(color)), "Color");
		addProp(new GUIUnitProp(&size), "Size");
		addProp(new GUIUnitProp(&top), "Top");
		addProp(new GUIUnitProp(&left), "Left");
		addProp(new StringProp(&text), "Text");
		text = "Sample";
		color = glm::vec4(1.0f);
		texture = nullptr;
		addProp(new StringKeyMapValuePointerProp<Texture>(&texture, &Core::textureManager.getLoadedMap()), "Font image");

		vao.create();
		glGenBuffers(1, &vbo);
		glGenBuffers(1, &ebo);
		vao.bindElementArray(ebo);
		vao.vertexAttribPointer(Kirino::VertexArrayObject::VertexAttribPointer(true, vbo, 0, 2, GL_FLOAT, false, sizeof(float) * 4, 0, 0));
		vao.vertexAttribPointer(Kirino::VertexArrayObject::VertexAttribPointer(true, vbo, 1, 2, GL_FLOAT, false, sizeof(float) * 4, (void*)(sizeof(float) * 2), 0));
		vao.updateAttribs();
	}
	
	void MonospaceBitmapText::GUIRender() 
	{
		if(this->text.length() == 0) return;
		if(!this->texture) return;
		GUITransform& t = this->getParentOfType<GameNodeGUI>()->getTransform();
		if(material == nullptr) 
		{
			material = new Kirino::Material(&Core::shadersManager.get("Text"));
		}

		unsigned int tid = texture->id();
		material->setTexture("font", &tid, 1, 0);
		material->setUniform("color", glm::value_ptr(color), sizeof(float) * 4, 0);
		glm::vec2 scale = glm::vec2(this->size.inSelfWidths(t), this->size.inSelfHeights(t));
		glm::vec2 offset = glm::vec2(this->left.inSelfWidths(t), this->top.inSelfHeights(t));
		material->setUniform("scale", glm::value_ptr(scale), sizeof(float) * 2, 0);
		material->setUniform("offset", glm::value_ptr(offset), sizeof(float) * 2, 0);
		std::vector<float> vertices;
		std::vector<unsigned int> indices;
		glm::vec2 charPos = glm::vec2(0.0f);
		for(int charIndex = 0; charIndex < this->text.length(); charIndex++) 
		{
			const char character = this->text[charIndex];
			const float charOffset = (float)character;
			if(character == '\n') 
			{
				charPos.x = 0.0f;
				charPos.y += 1.0f;
				continue;
			}
			if(character == ' ') 
			{
				charPos.x += 1.0f;
				continue;
			}
			const int ibase = vertices.size() / 4;
			vertices.push_back(charPos.x);
			vertices.push_back(charPos.y);
			vertices.push_back(charOffset);
			vertices.push_back(0.0f);

			vertices.push_back(charPos.x+1.0f);
			vertices.push_back(charPos.y);
			vertices.push_back(charOffset+1.0f);
			vertices.push_back(0.0f);
			
			vertices.push_back(charPos.x+1.0f);
			vertices.push_back(charPos.y+1.0f);
			vertices.push_back(charOffset+1.0f);
			vertices.push_back(1.0f);
			
			vertices.push_back(charPos.x);
			vertices.push_back(charPos.y+1.0f);
			vertices.push_back(charOffset);
			vertices.push_back(1.0f);
			
			indices.push_back(ibase + 0);
			indices.push_back(ibase + 1);
			indices.push_back(ibase + 2);
			indices.push_back(ibase + 2);
			indices.push_back(ibase + 3);
			indices.push_back(ibase + 0);
			charPos.x += 1.0f;
		}
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), indices.data(), GL_STATIC_DRAW);
		
		Kirino::UBOBindings uboBindings;
		uboBindings.reset();
		Kirino::TextureBindings textureBindings;
		textureBindings.reset();
		material->useProgram();
		material->setupUniforms();
		material->setupTextures(textureBindings);
		material->setupUniformBlocks(uboBindings);
		glBindVertexArray(vao);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, (void*)0);
	}
	
	void MonospaceBitmapText::setColor(glm::vec4 color) 
	{
		this->color = color;
		redraw();
	}

	void MonospaceBitmapText::setSize(GUIUnit size) 
	{
		this->size = size;
		redraw();
	}
	void MonospaceBitmapText::setLeft(GUIUnit left) 
	{
		this->left = left;
		redraw();
	}
	void MonospaceBitmapText::setTop(GUIUnit top) 
	{
		this->top = top;
		redraw();
	}
	void MonospaceBitmapText::setText(const std::string& text) 
	{
		this->text = text;
		redraw();
	}
	void MonospaceBitmapText::setTexture(const Texture* texture) 
	{
		this->texture = texture;
		redraw();
	}
	EXPORT_COMPONENT(MonospaceBitmapText);
}