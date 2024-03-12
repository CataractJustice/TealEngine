#include "GUITransform.h"
#include "Core.h"

namespace TealEngine
{
	GUIUnit::GUIUnit() : 
	pixelsValue(0.0f), 
	windowWidthsValue(0.0f), 
	windowHeightsValue(0.0f), 
	rootWidthsValue(0.0f), 
	rootHeightsValue(0.0f), 
	parentWidthsValue(0.0f), 
	parentHeightsValue(0.0f)
	{
		
	}

	GUIUnit GUIUnit::pixels(float pixels) 
	{
		GUIUnit unit;
		unit.pixelsValue = pixels;
		return unit;
	}

	GUIUnit GUIUnit::windowWidths(float windowWidths) 
	{
		GUIUnit unit;
		unit.windowWidthsValue = windowWidths;
		return unit;
	}
	GUIUnit GUIUnit::windowHeights(float windowHeights) 
	{
		GUIUnit unit;
		unit.windowHeightsValue = windowHeights;
		return unit;
	}
	GUIUnit GUIUnit::rootWidths(float rootWidths) 
	{
		GUIUnit unit;
		unit.rootWidthsValue = rootWidths;
		return unit;
	}
	GUIUnit GUIUnit::rootHeights(float rootHeights) 
	{
		GUIUnit unit;
		unit.rootHeightsValue = rootHeights;
		return unit;
	}
	GUIUnit GUIUnit::parentWidths(float parentWidths) 
	{
		GUIUnit unit;
		unit.parentWidthsValue = parentWidths;
		return unit;
	}
	GUIUnit GUIUnit::parentHeights(float parentHeights) 
	{
		GUIUnit unit;
		unit.parentHeightsValue = parentHeights;
		return unit;
	}

	float GUIUnit::inWindowWidths(const GUITransform& transform) const
	{
		return inPixels(transform) / Graphics::window->getWindowWidth();
	};
	float GUIUnit::inWindowHeights(const GUITransform& transform) const
	{
		return inPixels(transform) / Graphics::window->getWindowHeight();
	};
	float GUIUnit::inRootHeights(const GUITransform& transform) const
	{
		return inPixels(transform) / transform.getRoot()->getCHeight().inPixels(transform);
	};
	float GUIUnit::inRootWidths(const GUITransform& transform) const
	{
		return inPixels(transform) / transform.getRoot()->getCWidth().inPixels(transform);
	};
	float GUIUnit::inPixels(const GUITransform& transform) const
	{
		return 
		pixelsValue + 
		windowWidthsValue * Graphics::window->getWindowWidth() +
		windowHeightsValue * Graphics::window->getWindowHeight() + 
		((transform.getRoot() != &transform) ? rootWidthsValue * transform.getRoot()->getCWidth().inPixels(*transform.getRoot()) : 0.0f) + 
		((transform.getRoot() != &transform) ? rootHeightsValue * transform.getRoot()->getCHeight().inPixels(*transform.getRoot()) : 0.0f) +
		((transform.getParent()) ? parentWidthsValue * transform.getParent()->getCWidth().inPixels(*transform.getParent()) : 0.0f) +
		((transform.getParent()) ? parentHeightsValue * transform.getParent()->getCHeight().inPixels(*transform.getParent()) : 0.0f)
		;

	};
	float GUIUnit::inParentWidths(const GUITransform& transform) const
	{
		return inPixels(transform) / transform.getParent()->getCWidth().inPixels(transform);
	};
	float GUIUnit::inParentHeighs(const GUITransform& transform) const
	{
		return inPixels(transform) / transform.getParent()->getCHeight().inPixels(transform);
	};


	GUIUnit GUIUnit::operator+(GUIUnit unit) const
	{
		GUIUnit sum;
		sum.pixelsValue 		= this->pixelsValue 		+ unit.pixelsValue;
		sum.windowWidthsValue 	= this->windowWidthsValue 	+ unit.windowWidthsValue;
		sum.windowHeightsValue 	= this->windowHeightsValue 	+ unit.windowHeightsValue;
		sum.rootWidthsValue 	= this->rootWidthsValue 	+ unit.rootWidthsValue;
		sum.rootHeightsValue 	= this->rootHeightsValue 	+ unit.rootHeightsValue;
		sum.parentWidthsValue 	= this->parentWidthsValue 	+ unit.parentWidthsValue;
		sum.parentHeightsValue 	= this->parentHeightsValue 	+ unit.parentHeightsValue;
		return sum;
	}
	GUIUnit GUIUnit::operator-(GUIUnit unit) const
	{
		GUIUnit sum;
		sum.pixelsValue 		= this->pixelsValue 		- unit.pixelsValue;
		sum.windowWidthsValue 	= this->windowWidthsValue 	- unit.windowWidthsValue;
		sum.windowHeightsValue 	= this->windowHeightsValue 	- unit.windowHeightsValue;
		sum.rootWidthsValue 	= this->rootWidthsValue 	- unit.rootWidthsValue;
		sum.rootHeightsValue 	= this->rootHeightsValue 	- unit.rootHeightsValue;
		sum.parentWidthsValue 	= this->parentWidthsValue 	- unit.parentWidthsValue;
		sum.parentHeightsValue 	= this->parentHeightsValue 	- unit.parentHeightsValue;
		return sum;
	} 
	GUIUnit GUIUnit::operator*(GUIUnit unit) const
	{
		GUIUnit sum;
		sum.pixelsValue 		= this->pixelsValue 		* unit.pixelsValue;
		sum.windowWidthsValue 	= this->windowWidthsValue 	* unit.windowWidthsValue;
		sum.windowHeightsValue 	= this->windowHeightsValue 	* unit.windowHeightsValue;
		sum.rootWidthsValue 	= this->rootWidthsValue 	* unit.rootWidthsValue;
		sum.rootHeightsValue 	= this->rootHeightsValue 	* unit.rootHeightsValue;
		sum.parentWidthsValue 	= this->parentWidthsValue 	* unit.parentWidthsValue;
		sum.parentHeightsValue 	= this->parentHeightsValue 	* unit.parentHeightsValue;
		return sum;
	} 
	GUIUnit GUIUnit::operator/(GUIUnit unit) const
	{
		GUIUnit sum;
		sum.pixelsValue 		= this->pixelsValue 		/ unit.pixelsValue;
		sum.windowWidthsValue 	= this->windowWidthsValue 	/ unit.windowWidthsValue;
		sum.windowHeightsValue 	= this->windowHeightsValue 	/ unit.windowHeightsValue;
		sum.rootWidthsValue 	= this->rootWidthsValue 	/ unit.rootWidthsValue;
		sum.rootHeightsValue 	= this->rootHeightsValue 	/ unit.rootHeightsValue;
		sum.parentWidthsValue 	= this->parentWidthsValue 	/ unit.parentWidthsValue;
		sum.parentHeightsValue 	= this->parentHeightsValue 	/ unit.parentHeightsValue;
		return sum;
	}

	GUITransform::GUITransform() : parent(0), root(0) 
	{
		
	}

	void GUITransform::setParent(GUITransform* transform) 
	{
		this->parent = transform;
		this->root = this->parent->getRoot();
	}

	GUIUnit GUITransform::getTop() const 
	{
		return (this->parent ? this->parent->getTop() : GUIUnit::pixels(0.0f)) + GUIUnit::pixels(this->offsetY.inPixels(*this));
	}
	GUIUnit GUITransform::getLeft() const 
	{
		return (this->parent ? this->parent->getLeft() : GUIUnit::pixels(0.0f)) + GUIUnit::pixels(this->offsetX.inPixels(*this));
	}
	GUIUnit GUITransform::getBottom() const 
	{
		return GUIUnit::pixels(getTop().inPixels((*this)) + height.inPixels(*this));
	}
	GUIUnit GUITransform::getRight() const 
	{
		return GUIUnit::pixels(getLeft().inPixels((*this)) + width.inPixels(*this));
	}
	const GUIUnit& GUITransform::getCWidth() const
	{
		return width;
	}
	const GUIUnit& GUITransform::getCHeight() const
	{
		return height;
	}
	const GUIUnit& GUITransform::getCOffsetX() const
	{
		return offsetX;
	}
	const GUIUnit& GUITransform::getCOffsetY() const
	{
		return offsetY;
	}
	
	GUIUnit& GUITransform::getWidth()
        {
                return width;
        }
        GUIUnit& GUITransform::getHeight()
        {
                return height;
        }
        GUIUnit& GUITransform::getOffsetX()
        {
                return offsetX;
        }
        GUIUnit& GUITransform::getOffsetY()
        {
                return offsetY;
        }

	const GUITransform* GUITransform::getRoot() const
	{
		return this->parent ? this->parent->getRoot() : this;
	}
	const GUITransform* GUITransform::getParent() const
	{
		return this->parent;
	}

	void GUITransform::setOffsetX(GUIUnit offsetX) 
	{
		this->offsetX = offsetX;
	}
	void GUITransform::setOffsetY(GUIUnit offsetY)
	{
		this->offsetY = offsetY;
	}
	void GUITransform::setWidth(GUIUnit width) 
	{
		this->width = width;
	}
	void GUITransform::setHeight(GUIUnit height) 
	{
		this->height = height;
	}
} // namespace TealEngine
