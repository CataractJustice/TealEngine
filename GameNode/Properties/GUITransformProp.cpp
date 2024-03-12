#include "GUITransformProp.h"
#include "libs/glm/gtc/type_ptr.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/euler_angles.hpp"

namespace TealEngine 
{
	GUITransformProp::GUITransformProp(GUITransform* transformPtr) : 
	x(&transformPtr->getOffsetX()),
	y(&transformPtr->getOffsetY()),
	w(&transformPtr->getWidth()),
	h(&transformPtr->getHeight())
	{
		this->propptr = transformPtr;
	}
	
	Json GUITransformProp::get()
	{
		GUITransform& transform = this->valueref<GUITransform>();
		Json json;
		json["oxp"] =  transform.getOffsetX().getPixelsValue();
		json["oxww"] = transform.getOffsetX().getWindowWidthsValue();
		json["oxwh"] = transform.getOffsetX().getWindowHeightsValue();
		json["oxrw"] = transform.getOffsetX().getRootWidthsValue();
		json["oxrh"] = transform.getOffsetX().getRootHeightsValue();
		json["oxpw"] = transform.getOffsetX().getParentWidthsValue();
		json["oxph"] = transform.getOffsetX().getParentHeightsValue();

		json["oyp"] =  transform.getOffsetY().getPixelsValue();
		json["oyww"] = transform.getOffsetY().getWindowWidthsValue();
		json["oywh"] = transform.getOffsetY().getWindowHeightsValue();
		json["oyrw"] = transform.getOffsetY().getRootWidthsValue();
		json["oyrh"] = transform.getOffsetY().getRootHeightsValue();
		json["oypw"] = transform.getOffsetY().getParentWidthsValue();
		json["oyph"] = transform.getOffsetY().getParentHeightsValue();

		json["wp"] =  transform.getWidth().getPixelsValue();
		json["www"] = transform.getWidth().getWindowWidthsValue();
		json["wwh"] = transform.getWidth().getWindowHeightsValue();
		json["wrw"] = transform.getWidth().getRootWidthsValue();
		json["wrh"] = transform.getWidth().getRootHeightsValue();
		json["wpw"] = transform.getWidth().getParentWidthsValue();
		json["wph"] = transform.getWidth().getParentHeightsValue();

		json["hp"] =  transform.getHeight().getPixelsValue();
		json["hww"] = transform.getHeight().getWindowWidthsValue();
		json["hwh"] = transform.getHeight().getWindowHeightsValue();
		json["hrw"] = transform.getHeight().getRootWidthsValue();
		json["hrh"] = transform.getHeight().getRootHeightsValue();
		json["hpw"] = transform.getHeight().getParentWidthsValue();
		json["hph"] = transform.getHeight().getParentHeightsValue();
		return json;
	}
	void GUITransformProp::set(const Json& json) 
	{
		GUITransform& transform = this->valueref<GUITransform>();
		transform.getOffsetX().getPixelsValue() = json["oxp"];
    	transform.getOffsetX().getWindowWidthsValue() = json["oxww"];
    	transform.getOffsetX().getWindowHeightsValue() = json["oxwh"];
    	transform.getOffsetX().getRootWidthsValue() = json["oxrw"];
    	transform.getOffsetX().getRootHeightsValue() = json["oxrh"];
    	transform.getOffsetX().getParentWidthsValue() = json["oxpw"];
    	transform.getOffsetX().getParentHeightsValue() = json["oxph"];

    	transform.getOffsetY().getPixelsValue() = json["oyp"];
    	transform.getOffsetY().getWindowWidthsValue() = json["oyww"];
    	transform.getOffsetY().getWindowHeightsValue() = json["oywh"];
    	transform.getOffsetY().getRootWidthsValue() = json["oyrw"];
    	transform.getOffsetY().getRootHeightsValue() = json["oyrh"];
    	transform.getOffsetY().getParentWidthsValue() = json["oypw"];
    	transform.getOffsetY().getParentHeightsValue() = json["oyph"];

    	transform.getWidth().getPixelsValue() = json["wp"];
    	transform.getWidth().getWindowWidthsValue() = json["www"];
    	transform.getWidth().getWindowHeightsValue() = json["wwh"];
    	transform.getWidth().getRootWidthsValue() = json["wrw"];
    	transform.getWidth().getRootHeightsValue() = json["wrh"];
    	transform.getWidth().getParentWidthsValue() = json["wpw"];
    	transform.getWidth().getParentHeightsValue() = json["wph"];

    	transform.getHeight().getPixelsValue() = json["hp"];
    	transform.getHeight().getWindowWidthsValue() = json["hww"];
    	transform.getHeight().getWindowHeightsValue() = json["hwh"];
    	transform.getHeight().getRootWidthsValue() = json["hrw"];
    	transform.getHeight().getRootHeightsValue() = json["hrh"];
    	transform.getHeight().getParentWidthsValue() = json["hpw"];
    	transform.getHeight().getParentHeightsValue() = json["hph"];
	}
	bool GUITransformProp::display(const char* label) 
	{
		GUITransform& transform = this->valueref<GUITransform>();
		bool edited = false;
		edited |= x.display("X offset");
		edited |= y.display("Y offset");
		edited |= w.display("Width");
		edited |= h.display("Height");

		ImGui::Text("Left: %f Top: %f Right: %f Bottom: %f", transform.getLeft().inPixels(transform), transform.getTop().inPixels(transform), transform.getRight().inPixels(transform), transform.getBottom().inPixels(transform));

		return edited;
	}
}