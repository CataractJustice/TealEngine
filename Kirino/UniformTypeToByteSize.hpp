#include <GL/glew.h>
namespace Kirino 
{
	inline unsigned int uniformTypeToByteSize(GLenum type)
	{
		switch (type) 
		{
			case GL_FLOAT: return 4;
			case GL_FLOAT_VEC2: return 8;
			case GL_FLOAT_VEC3: return 12;
			case GL_FLOAT_VEC4: return 16;
			case GL_INT: return 4;
			case GL_INT_VEC2: return 8;
			case GL_INT_VEC3: return 12;
			case GL_INT_VEC4: return 16;
			case GL_UNSIGNED_INT: return 4;
			case GL_UNSIGNED_INT_VEC2: return 8;
			case GL_UNSIGNED_INT_VEC3: return 12;
			case GL_UNSIGNED_INT_VEC4: return 16;
			case GL_BOOL: return 4;
			case GL_BOOL_VEC2: return 8;
			case GL_BOOL_VEC3: return 12;
			case GL_BOOL_VEC4: return 16;
			case GL_FLOAT_MAT2: return 32;
			case GL_FLOAT_MAT3: return 48;
			case GL_FLOAT_MAT4: return 64;
			case GL_FLOAT_MAT2x3: return 32;
			case GL_FLOAT_MAT2x4: return 32;
			case GL_FLOAT_MAT3x2: return 24;
			case GL_FLOAT_MAT3x4: return 48;
			case GL_FLOAT_MAT4x2: return 32;
			case GL_FLOAT_MAT4x3: return 48;
			default: return 0;
		}
	}
}