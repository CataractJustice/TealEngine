#pragma once
#include "libs/glm/vec3.hpp"
#include "libs/glm/matrix.hpp"
#include "libs/glm/ext/matrix_transform.hpp"
#include "libs/glm/gtc/quaternion.hpp"

namespace TealEngine {
	class Transform
	{
	protected:
		glm::mat4 position, rotation, scale, model, normalsModel;

		bool modelUpdateRequired, normalsModelUpdateRequired;

		void onChange();

	public:

		Transform operator *(Transform b);

		Transform operator /(Transform b);

		Transform& operator *=(Transform b);

		Transform& operator /=(Transform b);

		bool operator ==(Transform b);

		bool operator != (Transform b);

		void setPosition(glm::vec3 vector);

		void translate(glm::vec3 vector);

		glm::vec3 getPosition();

		void setScale(glm::vec3 vector);

		void scaling(glm::vec3 vector);

		glm::vec3 getScale();

		void rotate(float angle, glm::vec3 axis);

		void setRotation(float angle, glm::vec3 axis);

		void setRotation(glm::quat);

		glm::quat getRotation();

		glm::vec3 forward();

		glm::vec3 up();

		glm::vec3 right();

		glm::mat4 getMatrix();
		glm::mat4 getNormalsModel();

		glm::vec3 pointFromGlobalToLocal(glm::vec3 pos, bool ignoreScale = false);

		glm::vec3 pointFromLocalToGlobal(glm::vec3 pos, bool ignoreScale = false);

		glm::vec3 pointFromLocalToTransform(glm::vec3 pos, Transform transform, bool ignoreScale = false);

		void setMatrix(glm::mat4 matrix);

		Transform();
	};
}