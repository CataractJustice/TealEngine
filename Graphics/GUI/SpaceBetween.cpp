#include "SpaceBetween.h"
#include "GameNode/GameNodeGUI.h"
#include "GameNode/ComponentFactory.h"
namespace TealEngine
{
	void SpaceBetween::updateChildrenTransform() 
	{
		GameNodeGUI* parent = this->getParentOfType<GameNodeGUI>();
		auto children = parent->findChildsByType<GameNodeGUI>();
		float childrenPixelWidth = 0.0f;
		float thisPixelWidth = parent->getTransform().getWidth().inPixels(parent->getTransform());
		for(const auto& child : children) 
		{
			childrenPixelWidth += child->getTransform().getWidth().inPixels(child->getTransform());
		}
		float gap = (thisPixelWidth - childrenPixelWidth) / std::max(1.0f, static_cast<float>(children.size()) - 1.0f);
		float currentOffset = 0.0f;
		for(const auto& child : children) 
		{
			child->getTransform().getOffsetX() = GUIUnit::pixels(currentOffset);
			currentOffset += child->getTransform().getWidth().inPixels(child->getTransform()) + gap;
		}
	}
	void SpaceBetween::GUIRender() 
	{
		updateChildrenTransform();
	}
	EXPORT_COMPONENT(SpaceBetween);
} // namespace TealEngine
