#pragma once
#include "libs/glm/glm.hpp"
#include "Graphics/Graphics.h"
#include <utility>
namespace TealEngine 
{

	enum  GUIUnitType
	{
		PIXELS,
		PERCENTS,
		WIDTH_PERCENTS,
		HEIGHT_PERCENTS
	};

	enum GUITransformParam 
	{
		GUI_X_OFFSET,
		GUI_Y_OFFSET,
		GUI_LEFT_BORDER,
		GUI_BOTTOM_BORDER,
		GUI_RIGHT_BORDER,
		GUI_TOP_BORDER,
		GUI_PARAMS_COUNT
	};
	
	bool isGUITransformParamVertical(GUITransformParam param);
	bool isGUITransformParamHorizontal(GUITransformParam param);

	struct GUIUnit 
	{
		float value;
		GUIUnitType type;
	};

	class GUITransform
	{
	private:
		float params[GUI_PARAMS_COUNT];
		float paramsRealValue[GUI_PARAMS_COUNT];
		bool updateRequiredParams[GUI_PARAMS_COUNT];
		GUIUnitType paramUnits[GUI_PARAMS_COUNT];
		GUITransform* parrent;
	public:
		GUITransform();

		void recalculateUnits();
		void set(GUITransformParam param, float value, GUIUnitType units);
		GUIUnit get(GUITransformParam param);
		float getParamRealValue(GUITransformParam param);
	};
}