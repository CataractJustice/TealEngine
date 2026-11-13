#include "GameNode/ComponentProp.h"
#include "GameNode/GameNode.h"
namespace TealEngine
{
	class GlobalyNamedLongMask : public IProp 
	{
		private:
		std::string* names;
		public:
		GlobalyNamedLongMask(unsigned long long* maskPtr, std::string* names);
		Json get() override;
		void set(const Json& json) override;
		bool display(const char* label) override;
	};
} // namespace TealEngine
