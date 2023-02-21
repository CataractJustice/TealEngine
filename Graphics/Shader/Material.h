#include  "ShaderProgram.h"
#include "GameNode/Component.h"
#include "Graphics/Texture/Texture.h"

namespace TealEngine 
{
	class Material : public Component
	{
		private:
		ShaderProgram* shader, *oldShader;
		ShaderProgram* material;
		std::map<std::string, Texture*> textures;
		std::string materialName;
		std::string oldName;
		std::string name;
		std::function<void()> addMaterial;
		public:
		Material();	
		void onPropSet(const std::string& propName) override;
	};
}