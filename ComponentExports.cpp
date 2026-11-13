#include "Editor/Components/EditorCameraController.h"
#include "Physics/Colliders/Colliders.h"
#include "Physics/Bodies/RigidBody.h"
#include "GameNode/ComponentFactory.h"
#include "Graphics/Mesh/MeshRenderer.h"
#include "Graphics/Mesh/MeshRendererInstanced.h"
#include "Editor/Components/NetworkTester.h"
#include "Graphics/GUI/TextInput.h"
#include "Components/Bilboard.h"
#include "Graphics/GUI/ScrollOnHover.h"
namespace TealEngine 
{
	EXPORT_COMPONENT(EditorCameraController);
	EXPORT_COMPONENT(RigidBody);
	EXPORT_COMPONENT(AABC);
	EXPORT_COMPONENT(BoxCollider);
	EXPORT_COMPONENT(SphereCollider);
	EXPORT_COMPONENT(Mesh3DRenderer);
	EXPORT_COMPONENT(Mesh3DRendererInstanced);
	EXPORT_COMPONENT(NetworkTester);
	EXPORT_COMPONENT(TextInput);
	EXPORT_COMPONENT(Billboard);
	EXPORT_COMPONENT(ScrollOnHover);
}