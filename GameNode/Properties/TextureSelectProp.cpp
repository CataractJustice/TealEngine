#include "TextureSelectProp.h"
namespace TealEngine 
{
	bool TextureSelectProp::display(const char* label) 
	{
		bool edited = StringKeyMapValuePointerProp::display(label);
		if(*((Texture**)propptr))
		{
			const int maxPixelsSize = 320;
			const int minPixelsSize = 64;

			int oWidth = this->valueref<Texture*>()->getWidth();
			int oHeight = this->valueref<Texture*>()->getHeight();

			int longSide = std::max(oWidth, oHeight);

			float scaleFactor = 1.0f;

			if(longSide > maxPixelsSize) 
			{
				scaleFactor = maxPixelsSize / ((float)longSide);
			}
			else if(longSide < minPixelsSize) 
			{
					scaleFactor = minPixelsSize / ((float)longSide);
			}

			ImGui::Image(ImTextureID((long)(this->valueref<Texture*>()->id())), ImVec2(oWidth * scaleFactor, oHeight * scaleFactor));
		}
		
		return edited;
	}
}