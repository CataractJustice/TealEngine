#pragma once
#include "Colliders.h"
#include "Math/IndexConvert.h"
#include "Math/Interpolation.h"
#include "GameNode/GameNode3D.h"

namespace TealEngine 
{
	class HeightCollider : public Collider 
	{
	private:
		int w, l;
		float* hv;
		bool pointer;
	public:
		glm::vec3 scale;
		//uses values from given pointer without deleting data on destruction
		//hv - array of height values  of (w * l) length
		//w - width of array
		//l - length of array
		void setHeightMapPointer(float* hv, int w, int l) 
		{
			this->hv = hv;
			this->w = w;
			this->l = l;
			pointer = true;
		}

		//copies and saves values from given pointer, deletes data on destruction
		//hv - array of height values  of (w * l) length
		//w - width of array
		//l - length of array
		void setHeightMap(float* hv, int w, int l) 
		{
			if (this->hv && !pointer) delete[] this->hv;
			memcpy(this->hv, hv, w * l * sizeof(float));
			pointer = false;
		}

		float getHeightAtIndex(glm::ivec2 ind) 
		{
			if (ind.x >= 0 && ind.x < w && ind.y >= 0 && ind.y < l)
				return hv[a2to1ind(ind.x, ind.y, l)];
			else
				return -INFINITY;
		}

		float getHeightAtPosition(glm::vec2 pos) 
		{
			Transform& wt = this->getParentOfType<GameNode3D>()->getWorldTransform();
			pos -= glm::vec2(wt.getX(), wt.getZ());
			pos /= glm::vec2(scale.x, scale.z);
			return linear2DGridInterpolation(glm::mod(pos, 1.0f), 
				getHeightAtIndex(glm::ivec2(pos) + glm::ivec2(0, 0)), 
				getHeightAtIndex(glm::ivec2(pos) + glm::ivec2(1, 0)),
				getHeightAtIndex(glm::ivec2(pos) + glm::ivec2(0, 1)),
				getHeightAtIndex(glm::ivec2(pos) + glm::ivec2(1, 1))
			) * scale.y + wt.getY();
		}

		glm::vec3 getNormalAtPosition(glm::vec2 pos, float step) 
		{
			glm::vec3 p0 = glm::vec3(0.0f, getHeightAtPosition(pos), 0.0f);
			glm::vec3 v1 = glm::vec3(step, getHeightAtPosition(pos + glm::vec2(step, 0.0f)), 0.0f) - p0;
			glm::vec3 v2 = glm::vec3(0.0f, getHeightAtPosition(pos + glm::vec2(0.0f, step)), step) - p0;
			return glm::normalize(glm::cross(v2, v1));
		}

		HeightCollider() 
		{
			w = l = 0;
			hv = nullptr;
			pointer = true;
			scale = glm::vec3(1.0f);
		}

		~HeightCollider() 
		{
			if (hv && !pointer) delete[] hv;
		}
		Collider::ColliderType getColliderType() { return Collider::ColliderType::HEIGHTMAP; }
	};
}