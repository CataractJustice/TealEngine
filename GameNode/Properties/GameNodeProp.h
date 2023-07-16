#include "GameNode/ComponentProp.h"
#include "GameNode/GameNode.h"
namespace TealEngine
{
	class GameNodeProp : public IProp 
	{
		private:
		
		public:
		GameNodeProp(GameNode** nodePtr);
		Json get() override;
		void set(const Json& json) override;
		bool display(const char* label) override;
	};
} // namespace TealEngine
