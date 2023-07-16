#pragma once

#include "Shapes/Shape.h"
#include <queue>

namespace TealEngine
{
	class ShapesRenderer
	{
	private:
		std::queue<Shape*> shapesQueue;
	public:
		//renders and deletes all shapes that are currently in the shape render queue
		void renderAll();

		//adds a copy of the shape owned by this renderer into the shape render queue
		template<class T>
		void pushShape(T shape) 
		{
			this->shapesQueue.push(new T(shape));
		}
	};
	
} // namespace TealEngine
