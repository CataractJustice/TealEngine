#pragma once
#include "GameNode.h"
#include "Graphics/GUI/GUITransform.h"
#include "GameNode/Properties/GUITransformProp.h"
namespace TealEngine
{
	class GameNodeGUI : public GameNode 
	{
	private:
	protected:
		GUITransform transform;
		unsigned short mousePressButton;
	public:
		GUITransformProp* transformProp;
		GameNodeGUI();
		Json toJson() override;
		void displayProps() override;
		void setParent(GameNode* parent) override;
		GUITransform& getTransform();
		void onWindowMousePress(unsigned short) override;
		void onWindowMouseRelease(unsigned short) override; 
	};
} // namespace TealEngine
