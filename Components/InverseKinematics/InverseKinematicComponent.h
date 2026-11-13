#pragma once
#include "Math/InverseKinematics.h"
#include "GameNode/GameNode3D.h"
#include "libs/glm/glm.hpp"
#include "Math/InverseKinematics.h"
namespace TealEngine
{
	class InverseKinematics 
	{
		public:
		struct Segment 
		{
			float armLength;
			GameNode3D* node;
		};
		private:
		glm::vec3 target;
		std::vector<InverseKinematics::Segment> segments;
		std::vector<InverseKinematicPoint> inverseKinematicPointsBuffer;
		public:
		void setTarget(glm::vec3 target);
		void pushSegment(GameNode3D* baseNode, float armLength);
		void popSegment();
		void setSegment(int index, GameNode3D* baseNode, float armLength);
		const InverseKinematics::Segment& getSegment(int index);
		int getSegmentCount();

		void iterate(glm::vec3 origin, int iterationCount = 1);
	};
} // namespace TealEngine
