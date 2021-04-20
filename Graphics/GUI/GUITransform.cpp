#include "GUITransform.h"
namespace TealEngine 
{
	bool isGUITransformParamVertical(GUITransformParam param)
	{
		return
			param == GUI_Y_OFFSET ||
			param == GUI_BOTTOM_BORDER ||
			param == GUI_TOP_BORDER;
	}
	bool isGUITransformParamHorizontal(GUITransformParam param)
	{
		return !isGUITransformParamVertical(param);
	}

	GUITransform::GUITransform()
	{
		parrent = nullptr;
		memset(params, 0, sizeof(float) * GUI_PARAMS_COUNT);
		memset(paramsRealValue, 0, sizeof(float) * GUI_PARAMS_COUNT);
		memset(updateRequiredParams, 0, sizeof(bool) * GUI_PARAMS_COUNT);
		memset(paramUnits, 0, sizeof(GUIUnitType) * GUI_PARAMS_COUNT);
	}

	void GUITransform::recalculateUnits()
	{
		float width, height, xOffs = 0.0f, yOffs = 0.0f;
		if (parrent)
		{
			width = parrent->getParamRealValue(GUI_RIGHT_BORDER) - parrent->getParamRealValue(GUI_LEFT_BORDER);
			height = parrent->getParamRealValue(GUI_TOP_BORDER) - parrent->getParamRealValue(GUI_BOTTOM_BORDER);
			xOffs = parrent->getParamRealValue(GUI_X_OFFSET);
			yOffs = parrent->getParamRealValue(GUI_Y_OFFSET);
		}
		else
		{
			width = Graphics::window->getScreenWidth();
			height = Graphics::window->getScreenHeight();
		}
		for (unsigned int i = 0; i < GUI_PARAMS_COUNT; i++)
		{
			if (updateRequiredParams[i])
			{
				updateRequiredParams[i] = false;
				switch (paramUnits[i])
				{
				case PIXELS:
					paramsRealValue[i] = params[i] / (isGUITransformParamVertical((GUITransformParam)i) ? height : width);
					break;
				case PERCENTS:
					paramsRealValue[i] = params[i];
					break;
				case WIDTH_PERCENTS:
					paramsRealValue[i] = params[i] / (isGUITransformParamVertical((GUITransformParam)i) ? (height / width) : 1.0f);
					break;
				case HEIGHT_PERCENTS:
					paramsRealValue[i] = params[i] / (isGUITransformParamVertical((GUITransformParam)i) ? 1.0f : (width / height));
					break;
				default:
					break;
				}
			}
			paramsRealValue[GUI_X_OFFSET] += xOffs;
			paramsRealValue[GUI_Y_OFFSET] += yOffs;
		}
	}
	void GUITransform::set(GUITransformParam param, float value, GUIUnitType units)
	{
		params[param] = value;
		paramUnits[param] = units;
		updateRequiredParams[param] = true;
	}

	GUIUnit GUITransform::get(GUITransformParam param)
	{
		return GUIUnit{ params[param], paramUnits[param] };
	}

	float GUITransform::getParamRealValue(GUITransformParam param)
	{
		return paramsRealValue[param];
	}
	
}