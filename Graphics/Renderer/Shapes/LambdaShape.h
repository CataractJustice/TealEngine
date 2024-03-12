#include "Shape.h"
#include <functional>

namespace TealEngine 
{
	class LambdaShape : public Shape 
	{
		private:
		std::function<void()> f;
		public:
		void Draw() override;
		LambdaShape(std::function<void()> f);
	};
}
