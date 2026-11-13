#include <GL/glew.h>
#include "TextureBindings.h"

namespace Kirino
{
	TextureBindings::TextureBindings() : BindingManager(128) 
	{

	}
	void TextureBindings::bindImpl(unsigned int index, unsigned int to) 
	{
		if(to == 0) return;
		glActiveTexture(GL_TEXTURE0 + index);
		unsigned int volumeMask = to & (1 << 31);
		if(volumeMask) 
		{
			glBindTexture(GL_TEXTURE_3D, to ^ volumeMask);
		}
		else 
		{
			glBindTexture(GL_TEXTURE_2D, to);
		}
	}
} // namespace Kirino
