#include "GameNode/ComponentProp.h"
#include "GameNode/Transform.h"

namespace TealEngine 
{
	class TransformProp : public IProp 
	{
		private:

		public:
		TransformProp(Transform* transformPtr);
		Json get() override;
		void set(const Json& json) override;
		bool display(const char* label) override;
	};
}