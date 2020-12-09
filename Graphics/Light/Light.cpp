#include "Light.h"

namespace TealEngine
{
	ShaderProgram dLightShader;
	ShaderProgram aLightShader;
	ShaderProgram combineLightShader;
	void lightInit() 
	{
		dLightShader = Resources::getShader("direction_light");
		aLightShader = Resources::getShader("ambient_light");
		combineLightShader = Resources::getShader("light_combine");
	}
}