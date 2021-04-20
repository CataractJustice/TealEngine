#pragma once
#include<iostream>
#include "../libs/glm/vec3.hpp"
#include "../libs/glm/matrix.hpp"
#include "../libs/glm/ext/matrix_transform.hpp"
#include "../libs/glm/gtc/quaternion.hpp"
using namespace glm;

namespace TealEngine {
	class Transform
	{
	protected:
		mat4 position, rotation, scale, model, normalsModel;

		bool modelUpdateRequired, normalsModelUpdateRequired;
		void onChange() 
		{
			modelUpdateRequired = normalsModelUpdateRequired = true;
		}

	public:

		Transform operator *(Transform b)
		{
			Transform a = *this;
			a.onChange();
			vec3 pos = a.getPosition();
			a.position = glm::translate(b.position, vec3(b.rotation * b.scale * vec4(pos, 1.0f)));
			a.rotation = b.rotation * a.rotation;
			a.scale *= b.scale;
			return a;
		}

		Transform operator /(Transform b)
		{
			Transform a = *this;
			a.onChange();
			vec3 pos = a.getPosition();
			a.position = glm::translate(mat4(1.0f), vec3(vec4(pos, 1.0f) / b.rotation / b.scale)) / b.position;
			a.rotation = a.rotation / b.rotation;
			a.scale /= b.scale;
			return a;
		}

		Transform operator *=(Transform b)
		{
			*this = *this / b;
			return *this;
		}

		Transform operator /=(Transform b)
		{
			*this = *this / b;
			return *this;
		}

		bool operator ==(Transform b) 
		{
			return
				(
					position == b.position &&
					rotation == b.rotation &&
					scale == b.scale
				);
		}

		bool operator != (Transform b) 
		{
			return !(*this == b);
		}

		//position
		void setPosition(vec3 vector)
		{
			position = glm::translate(mat4(1.0f), vector);
			onChange();
		}

		void translate(vec3 vector)
		{
			position = glm::translate(position, vector);
			onChange();
		}

		vec3 getPosition()
		{
			return position * vec4(0.0f, 0.0f, 0.0f, 1.0f);
		}

		//scale
		void setScale(vec3 vector)
		{
			scale = glm::scale(mat4(1.0f), vector);
			onChange();
		}

		void scaling(vec3 vector)
		{
			scale = glm::scale(scale, vector);
			onChange();
		}

		vec3 getScale()
		{
			return scale * vec4(1.0f);
		}


		//roatation
		void rotate(float angle, vec3 axis)
		{
			rotation = glm::rotate(rotation, radians(angle), axis);
			onChange();
		}

		void setRotation(float angle, vec3 axis)
		{
			rotation = glm::rotate(mat4(1.0f), radians(angle), axis);
			onChange();
		}

		glm::quat getRotation() 
		{
			return quat_cast(rotation);
		}

		vec3 forward()
		{
			return rotation * vec4(0.0f, 0.0f, 1.0f, 1.0f);
		}

		vec3 up()
		{
			return rotation * vec4(0.0f, 1.0f, 0.0f, 1.0f);
		}

		vec3 right()
		{
			return rotation * vec4(1.0f, 0.0f, 0.0f, 1.0f);
		}

		mat4 getMatrix()
		{
			if (modelUpdateRequired)
			{
				model = position * rotation * scale;
				modelUpdateRequired = false;
			}
			return model;
		}

		mat4 getNormalsModel()
		{
			if (normalsModelUpdateRequired)
			{
				normalsModel = transpose(inverse(getMatrix()));
				normalsModelUpdateRequired = false;
			}
			return normalsModel;
		}

		vec3 pointFromGlobalToLocal(vec3 pos, bool ignoreScale = false) 
		{
			pos -= this->getPosition();
			pos = vec3(dot(pos, this->right()), dot(pos, this->up()), dot(pos, this->forward()));
			if(!ignoreScale) pos /= this->getScale();
			return pos;
		}

		vec3 pointFromLocalToGlobal(vec3 pos, bool ignoreScale = false) 
		{
			if(!ignoreScale) pos *= this->getScale();
			pos = this->right() * pos.x + this->up() * pos.y + this->forward() * pos.z;
			pos += this->getPosition();
			return pos;
		}

		vec3 pointFromLocalToTransform(vec3 pos, Transform transform, bool ignoreScale = false) 
		{
			transform.pointFromGlobalToLocal(pointFromLocalToGlobal(pos, ignoreScale), ignoreScale);
		}

		void setMatrix(mat4 matrix)
		{
			position = glm::translate(mat4(1.0f), vec3(matrix * vec4(0.0f, 0.0f, 0.0f, 1.0f)));
			matrix /= position;
			rotation = glm::lookAt(vec3(0.0f), vec3(matrix * vec4(0.0f, 0.0f, 1.0f, 1.0f)), vec3(matrix * vec4(0.0f, 1.0f, 0.0f, 1.0f)));
			matrix /= rotation;
			scale = glm::scale(mat4(1.0f), vec3(matrix * vec4(1.0f)));
			onChange();
		}

		Transform()
		{
			position = glm::translate(mat4(1.0f), vec3(0.0f));
			scale = glm::scale(mat4(1.0f), vec3(1.0f));
			rotation = glm::rotate(mat4(1.0f), 0.0f, vec3(1.0f));
			onChange();
		}
	};
}