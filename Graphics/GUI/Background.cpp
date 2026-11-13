#include "Background.h"
#include "GameNode/GameNodeGUI.h"
#include "Graphics/Renderer/RenderUtil.h"
#include "Core.h"
#include "libs/glm/glm.hpp"
#include "libs/glm/gtc/type_ptr.hpp"
#include "GameNode/ComponentFactory.h"
namespace TealEngine 
{
	Kirino::Material* Background::bgcolormaterial = nullptr;
	Background::Background() 
	{
		addProp(new ColorProp(glm::value_ptr(color)), "Color");
		addProp(new ColorProp(glm::value_ptr(pressColor)), "On press color");
		color = glm::vec4(1.0f);
		pressColor = color;
		pressed = false;
		texture = nullptr;
		textureId = 0;
		addProp(new StringKeyMapValuePointerProp<Texture>(&texture, &Core::textureManager.getLoadedMap()), "Image");
	}

	void Background::GUIRender() 
	{
		if(bgcolormaterial == nullptr) 
		{
			bgcolormaterial = new Kirino::Material(&Core::shadersManager.get("GuiBackground"));
		}
		glm::vec4 color = this->getParent()->getMouseButtonPressMask() ? pressColor : this->color;

		if(this->texture) 
		{
			unsigned int tid = texture->id();
			bgcolormaterial->setTexture("image", &tid, 1, 0);
		}
		else 
		{
			unsigned int tid = this->textureId ? textureId : Core::textureManager.get("t.png").id();
			bgcolormaterial->setTexture("image", &tid, 1, 0);
		}
		bgcolormaterial->setUniform("color", glm::value_ptr(color), sizeof(float) * 4, 0);
		Render::renderShader(bgcolormaterial);
	}

	void Background::onMousePress(unsigned short) 
	{
		pressed = true;
		if(pressColor != color) redraw();
	}
	void Background::onWindowMouseRelease(unsigned short) 
	{
		if(!pressed) return;
		pressed = false;
		if(pressColor != color) 
		{
			redraw();
		}
	}

	void Background::setColor(glm::vec4 color) 
	{
		this->color = color;
		redraw();
	}
	void Background::setPressColor(glm::vec4 color) 
	{
		this->pressColor = color;
		redraw();
	}
	void Background::setTexture(const Texture* texture) 
	{
		this->texture = texture;
		this->textureId = texture ? texture->id() : 0;
		redraw();
	}
	void Background::setTexture(unsigned int texture) 
	{
		this->textureId = texture;
		this->texture = nullptr;
		redraw();
	}
	void Background::onPropSet(const std::string& name) 
	{
		if(this->texture) 
		{
			this->textureId = texture->id();
		}
	}
	EXPORT_COMPONENT(Background);
}