#include "Editor/Components/EditorCameraController.h"
#include "Physics/Colliders/Colliders.h"
#include "Physics/Bodies/RigidBody.h"
#include "GameNode/ComponentFactory.h"
namespace TealEngine 
{
	EXPORT_COMPONENT(EditorCameraController);
	EXPORT_COMPONENT(RigidBody);
	EXPORT_COMPONENT(AABC);
	EXPORT_COMPONENT(BoxCollider);
	EXPORT_COMPONENT(SphereCollider);
}