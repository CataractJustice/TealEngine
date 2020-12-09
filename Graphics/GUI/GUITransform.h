#pragma once
#include "glm/glm.hpp"
#include "Graphics.h"
#include <utility>
namespace TealEngine 
{

	enum  GUIUnit
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

	class GUITransform
	{
	private:
		float params[GUI_PARAMS_COUNT];
		float paramsRealValue[GUI_PARAMS_COUNT];
		GUIUnit paramUnits[GUI_PARAMS_COUNT];
		GUITransform* parrent;
	public:
		void recalculateUnits() 
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
				width = Graphics::Window::getScreenWidth();
				height = Graphics::Window::getScreenHeight();
			}
			for (unsigned int i = 0; i < GUI_PARAMS_COUNT; i++) 
			{
				switch (paramUnits[i])
				{
				case PIXELS:
					paramsRealValue[i] = params[i] / ((i % 2) ? height : width);
					break;
				case PERCENTS:
					paramsRealValue[i] = params[i];
					break;
				case WIDTH_PERCENTS:
					paramsRealValue[i] = params[i] / ((i % 2) ? (height / width) : 1.0f);
					break;
				case HEIGHT_PERCENTS:
					paramsRealValue[i] = params[i] / ((i % 2) ? 1.0f : (width / height));
					break;
				default:
					break;
				}
				paramsRealValue[GUI_X_OFFSET] += xOffs;
				paramsRealValue[GUI_Y_OFFSET] += yOffs;
			}
		}
		void set(GUITransformParam param, float value, GUIUnit units) 
		{
			params[param] = value;
			paramUnits[param] = units;
		}

		std::pair<float, GUIUnit> get(GUITransformParam param) 
		{
			return std::pair<float, GUIUnit>(params[param], paramUnits[param]);
		}

		float getParamRealValue(GUITransformParam param)
		{
			return paramsRealValue[param];
		}
	};
}