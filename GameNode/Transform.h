#pragma once
#include "libs/glm/vec3.hpp"
#include "libs/glm/matrix.hpp"
#include "libs/glm/ext/matrix_transform.hpp"
#include "libs/glm/gtc/quaternion.hpp"

namespace TealEngine {
	class Transform
	{
	protected:
		glm::mat4 position, scale, rotation, model, normalsModel;
		unsigned int lastModifyStamp;
		bool modelUpdateRequired, normalsModelUpdateRequired;

		void onChange();

	public:

		Transform operator *(const Transform& b);

		Transform operator /(const Transform& b);

		Transform& operator *=(const Transform& b);

		Transform& operator /=(const Transform& b);

		bool operator ==(const Transform& b);

		bool operator != (const Transform& b);

		void setPosition(const glm::vec3& vector);

		void translate(const glm::vec3& vector);

		glm::vec3 getPosition();

		float& getX();
		float& getY();
		float& getZ();

		void setScale(const glm::vec3& vector);

		void scaling(const glm::vec3& vector);

		glm::vec3 getScale();

		void rotate(float angle, const glm::vec3& axis);

		void globalRotate(float angle, const glm::vec3& axis);

		void setRotation(float angle, const glm::vec3& axis);

		void setRotation(glm::quat);

		void lookAt(const glm::vec3& target, const glm::vec3& up);
		void lookAt(const glm::vec3& position, const glm::vec3& target, const glm::vec3& up);

		glm::quat getRotation();

		glm::vec3 forward();

		glm::vec3 up();

		glm::vec3 right();

		const glm::mat4& getMatrix();
		const glm::mat4& getNormalsModel();

		glm::vec3 pointFromGlobalToLocal(glm::vec3 pos, bool ignoreScale = false);

		glm::vec3 pointFromLocalToGlobal(glm::vec3 pos, bool ignoreScale = false);

		glm::vec3 pointFromLocalToTransform(glm::vec3 pos, Transform& transform, bool ignoreScale = false);

		void setMatrix(glm::mat4 matrix);

		unsigned int getLastModifyStamp();

		Transform();
	};
}