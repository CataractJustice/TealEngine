#pragma once
#include "Graphics/GUI/GUIComponent.h"
#include "Kirino/Material.h"
#include "Graphics/Texture.h"
namespace TealEngine 
{
	class Background : public GUIComponent 
	{
		private:
		static Kirino::Material* bgcolormaterial;
		public:
		glm::vec4 color;
		glm::vec4 pressColor;
		bool useImage;
		unsigned int textureId;
		const Texture* texture;
		bool pressed;
		Background();
		void GUIRender() override;
		void onMousePress(unsigned short) override;
		void onWindowMouseRelease(unsigned short) override;
		void setColor(glm::vec4 color);
		void setPressColor(glm::vec4 color);
		void setTexture(unsigned int texture);
		void setTexture(const Texture* texture);
		void onPropSet(const std::string& name) override;
	};
}