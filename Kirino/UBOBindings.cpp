#include <GL/glew.h>
#include "UBOBindings.h"

namespace Kirino
{
	UBOBindings::UBOBindings() : BindingManager(32) 
	{

	}
	void UBOBindings::bindImpl(unsigned int index, unsigned int to) 
	{
		glBindBufferBase(GL_UNIFORM_BUFFER, index, to);	
	}
} // namespace Kirino
