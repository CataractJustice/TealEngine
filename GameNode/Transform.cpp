#include "Transform.h"
#include "Math/VecAngleAroundAxis.h"

using namespace glm;

namespace TealEngine {

	void Transform::onChange()
	{
		modelUpdateRequired = normalsModelUpdateRequired = true;
		lastModifyStamp = rand();
	}

	Transform Transform::operator *(const Transform& b)
	{
		Transform a = *this;
		a.onChange();
		a.position = glm::translate(b.position, vec3(b.rotation * b.scale * position[3]));
		a.rotation = b.rotation * a.rotation;
		a.scale[0][0] *= b.scale[0][0];
		a.scale[1][1] *= b.scale[1][1];
		a.scale[2][2] *= b.scale[2][2];
		return a;
	}

	Transform Transform::operator /(const Transform& b)
	{
		Transform a = *this;
		a.onChange();
		vec3 pos = a.getPosition();
		a.position = glm::translate(mat4(1.0f), vec3(vec4(pos, 1.0f) / b.rotation / b.scale)) / b.position;
		a.rotation = a.rotation / b.rotation;
		a.scale /= b.scale;
		return a;
	}

	Transform& Transform::operator *=(const Transform& b)
	{
		*this = *this * b;
		return *this;
	}

	Transform& Transform::operator /=(const Transform& b)
	{
		*this = *this / b;
		return *this;
	}

	bool Transform::operator ==(const Transform& b)
	{
		if (position[3][0] != b.position[3][0]) return false;
		if (position[3][1] != b.position[3][1]) return false;
		if (position[3][2] != b.position[3][2]) return false;
		if (scale[0][0] != b.scale[0][0]) return false;
		if (scale[1][1] != b.scale[1][1]) return false;
		if (scale[2][2] != b.scale[2][2]) return false;
		if (rotation != b.rotation) return false;
		return true;
	}

	bool Transform::operator != (const Transform& b)
	{
		return !(*this == b);
	}

	void Transform::setPosition(const vec3& vector)
	{
		position = glm::translate(mat4(1.0f), vector);
		onChange();
	}

	void Transform::translate(const vec3& vector)
	{
		position = glm::translate(position, vector);
		onChange();
	}

	vec3 Transform::getPosition()
	{
		return position[3];
	}

	float& Transform::getX() { return position[3][0]; }
	float& Transform::getY() { return position[3][1]; }
	float& Transform::getZ() { return position[3][2]; }

	void Transform::setScale(const vec3& vector)
	{
		scale = glm::scale(mat4(1.0f), vector);
		onChange();
	}

	void Transform::scaling(const vec3& vector)
	{
		scale = glm::scale(scale, vector);
		onChange();
	}

	vec3 Transform::getScale()
	{
		return scale * vec4(1.0f);
	}

	void Transform::rotate(float angle, const vec3& axis)
	{
		rotation = glm::rotate(rotation, angle, axis);
		onChange();
	}

	void Transform::globalRotate(float angle, const glm::vec3& axis)
	{
		if (angle && (axis.x || axis.y || axis.z))
		{
			rotation = glm::rotate(glm::mat4(1.0f), angle, axis) * rotation;
			onChange();
		}
	}

	void Transform::setRotation(float angle, const vec3& axis)
	{
		rotation = glm::rotate(mat4(1.0f), angle, axis);
		onChange();
	}

	void Transform::setRotation(glm::quat q) 
	{
		rotation = glm::mat4(q);
		onChange();
	}

	void Transform::setRotationMatrix(glm::mat4 rotation) 
	{
		this->rotation = rotation;
	}

	glm::quat Transform::getRotation()
	{
		return quat_cast(rotation);
	}

	glm::vec3 Transform::getXYZRotation() 
	{
		glm::vec3 angles;
		glm::quat q = getRotation();
		float sinr_cosp = 2 * (q.w * q.x + q.y * q.z);
		float cosr_cosp = 1 - 2 * (q.x * q.x + q.y * q.y);
		angles.z = std::atan2(sinr_cosp, cosr_cosp);

		float sinp = std::sqrt(1 + 2 * (q.w * q.y - q.x * q.z));
		float cosp = std::sqrt(1 - 2 * (q.w * q.y - q.x * q.z));
		angles.x = 2 * std::atan2(sinp, cosp) - M_PI / 2;

		float siny_cosp = 2 * (q.w * q.z + q.x * q.y);
		float cosy_cosp = 1 - 2 * (q.y * q.y + q.z * q.z);
		angles.y = std::atan2(siny_cosp, cosy_cosp);
		return angles;
	}

	glm::mat4 Transform::getRotationMatrix() 
	{
		return this->rotation;
	}


	void Transform::lookAt(const glm::vec3& target, const glm::vec3& up) 
	{
		this->rotation = glm::lookAt(glm::vec3(0.0f), target, up);
		onChange();
	}
	void Transform::lookAt(const glm::vec3& from, const glm::vec3& target, const glm::vec3& up) 
	{
		lookAt(target - from, up);
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

	const mat4& Transform::getMatrix()
	{
		if (modelUpdateRequired)
		{
			model = rotation;
			model[0] *= scale[0].x;
			model[1] *= scale[1].y;
			model[2] *= scale[2].z;

			model[3].x += position[3].x;
			model[3].y += position[3].y;
			model[3].z += position[3].z;
			modelUpdateRequired = false;
		}
		return model;
	}

	const mat4& Transform::getNormalsModel()
	{
		if (normalsModelUpdateRequired)
		{
			//aoaoaoaoaaaammmmmmmmmmmmmmmmm nested max and min
			normalsModel = (glm::max(abs(scale[0][0]), glm::max(abs(scale[1][1]), abs(scale[2][2]))) - glm::min(abs(scale[0][0]), glm::min(abs(scale[1][1]), abs(scale[2][2]))) < 0.1f) ? getMatrix() : transpose(inverse(getMatrix()));
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

	vec3 Transform::pointFromLocalToTransform(vec3 pos, Transform& transform, bool ignoreScale)
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
		lastModifyStamp++;
	}

	unsigned int Transform::getLastModifyStamp() 
	{
		return lastModifyStamp;
	}

	Transform::Transform()
	{
		lastModifyStamp = rand();
		position = glm::translate(mat4(1.0f), vec3(0.0f));
		scale = glm::scale(mat4(1.0f), vec3(1.0f));
		rotation = glm::rotate(mat4(1.0f), 0.0f, vec3(1.0f));
		onChange();
	}
}