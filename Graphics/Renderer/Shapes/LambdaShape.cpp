#include "LambdaShape.h"

namespace TealEngine 
{
	void LambdaShape::Draw()
	{
		this->f();
	}

	LambdaShape::LambdaShape(std::function<void()> f) : f(f) {}
}
