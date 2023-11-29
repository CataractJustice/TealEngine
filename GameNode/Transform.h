#pragma once
#include "libs/glm/vec3.hpp"
#include "libs/glm/matrix.hpp"
#include "libs/glm/ext/matrix_transform.hpp"
#include "libs/glm/gtc/quaternion.hpp"

namespace TealEngine {
	class Transform
	{
	protected:
		glm::mat4 position, scale, rotation;
		mutable glm::mat4 model, normalsModel;
		mutable bool modelUpdateRequired, normalsModelUpdateRequired;
		unsigned int lastModifyStamp;

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

		glm::vec3 getPosition() const;

		float getX() const;
		float getY() const;
		float getZ() const;

		void setScale(const glm::vec3& vector);

		void scaling(const glm::vec3& vector);

		glm::vec3 getScale() const;

		void rotate(float angle, const glm::vec3& axis);

		void globalRotate(float angle, const glm::vec3& axis);

		void setRotation(float angle, const glm::vec3& axis);
		void setRotationMatrix(glm::mat4 rotation);

		void setRotation(glm::quat);

		void lookAt(const glm::vec3& target, const glm::vec3& up);
		void lookAt(const glm::vec3& position, const glm::vec3& target, const glm::vec3& up);

		glm::quat getRotation() const;
		glm::mat4 getRotationMatrix() const;

		glm::vec3 forward() const;

		glm::vec3 up() const;

		glm::vec3 right() const;

		const glm::mat4& getMatrix() const;
		const glm::mat4& getNormalsModel() const;

		glm::vec3 pointFromGlobalToLocal(glm::vec3 pos, bool ignoreScale = false) const;

		glm::vec3 pointFromLocalToGlobal(glm::vec3 pos, bool ignoreScale = false) const;

		glm::vec3 pointFromLocalToTransform(glm::vec3 pos, Transform& transform, bool ignoreScale = false) const;

		void setMatrix(glm::mat4 matrix);

		unsigned int getLastModifyStamp() const;

		Transform();
	};
}