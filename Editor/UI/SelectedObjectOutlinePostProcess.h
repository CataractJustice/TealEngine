#include "GameNode/Component.h"

namespace TealEngine 
{
	class SelectedObjectOutlinePostProcess : public Component 
	{
		public:
		void postProcess(unsigned int unlitColor, unsigned int litColor, unsigned int position, unsigned int normal, unsigned int specular, unsigned int light) override;
		static ShaderProgram outlineShader;
		static void loadShader();
	};
}