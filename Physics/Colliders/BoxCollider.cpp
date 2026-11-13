#include "BoxCollider.h"
#include "Core.h"
#include "GameNode/GameNode3D.h"
#include "Graphics/Mesh/MeshRenderer.h"

namespace TealEngine
{		
	BoxCollider::BoxCollider() 
	{
		this->from = glm::vec3(-1.0f);
		this->to = glm::vec3(1.0f);
		this->addProp(new FloatVecProp(glm::value_ptr(this->from), 3), "From");
		this->addProp(new FloatVecProp(glm::value_ptr(this->to), 3), "To");
	}

	void BoxCollider::getAABoundingBox(glm::vec3& from, glm::vec3& to)
	{
		auto* parent = this->getParentOfType<GameNode3D>();
		const glm::mat4 world = parent->getWorldTransform().getMatrix();

		// Local corners of the box
		glm::vec3 minLocal = this->from;
		glm::vec3 maxLocal = this->to;

		glm::vec3 corners[8] = {
			{minLocal.x, minLocal.y, minLocal.z},
			{maxLocal.x, minLocal.y, minLocal.z},
			{minLocal.x, maxLocal.y, minLocal.z},
			{maxLocal.x, maxLocal.y, minLocal.z},
			{minLocal.x, minLocal.y, maxLocal.z},
			{maxLocal.x, minLocal.y, maxLocal.z},
			{minLocal.x, maxLocal.y, maxLocal.z},
			{maxLocal.x, maxLocal.y, maxLocal.z}
		};

		glm::vec3 minP( std::numeric_limits<float>::max() );
		glm::vec3 maxP( std::numeric_limits<float>::lowest() );

		for (int i = 0; i < 8; i++) {
			// Transform corner
			glm::vec3 worldP = glm::vec3(world * glm::vec4(corners[i], 1.0f));

			minP = glm::min(minP, worldP);
			maxP = glm::max(maxP, worldP);
		}

		from = minP;
		to = maxP;
	}
} 
// namespace TealEngine