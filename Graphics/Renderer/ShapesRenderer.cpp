#include "ShapesRenderer.h"

namespace TealEngine
{
	void ShapesRenderer::renderAll() 
	{
		while(this->shapesQueue.size()) 
		{
			Shape* shape = shapesQueue.front();
			shapesQueue.pop();
			shape->Draw();
			delete shape;
		}
	}
} // namespace TealEngine
