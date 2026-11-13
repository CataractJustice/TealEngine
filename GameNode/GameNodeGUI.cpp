#include "GameNodeGUI.h"
#include "System/Input.h"
#include "Component.h"
#include "Graphics/Renderer/RenderUtil.h"
namespace TealEngine 
{
	GameNodeGUI::GameNodeGUI() 
	{
		this->transformProp = new GUITransformProp(&transform);
		childrenTexture = Texture(GL_TEXTURE_2D, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE);
		componentsTexture = Texture(GL_TEXTURE_2D, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE);
		childrenTexture.create(1,1);
		componentsTexture.create(1,1);
		redrawChildrenFlag = redrawComponentsFlag = false;

		childrenFB.bind();
		childrenFB.attachTexture(childrenTexture.id(), 0);
		childrenFB.enable(0);
		childrenFB.apply();
		childrenFB.unbind();

		componentsFB.bind();
		componentsFB.attachTexture(componentsTexture.id(), 0);
		componentsFB.enable(0);
		componentsFB.apply();
		componentsFB.unbind();
		oldXPixels = oldYPixels = oldWidthPixels = oldHeightPixels = 0;
	}
	GameNodeGUI::~GameNodeGUI() 
	{
		delete transformProp;
	}
	Json GameNodeGUI::toJson() 
	{
		Json json = GameNode::toJson();
		json["transform"] = this->transformProp->get();
		json["type"] = "gui";
		return json;
	}
		
	void GameNodeGUI::displayProps() 
	{
		this->transformProp->display("Transform");
		GameNode::displayProps();
	}

	void GameNodeGUI::setParent(GameNode* parent) 
	{
		GameNode::setParent(parent);
		GameNodeGUI* nodeGUI = this->getParentOfType<GameNodeGUI>();
		if(nodeGUI) 
		{
			this->transform.setParent(&nodeGUI->getTransform());
		}
	}

	GUITransform& GameNodeGUI::getTransform() 
	{
		return this->transform;
	}

	void GameNodeGUI::onWindowMousePress(unsigned short button) 
	{
		if(!active) return;
		glm::vec2 mpos = Input::Mouse::getMousePos();
		if(
		mpos.x > getTransform().getLeft().inPixels(transform) &&
		mpos.y > getTransform().getTop().inPixels(transform) &&
		mpos.x < getTransform().getRight().inPixels(transform) &&
		mpos.y < getTransform().getBottom().inPixels(transform)) 
		{
			onMousePress(button);
		}
		GameNode::onWindowMousePress(button);
	}
	void GameNodeGUI::onWindowMouseRelease(unsigned short button) 
	{
		if(!active) return;
		glm::vec2 mpos = Input::Mouse::getMousePos();
		if(
		mpos.x > getTransform().getLeft().inPixels(transform) &&
		mpos.y > getTransform().getTop().inPixels(transform) &&
		mpos.x < getTransform().getRight().inPixels(transform) &&
		mpos.y < getTransform().getBottom().inPixels(transform)) 
		{
			onMouseRelease(button);
		}
		GameNode::onWindowMouseRelease(button);
	}
	void GameNodeGUI::redraw()
	{
		if(!active) return;
		unsigned int wpx = this->transform.getWidth().inPixels(this->transform);
		unsigned int hpx = this->transform.getHeight().inPixels(this->transform);

		GameNodeGUI* guiparent = getParentOfType<GameNodeGUI>();
		int xpx = this->transform.getOffsetX().inPixels(this->transform) - (guiparent ? (guiparent->getTransform().getScrollX().inPixels(transform)) : (0));
		int ypx = this->transform.getOffsetY().inPixels(this->transform) - (guiparent ? (guiparent->getTransform().getScrollY().inPixels(transform)) : (0));
		unsigned int vw = guiparent ? guiparent->childrenFB.getVW() : Graphics::window->getWindowWidth();
		unsigned int vh = guiparent ? guiparent->childrenFB.getVH() : Graphics::window->getWindowHeight();

		glViewport(xpx, vh-ypx-hpx, wpx, hpx);
		Render::renderTexture(componentsTexture.id());
		Render::renderTexture(childrenTexture.id());
	}
	void GameNodeGUI::redrawChildren(bool clear)
	{
		redrawChildrenFlag = false;
		unsigned int wpx = this->transform.getWidth().inPixels(this->transform);
		unsigned int hpx = this->transform.getHeight().inPixels(this->transform);

		GUIUnit cBoundsLeft, cBoundsRight, cBoundsTop, cBoundsBottom;
		getChildrenBounds(cBoundsLeft, cBoundsRight, cBoundsTop, cBoundsBottom);
		maxScrollX = GUIUnit::pixels(
			std::max(0.0f, cBoundsRight.inPixels(transform) - wpx)
		);
		maxScrollY = GUIUnit::pixels(
			std::max(0.0f, cBoundsBottom.inPixels(transform) - hpx)
		);
		//Max scroll bounds
		if(transform.getScrollX().inPixels(transform) > maxScrollX.inPixels(transform)) transform.setScrollX(maxScrollX);
		if(transform.getScrollY().inPixels(transform) > maxScrollY.inPixels(transform)) transform.setScrollY(maxScrollY);
		//Min scroll bounds:
		if(transform.getScrollX().inPixels(transform) < 0.0f) transform.setScrollX(GUIUnit::pixels(0.0f));
		if(transform.getScrollY().inPixels(transform) < 0.0f) transform.setScrollY(GUIUnit::pixels(0.0f));

		if(childrenTexture.getWidth() != wpx || childrenTexture.getHeight() != hpx)
			childrenTexture.create(wpx, hpx);
		childrenFB.bind();
		childrenFB.viewport(wpx, hpx);
		if(clear) glClear(GL_COLOR_BUFFER_BIT);
		for(GameNode* c : childNodes) 
		{
			if(c->getActive() == false) continue;
			GameNodeGUI* cgui = dynamic_cast<GameNodeGUI*>(c);
			if(cgui == nullptr) continue;
			cgui->redraw();
		}
		childrenFB.unbind();
		GameNodeGUI* guiparent = getParentOfType<GameNodeGUI>();
		if(guiparent) guiparent->triggerRedrawChildren();
	}

	void GameNodeGUI::redrawComponents(bool clear) 
	{
		redrawComponentsFlag = false;
		unsigned int wpx = this->transform.getWidth().inPixels(this->transform);
		unsigned int hpx = this->transform.getHeight().inPixels(this->transform);
		unsigned int xpx = this->transform.getOffsetX().inPixels(this->transform);
		unsigned int ypx = this->transform.getOffsetY().inPixels(this->transform);
		unsigned int ww =  Graphics::window->getWindowWidth();
		unsigned int wh =  Graphics::window->getWindowHeight();
		componentsTexture.create(wpx, hpx);
		componentsFB.bind();
		componentsFB.viewport(wpx, hpx);
		if(clear) glClear(GL_COLOR_BUFFER_BIT);
		for(Component* c : components) 
		{
			c->GUIRender();
		}
		componentsFB.unbind();
		oldXPixels = xpx;
		oldYPixels = ypx;
		oldWidthPixels = wpx;
		oldHeightPixels = hpx;
		GameNodeGUI* guiparent = getParentOfType<GameNodeGUI>();
		if(guiparent) guiparent->triggerRedrawChildren();
	}

	void GameNodeGUI::triggerRedrawComponents() 
	{
		this->redrawComponentsFlag = true;
	}
	void GameNodeGUI::triggerRedrawChildren() 
	{
		this->redrawChildrenFlag = true;
	}

	void GameNodeGUI::guiUpdate() 
	{
		bool treeActive = getActive();
		glEnable(GL_BLEND);
		glDisable(GL_DEPTH_TEST);
		int xpx = this->transform.getOffsetX().inPixels(this->transform);
		int ypx = this->transform.getOffsetY().inPixels(this->transform);
		int sxpx = this->transform.getScrollX().inPixels(this->transform);
		int sypx = this->transform.getScrollY().inPixels(this->transform);
		unsigned int wpx = this->transform.getWidth().inPixels(this->transform);
		unsigned int hpx = this->transform.getHeight().inPixels(this->transform);
		if(sxpx != oldScrollXPixels || sypx != oldScrollYPixels) 
		{
			triggerRedrawChildren();
			triggerRedrawComponents();
		}
		if(xpx != oldXPixels || ypx != oldYPixels) 
		{
			GameNodeGUI* parentGUI =  this->getParentOfType<GameNodeGUI>();
			if(parentGUI) parentGUI->triggerRedrawChildren();
		}
		if(wpx != oldWidthPixels || hpx != oldHeightPixels) triggerRedrawComponents();
		if(this->redrawComponentsFlag && treeActive) 
		{
			glBlendFuncSeparate(GL_ONE_MINUS_DST_COLOR, GL_ONE, GL_ZERO, GL_ZERO);
			redrawComponents();
			glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE_MINUS_DST_ALPHA, GL_ONE);
			redrawComponents(false);
		}
		if(getParentOfType<GameNodeGUI>() == nullptr && treeActive) 
		{
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			redraw();
		}
		if(this->redrawChildrenFlag && treeActive) 
		{
			glBlendFuncSeparate(GL_ONE_MINUS_DST_COLOR, GL_ONE, GL_ZERO, GL_ONE);
			redrawChildren();
			glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE_MINUS_DST_ALPHA, GL_ONE);
			redrawChildren(false);
		}
		oldScrollXPixels = sxpx;
		oldScrollYPixels = sypx;
		oldWidthPixels = wpx;
		oldHeightPixels = hpx;
		oldXPixels = xpx;
		oldYPixels = ypx;
	}

	void GameNodeGUI::onChildRemove(GameNode* child, GameNode* parent) 
	{
		GameNode::onChildRemove(child, parent);
		this->triggerRedrawChildren();
	}

	void GameNodeGUI::setScrollableX(bool scrollable)
	{
		scrollableX = scrollable;

	}

	void GameNodeGUI::setScrollableY(bool scrollable)
	{
		scrollableY = scrollable;
		this->triggerRedrawChildren();
	}

	bool GameNodeGUI::isScrollableX()
	{
		return scrollableX;
	}

	bool GameNodeGUI::isScrollableY()
	{
		return scrollableY;
	}

	GUIUnit GameNodeGUI::getMaxScrollX()
	{
		return maxScrollX;
	}

	GUIUnit GameNodeGUI::getMaxScrollY()
	{
		return maxScrollY;
	}
	
	void GameNodeGUI::getChildrenBounds(GUIUnit& left, GUIUnit& top, GUIUnit& right, GUIUnit& bottom) 
	{
		left = GUIUnit::pixels(10000000.0f);
		top = GUIUnit::pixels(10000000.0f);
		right = GUIUnit::pixels(-10000000.0f);
		bottom = GUIUnit::pixels(-10000000.0f);

		for(GameNode* child : childNodes) 
		{
			GameNodeGUI* guiChild = dynamic_cast<GameNodeGUI*>(child);
			if(!guiChild) continue;
			GUITransform& childTransform = guiChild->getTransform();
			GUIUnit childLeft = childTransform.getOffsetX();
			GUIUnit childTop = childTransform.getOffsetY();
			GUIUnit childRight = childTransform.getWidth() + childLeft;
			GUIUnit childBottom = childTransform.getHeight() + childTop;
			if(childLeft.inPixels(childTransform) < left.inPixels(childTransform)) left = childLeft;
			if(childTop.inPixels(childTransform) < top.inPixels(childTransform)) top = childTop;
			if(childRight.inPixels(childTransform) > right.inPixels(childTransform)) right = childRight;
			if(childBottom.inPixels(childTransform) > bottom.inPixels(childTransform)) bottom = childBottom;
		}
	}

	bool GameNodeGUI::isOverElement(float x, float y, bool ignoreParentBounds) 
	{
		GameNodeGUI* parentGui = getParentOfType<GameNodeGUI>();
		if(ignoreParentBounds == false && parentGui && parentGui->isOverElement(x,y,false) == false) 
		{
			return false;
		}
		return 
			transform.getLeft().inPixels(transform) < x &&
			transform.getRight().inPixels(transform) > x &&
			transform.getTop().inPixels(transform) < y &&
			transform.getBottom().inPixels(transform) > y;
	}

	void GameNodeGUI::setActive(bool active) 
	{
		GameNodeGUI* guiParent = this->getParentOfType<GameNodeGUI>();
		if(guiParent) guiParent->triggerRedrawChildren();
		GameNode::setActive(active);
	}
}
