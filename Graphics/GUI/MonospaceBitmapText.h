#pragma once
#include "Graphics/GUI/GUIComponent.h"
#include "Kirino/Material.h"
#include "Kirino/VertexArrayObject.h"
#include "Graphics/Texture.h"
#include "GUITransform.h"
namespace TealEngine 
{
	class MonospaceBitmapText : public GUIComponent 
	{
		private:
		bool meshInit;
		static Kirino::Material* material;
		Kirino::VertexArrayObject vao;
		GUIUnit size;
		GUIUnit top;
		GUIUnit left;
		unsigned int vbo;
		unsigned int ebo;
		std::string text;
		public:
		glm::vec4 color;
		const Texture* texture;
		MonospaceBitmapText();
		void GUIRender() override;
		void setColor(glm::vec4 color);
		void setSize(GUIUnit size);
		void setLeft(GUIUnit left);
		void setTop(GUIUnit top);
		void setTexture(const Texture* texture);
		void setText(const std::string& text);
	};
}