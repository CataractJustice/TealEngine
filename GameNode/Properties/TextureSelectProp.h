#include "GameNode/ComponentProp.h"
#include "Graphics/Texture/Texture.h"
namespace TealEngine 
{
	class TextureSelectProp : public StringKeyMapValuePointerProp<Texture> 
	{
		public:
		using StringKeyMapValuePointerProp<Texture>::StringKeyMapValuePointerProp;
		bool display(const char* label) override;
	};
}