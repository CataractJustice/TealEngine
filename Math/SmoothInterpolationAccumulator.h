#include <math.h>
namespace TealEngine
{
	template<typename T>
	class SmoothInterpolationAccumulator 
	{
		private:
		T acceleration;
		T velocity;
		T position;
		int step;
		int steps;
		int stepshalf;
		int stepsmid;
		T from;
		public:
		SmoothInterpolationAccumulator(T from, T to, int steps)
		{
			reset(from, to, steps);
		}
		SmoothInterpolationAccumulator() {};

		void reset(T from, T to, int steps) 
		{
			velocity = 0;
			position = from;
			this->from = from;
			step = 0;
			this->steps = steps;
			stepshalf = steps / 2;
			stepsmid = steps / 2 + (steps % 2);
			acceleration = (to - from) / (stepshalf * stepshalf);
		}
		T value() const 
		{
			return position;
		}
		T next() 
		{
			if(step == stepsmid) acceleration = -acceleration;
			velocity += acceleration;
			position += velocity;
			step++;
			return position;
		}
		void goTo(int stepNumber) 
		{
			velocity = 0;
			step = 0;
			position = from;
			for(int i = 0; i < stepNumber; i++) next();
		}
	};

	template<typename T>
	class SmoothInterpolationAccumulator2D 
	{
		private:
		SmoothInterpolationAccumulator<T> accY;
		SmoothInterpolationAccumulator<T> accX0;
		SmoothInterpolationAccumulator<T> accX1;
		int steps;
		public:
		SmoothInterpolationAccumulator2D(T v00, T v01, T v10, T v11, int steps) :
		accX0(v00, v10, steps),
		accX1(v01, v11, steps),
		accY(v00, v01, steps),
		steps(steps)
		{
		}
		void reset(T v00, T v01, T v10, T v11, int steps) 
		{
			accX0.reset(v00, v10, steps);
			accX1.reset(v01, v11, steps);
			accY.reset(v00, v01, steps);
		}
		T value() const 
		{
			return accY.value();
		}
		T nextY() 
		{
			return accY.next();
		}
		void nextX() 
		{
			accY.reset(accX0.next(), accX1.next(), steps);
		}
	};

	template <typename T>
	class SmoothInterpolationAccumulator3D
	{
		private:
		SmoothInterpolationAccumulator2D<T> planeX;
		SmoothInterpolationAccumulator<T> yz00;
		SmoothInterpolationAccumulator<T> yz01;
		SmoothInterpolationAccumulator<T> yz10;
		SmoothInterpolationAccumulator<T> yz11;
		int steps;
		public:
		SmoothInterpolationAccumulator3D(T v000, T v001, T v010, T v011, T v100, T v101, T v110, T v111, int steps) :
		yz00(v000, v100, steps),
		yz01(v001, v101, steps),
		yz10(v010, v110, steps),
		yz11(v011, v111, steps),
		steps(steps)
		{
			
		}
		T value() const 
		{
			return planeX.value();
		}
		T nextZ() 
		{
			return planeX.nextY();
		}
		void nextY() 
		{
			planeX.nextX();
		}
		void nextX() 
		{
			planeX.reset(
				yz00.next(),
				yz01.next(),
				yz10.next(),
				yz11.next(),
				steps
			);
		}
	};
} // namespace TealEngine
