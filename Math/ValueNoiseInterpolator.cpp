#include "ValueNoiseInterpolator.h"
namespace TealEngine
{
	void ValueNoiseInterpolator::reset(int valuesCount, int stepsPerValue, glm::ivec3 noiseOrigin, glm::ivec3 noiseStride) 
	{
		this->stepsPerValue = stepsPerValue;
		values.resize(valuesCount);
		for(int i = 0; i < valuesCount; i++) 
		{
			float& v = values[i];
			v = Noise::get(noiseOrigin);
			noiseOrigin += noiseStride;
		}
		index = 0;
		step = 0;
		interpolator.reset(values[0], values[1], stepsPerValue);
	}
	float ValueNoiseInterpolator::value() 
	{
		return interpolator.value();
	}
	
	float ValueNoiseInterpolator::next() 
	{
		if(step == stepsPerValue) 
		{
			index++;
			step = 0;
			interpolator.reset(values[index], values[index + 1], stepsPerValue);
		}
		else 
		{
			interpolator.next();
		}
		step++;
		return value();
	}

	void ValueNoiseInterpolator::goTo(int stepNumber) 
	{
		step = stepNumber % stepsPerValue;
		index = stepNumber / stepsPerValue;
		interpolator.reset(values[index], values[index + 1], stepsPerValue);
		interpolator.goTo(step);
	}

	void ValueNoiseInterpolator2D::reset(int width, int height, int stepsPerValue, glm::ivec3 noiseOrigin, glm::ivec3 noiseStride) 
	{
		index = 0;
		step = 0;
		this->stepsPerValue = stepsPerValue;
		values.resize(height);
		for(int i = 0; i < height; i++) 
		{
			values[i].reset(width, stepsPerValue, noiseOrigin + noiseStride * glm::ivec3(0, i, 0), noiseStride * glm::ivec3(1, 0, 0));
		}
		interpolator.reset(values[index].value(), values[index + 1].value(), stepsPerValue);
	}
	float ValueNoiseInterpolator2D::value() 
	{
		return interpolator.value();
	}
	float ValueNoiseInterpolator2D::nextY() 
	{
		if(step == stepsPerValue) 
		{
			index++;
			step = 0;
			interpolator.reset(values[index].value(), values[index + 1].value(), stepsPerValue);
		}
		else 
		{
			interpolator.next();
		}
		step++;
		return value();
	}
	void ValueNoiseInterpolator2D::nextX() 
	{
		for(auto& v : values) 
		{
			v.next();
		}
		index = 0;
		step = 0;
		interpolator.reset(values[index].value(), values[index + 1].value(), stepsPerValue);
	}

	void ValueNoiseInterpolator2D::goTo(int x, int y) 
	{
		step = y % stepsPerValue;
		index = y / stepsPerValue;
		for(auto& v : values) 
		{
			v.goTo(x);
		}
		interpolator.reset(values[index].value(), values[index + 1].value(), stepsPerValue);
		interpolator.goTo(step);
	}

	void ValueNoiseInterpolator3D::reset(int width, int height, int depth, int stepsPerValue, glm::ivec3 noiseOrigin, glm::ivec3 noiseStride) 
	{
		index = 0;
		step = 0;
		this->stepsPerValue = stepsPerValue;
		values.resize(depth);
		for(int i = 0; i < depth; i++) 
		{
			values[i].reset(width, height, stepsPerValue, noiseOrigin + noiseStride * glm::ivec3(0, 0, i), noiseStride * glm::ivec3(1, 1, 0));
		}
		interpolator.reset(values[0].value(), values[1].value(), stepsPerValue);
	}

	float ValueNoiseInterpolator3D::value() 
	{
		return interpolator.value();
	}

	float ValueNoiseInterpolator3D::nextZ() 
	{
		if(step == stepsPerValue) 
		{
			index++;
			step = 0;
			interpolator.reset(values[index].value(), values[index + 1].value(), stepsPerValue);
		}
		else 
		{
			interpolator.next();
		}
		step++;
		return value();
	}

	void ValueNoiseInterpolator3D::nextY() 
	{
		for(auto& v : values) 
		{
			v.nextY();
		}
		step = 0;
		index = 0;
		interpolator.reset(values[0].value(), values[1].value(), stepsPerValue);
	}

	void ValueNoiseInterpolator3D::nextX() 
	{
		for(auto& v : values) 
		{
			v.nextX();
		}
		step = 0;
		index = 0;
		interpolator.reset(values[0].value(), values[1].value(), stepsPerValue);
	}

	void ValueNoiseInterpolator3D::goTo(int x, int y, int z) 
	{
		for(int i = 0; i < x; i++) 
		{
			nextX();
		}
		for(int j = 0; j < y; j++) 
		{
			nextY();
		}
		for(int k = 0; k < z; k++) 
		{
			nextZ();
		}
		return;
		for(int i = 0; i < y; i++) nextY();
		for(int i = 0; i < z; i++) nextZ();
		return; 
		step = z % stepsPerValue;
		index = z / stepsPerValue;
		for(auto& v : values) 
		{
			v.goTo(x, y);
		}
		interpolator.reset(values[index].value(), values[index + 1].value(), stepsPerValue);
		interpolator.goTo(step);
	}
} // namespace TealEngine
