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
		inline float& getPixelsValue() { return pixelsValue; };
		inline float& getWindowWidthsValue() { return windowWidthsValue; };
		inline float& getWindowHeightsValue() { return windowHeightsValue; };
		inline float& getRootWidthsValue() { return rootWidthsValue; };
		inline float& getRootHeightsValue() { return rootHeightsValue; };
		inline float& getParentWidthsValue() { return parentWidthsValue; };
		inline float& getParentHeightsValue() { return parentHeightsValue; };
		GUIUnit operator-(GUIUnit unit) const;
		GUIUnit operator+(GUIUnit unit) const;
		GUIUnit operator*(GUIUnit unit) const;
		GUIUnit operator/(GUIUnit unit) const;
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
		void setOffsetX(GUIUnit offsetX);
		void setOffsetY(GUIUnit offsetX);
		void setWidth(GUIUnit width);
		void setHeight(GUIUnit height);
		const GUITransform* getRoot() const;
		const GUITransform* getParent() const;
	};
} // namespace TealEngine
