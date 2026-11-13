#pragma once

namespace TealEngine
{
	class Shape {
		private:

		public:
		virtual void Draw() = 0;
		virtual ~Shape() = default;
	};
} // namespace TealEngine