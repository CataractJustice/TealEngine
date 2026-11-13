#pragma once
#include "GameNode.h"
#include "Graphics/GUI/GUITransform.h"
#include "GameNode/Properties/GUITransformProp.h"
#include "Graphics/Texture.h"
#include "Graphics/FrameBuffer/FrameBuffer.h"
namespace TealEngine
{
	class GameNodeGUI : public GameNode 
	{
	private:
	protected:
		GUITransform transform;
		Texture childrenTexture;
		Texture componentsTexture;
		FrameBuffer childrenFB;
		FrameBuffer componentsFB;
		unsigned short mousePressButton;
		bool redrawChildrenFlag;
		bool redrawComponentsFlag;
		bool scrollableX;
		bool scrollableY;
		int oldXPixels;
		int oldYPixels;
		unsigned int oldWidthPixels;
		unsigned int oldHeightPixels;
		int oldScrollXPixels;
		int oldScrollYPixels;
		GUIUnit maxScrollX;
		GUIUnit maxScrollY;
	public:
		GUITransformProp* transformProp;
		GameNodeGUI();
		~GameNodeGUI();
		Json toJson() override;
		void displayProps() override;
		void setParent(GameNode* parent) override;
		GUITransform& getTransform();
		void onWindowMousePress(unsigned short) override;
		void onWindowMouseRelease(unsigned short) override; 
		void redrawGUI();
		void redraw();
		void redrawChildren(bool clear = true);
		void redrawComponents(bool clear = true);
		void triggerRedrawComponents();
		void triggerRedrawChildren();
		void guiUpdate();
		void onChildRemove(GameNode* child, GameNode* parent) override;
		void setScrollableX(bool scrollable);
		void setScrollableY(bool scrollable);
		bool isScrollableX();
		bool isScrollableY();
		void setActive(bool active) override;
		GUIUnit getMaxScrollX();
		GUIUnit getMaxScrollY();
		//saves dimensions to be used by checkOldDimensions
		void updateOldDimensions();
		//returns true if old dimensions are the same as they were when updateOldDimensions was called
		bool checkOldDimensions();
		//
		void getChildrenBounds(GUIUnit& left, GUIUnit& top, GUIUnit& right, GUIUnit& bottom);
		//
		bool isOverElement(float x, float y, bool ignoreParentBounds);
	};
} // namespace TealEngine
