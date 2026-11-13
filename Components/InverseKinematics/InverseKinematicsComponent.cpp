#include "InverseKinematicComponent.h"

namespace TealEngine 
{
	void InverseKinematics::setTarget(glm::vec3 target)
	{
		this->target = target;
	}

	void InverseKinematics::pushSegment(GameNode3D* baseNode, float armLength)
	{
		segments.push_back({armLength, baseNode});
	}

	void InverseKinematics::popSegment()
	{
		segments.pop_back();
	}

	void InverseKinematics::setSegment(int index, GameNode3D* baseNode, float armLength)
	{
		segments[index].node = baseNode;
		segments[index].armLength = armLength;
	}

	const InverseKinematics::Segment& InverseKinematics::getSegment(int index)
	{
		return segments[index];
	}

	// Get number of segments
	int InverseKinematics::getSegmentCount()
	{
		return static_cast<int>(segments.size());
	}

	void InverseKinematics::iterate(glm::vec3 origin, int iterationCount) 
	{
		inverseKinematicPointsBuffer.resize(segments.size() + 1);
		for(int i = 0; i < segments.size(); i++) 
		{
			InverseKinematicPoint& p = inverseKinematicPointsBuffer[i];
			const Transform& t = segments[i].node->getRelativeTransform();
			glm::vec3 position = t.getPosition();
			p.originX = position.x;
			p.originY = position.y;
			p.originZ = position.z;
			p.armLength = segments[i].armLength;
		}
		inverseKinematicPointsBuffer[0].originX = origin.x;
		inverseKinematicPointsBuffer[0].originY = origin.y;
		inverseKinematicPointsBuffer[0].originZ = origin.z;
		InverseKinematicPoint& lastPoint = inverseKinematicPointsBuffer[inverseKinematicPointsBuffer.size() - 1];
		lastPoint.armLength = -0.0001f;
		lastPoint.originX = target.x;
		lastPoint.originY = target.y;
		lastPoint.originZ = target.z;
		for(int i = 0; i < iterationCount; i++) 
		{
			inverseKinematicIteration(inverseKinematicPointsBuffer.data(), (float*)&target);
		}

		for(int i = 0; i < segments.size(); i++) 
		{
			InverseKinematicPoint& base = inverseKinematicPointsBuffer[i];
			InverseKinematicPoint& tip = inverseKinematicPointsBuffer[i + 1];
			Transform& t = segments[i].node->getRelativeTransform();
			Transform& pt = segments[i ? (i - 1) : 0].node->getRelativeTransform();
			t.setPosition(glm::vec3(base.originX, base.originY, base.originZ));
			t.lookAt(t.getPosition(), glm::vec3(tip.originX, tip.originY, tip.originZ), i ? pt.up() : glm::vec3(0.0f, 1.0f, 0.0f));
		}
	}
}