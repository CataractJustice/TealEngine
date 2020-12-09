#pragma once
#include "../../Event.h"
#include "../../../libs/glm/vec3.hpp"
using namespace TealEngine;
class VoxelDataModifyEvent : public Event
{
public:
	glm::ivec3 position;
	Voxel oldVoxel, newVoxel;
	VoxelDataModifyEvent(glm::ivec3 position, Voxel oldVoxel, Voxel newVoxel) : position(position), oldVoxel(oldVoxel), newVoxel(newVoxel)
	{

	}

	EVENT_TYPE(VOXEL_DATA_MODIFY);
	EVENT_CATEGORY(MISC_EVENTS);
};