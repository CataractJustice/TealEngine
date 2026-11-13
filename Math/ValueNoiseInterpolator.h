#include "SmoothInterpolationAccumulator.h"
#include <vector>
#include "Noise.h"
namespace TealEngine
{
	class ValueNoiseInterpolator 
	{
		private:
		std::vector<float> values;
		int index;
		int step;
		int stepsPerValue;
		SmoothInterpolationAccumulator<float> interpolator;
		public:
		void reset(int valuesCount, int stepsPerValue, glm::ivec3 noiseOrigin, glm::ivec3 noiseStride);
		float value();
		float next();
		void goTo(int step);
	};
	class ValueNoiseInterpolator2D 
	{
		private:
		std::vector<ValueNoiseInterpolator> values;
		SmoothInterpolationAccumulator<float> interpolator;
		int stepsPerValue;
		int step;
		int index;
		public:
		void reset(int width, int height, int stepsPerValue, glm::ivec3 noiseOrigin, glm::ivec3 noiseStride);
		float value();
		float nextY();
		void nextX();
		void goTo(int x, int y);
	};
	class ValueNoiseInterpolator3D 
	{
		private:
		std::vector<ValueNoiseInterpolator2D> values;
		SmoothInterpolationAccumulator<float> interpolator;
		int stepsPerValue;
		int step;
		int index;
		public:
		void reset(int width, int height, int depth, int stepsPerValue, glm::ivec3 noiseOrigin, glm::ivec3 noiseStride);
		float value();
		float nextZ();
		void nextY();
		void nextX();
		void goTo(int x, int y, int z);
	};
} // namespace TealEngine
