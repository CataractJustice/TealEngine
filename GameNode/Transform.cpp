#include "Transform.h"

using namespace glm;

namespace TealEngine {

	void Transform::onChange()
	{
		modelUpdateRequired = normalsModelUpdateRequired = true;
	}

	Transform Transform::operator *(Transform b)
	{
		Transform a = *this;
		a.onChange();
		vec3 pos = a.getPosition();
		a.position = glm::translate(b.position, vec3(b.rotation * b.scale * vec4(pos, 1.0f)));
		a.rotation = b.rotation * a.rotation;
		a.scale *= b.scale;
		return a;
	}

	Transform Transform::operator /(Transform b)
	{
		Transform a = *this;
		a.onChange();
		vec3 pos = a.getPosition();
		a.position = glm::translate(mat4(1.0f), vec3(vec4(pos, 1.0f) / b.rotation / b.scale)) / b.position;
		a.rotation = a.rotation / b.rotation;
		a.scale /= b.scale;
		return a;
	}

	Transform& Transform::operator *=(Transform b)
	{
		*this = *this / b;
		return *this;
	}

	Transform& Transform::operator /=(Transform b)
	{
		*this = *this / b;
		return *this;
	}

	bool Transform::operator ==(Transform b)
	{
		return
			(
				position == b.position &&
				rotation == b.rotation &&
				scale == b.scale
				);
	}

	bool Transform::operator != (Transform b)
	{
		return !(*this == b);
	}

	void Transform::setPosition(vec3 vector)
	{
		position = glm::translate(mat4(1.0f), vector);
		onChange();
	}

	void Transform::translate(vec3 vector)
	{
		position = glm::translate(position, vector);
		onChange();
	}

	vec3 Transform::getPosition()
	{
		return position * vec4(0.0f, 0.0f, 0.0f, 1.0f);
	}

	void Transform::setScale(vec3 vector)
	{
		scale = glm::scale(mat4(1.0f), vector);
		onChange();
	}

	void Transform::scaling(vec3 vector)
	{
		scale = glm::scale(scale, vector);
		onChange();
	}

	vec3 Transform::getScale()
	{
		return scale * vec4(1.0f);
	}

	void Transform::rotate(float angle, vec3 axis)
	{
		rotation = glm::rotate(rotation, radians(angle), axis);
		onChange();
	}

	void Transform::setRotation(float angle, vec3 axis)
	{
		rotation = glm::rotate(mat4(1.0f), radians(angle), axis);
		onChange();
	}

	glm::quat Transform::getRotation()
	{
		return quat_cast(rotation);
	}

	vec3 Transform::forward()
	{
		return rotation * vec4(0.0f, 0.0f, 1.0f, 1.0f);
	}

	vec3 Transform::up()
	{
		return rotation * vec4(0.0f, 1.0f, 0.0f, 1.0f);
	}

	vec3 Transform::right()
	{
		return rotation * vec4(1.0f, 0.0f, 0.0f, 1.0f);
	}

	mat4 Transform::getMatrix()
	{
		if (modelUpdateRequired)
		{
			model = position * rotation * scale;
			modelUpdateRequired = false;
		}
		return model;
	}

	mat4 Transform::getNormalsModel()
	{
		if (normalsModelUpdateRequired)
		{
			normalsModel = transpose(inverse(getMatrix()));
			normalsModelUpdateRequired = false;
		}
		return normalsModel;
	}

	vec3 Transform::pointFromGlobalToLocal(vec3 pos, bool ignoreScale)
	{
		pos -= this->getPosition();
		pos = vec3(dot(pos, this->right()), dot(pos, this->up()), dot(pos, this->forward()));
		if (!ignoreScale) pos /= this->getScale();
		return pos;
	}

	vec3 Transform::pointFromLocalToGlobal(vec3 pos, bool ignoreScale)
	{
		if (!ignoreScale) pos *= this->getScale();
		pos = this->right() * pos.x + this->up() * pos.y + this->forward() * pos.z;
		pos += this->getPosition();
		return pos;
	}

	vec3 Transform::pointFromLocalToTransform(vec3 pos, Transform transform, bool ignoreScale)
	{
		return transform.pointFromGlobalToLocal(pointFromLocalToGlobal(pos, ignoreScale), ignoreScale);
	}

	void Transform::setMatrix(mat4 matrix)
	{
		position = glm::translate(mat4(1.0f), vec3(matrix * vec4(0.0f, 0.0f, 0.0f, 1.0f)));
		matrix /= position;
		rotation = glm::lookAt(vec3(0.0f), vec3(matrix * vec4(0.0f, 0.0f, 1.0f, 1.0f)), vec3(matrix * vec4(0.0f, 1.0f, 0.0f, 1.0f)));
		matrix /= rotation;
		scale = glm::scale(mat4(1.0f), vec3(matrix * vec4(1.0f)));
		onChange();
	}

	Transform::Transform()
	{
		position = glm::translate(mat4(1.0f), vec3(0.0f));
		scale = glm::scale(mat4(1.0f), vec3(1.0f));
		rotation = glm::rotate(mat4(1.0f), 0.0f, vec3(1.0f));
		onChange();
	}
}