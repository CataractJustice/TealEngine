#pragma once
namespace TealEngine
{
	class GUITransform;
	
	class GUIUnit
	{
		private:
		protected:
		float pixelsValue;
		float windowWidthsValue;
		float windowHeightsValue;
		float rootWidthsValue;
		float rootHeightsValue;
		float parentWidthsValue;
		float parentHeightsValue;
		public:
		GUIUnit();
		static GUIUnit pixels(float pixels);
		static GUIUnit windowWidths(float windowWidths);
		static GUIUnit windowHeights(float windowHeights);
		static GUIUnit rootWidths(float rootWidths);
		static GUIUnit rootHeights(float rootHeights);
		static GUIUnit parentWidths(float parentWidths);
		static GUIUnit parentHeights(float parentHeights);
		float inWindowWidths(const GUITransform& transform) const;
		float inWindowHeights(const GUITransform& transform) const;
		float inRootHeights(const GUITransform& transform) const;
		float inRootWidths(const GUITransform& transform) const;
		float inPixels(const GUITransform& transform) const;
		float inParentWidths(const GUITransform& transform) const;
		float inParentHeighs(const GUITransform& transform) const;
		float inSelfWidths(const GUITransform& transform) const;
		float inSelfHeights(const GUITransform& transform) const;
		inline float& getPixelsValue() { return pixelsValue; };
		inline float& getWindowWidthsValue() { return windowWidthsValue; };
		inline float& getWindowHeightsValue() { return windowHeightsValue; };
		inline float& getRootWidthsValue() { return rootWidthsValue; };
		inline float& getRootHeightsValue() { return rootHeightsValue; };
		inline float& getParentWidthsValue() { return parentWidthsValue; };
		inline float& getParentHeightsValue() { return parentHeightsValue; };
		GUIUnit operator-(GUIUnit unit) const;
		GUIUnit operator-() const;
		GUIUnit operator+(GUIUnit unit) const;
		GUIUnit operator*(GUIUnit unit) const;
		GUIUnit operator/(GUIUnit unit) const;
		template<typename N>
		GUIUnit operator/(N divisor) const 
		{
			GUIUnit res;
			res.pixelsValue 		= this->pixelsValue 		/ divisor;
			res.windowWidthsValue 	= this->windowWidthsValue 	/ divisor;
			res.windowHeightsValue 	= this->windowHeightsValue 	/ divisor;
			res.rootWidthsValue 	= this->rootWidthsValue 	/ divisor;
			res.rootHeightsValue 	= this->rootHeightsValue 	/ divisor;
			res.parentWidthsValue 	= this->parentWidthsValue 	/ divisor;
			res.parentHeightsValue 	= this->parentHeightsValue 	/ divisor;
			return res;
		}
		template <typename N>
		GUIUnit operator*(N factor) const
		{
			GUIUnit res;
			res.pixelsValue 		= this->pixelsValue 		* factor;
			res.windowWidthsValue 	= this->windowWidthsValue 	* factor;
			res.windowHeightsValue 	= this->windowHeightsValue 	* factor;
			res.rootWidthsValue 	= this->rootWidthsValue 	* factor;
			res.rootHeightsValue 	= this->rootHeightsValue 	* factor;
			res.parentWidthsValue 	= this->parentWidthsValue 	* factor;
			res.parentHeightsValue 	= this->parentHeightsValue 	* factor;
			return res;
		}
	};

	class GUITransform 
	{
		private:
		const GUITransform* parent;
		const GUITransform* root;
		GUIUnit offsetX;
		GUIUnit offsetY;
		GUIUnit width;
		GUIUnit height;
		GUIUnit scrollX;
		GUIUnit scrollY;

		enum class GUIPositioning 
		{
			RELATIVE_TO_WINDOW,
			RELATIVE_TO_PARENT,
			RELATIVE_TO_ROOT
		};
		public:
		GUITransform();
		void setParent(GUITransform* transform);

		//returns distance from the top edge of a window to the top edge of this transform
		GUIUnit getTop() const;
		//returns distance from the left edge of a window transform to the left edge of this transform
		GUIUnit getLeft() const;
		//returns distance from the bottom edge of a window transform to the bottom edge of this transform
		GUIUnit getBottom() const;
		//returns distance from the right edge of a window transform to the right edge of this transform
		GUIUnit getRight() const;
		GUIUnit& getWidth();
        GUIUnit& getHeight();
        GUIUnit& getOffsetX();
        GUIUnit& getOffsetY();
		const GUIUnit& getCWidth() const;
		const GUIUnit& getCHeight() const;
		const GUIUnit& getCOffsetX() const;
		const GUIUnit& getCOffsetY() const;
		void setOffsetX(const GUIUnit& offsetX);
		void setOffsetY(const GUIUnit& offsetX);
		void setWidth(const GUIUnit& width);
		void setHeight(const GUIUnit& height);
		const GUITransform* getRoot() const;
		const GUITransform* getParent() const;
		void setScrollX(const GUIUnit& scroll);
		void setScrollY(const GUIUnit& scroll);
		GUIUnit getScrollX() const;
		GUIUnit getScrollY() const;
	};
} // namespace TealEngine
