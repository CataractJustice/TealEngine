#include "Bilboard.h"
#include "Graphics/Camera.h"
#include "GameNode/ComponentFactory.h"
namespace TealEngine
{
	Billboard::Billboard(const glm::vec3& up, bool flat) : up(up), flat(flat) {}
	void Billboard::update() 
	{
		Camera* camera = Camera::getPrimary();
		if(!camera) return;
		GameNode3D* node = (GameNode3D*)camera->getParent();
		if(flat) 
		{

			((GameNode3D*)(this->getParent()))->getRelativeTransform().lookAt(
				((GameNode3D*)(this->getParent()))->getRelativeTransform().getPosition() - 
				node->getWorldTransform().forward(),
				up
			);
		}
		else 
		{
			((GameNode3D*)(this->getParent()))->getRelativeTransform().lookAt(
				node->getWorldTransform().getPosition(),
				up
			);
		}
	}
} // namespace TealEngine