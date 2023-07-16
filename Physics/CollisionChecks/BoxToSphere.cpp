#include "Physics/Colliders/BoxCollider.h"
#include "Physics/Colliders/SphereCollider.h"
#include "Physics/Collision.h"
#include "Physics/CollisionSolver.h"
#include "GameNode/GameNode3D.h"

namespace TealEngine 
{
	std::pair<Collision, Collision> CollisionSolver::BoxToSphere(BoxCollider* bc, SphereCollider* sc) 
	{
		Transform& bt = bc->getParentOfType<GameNode3D>()->getWorldTransform();
		Transform& st = sc->getParentOfType<GameNode3D>()->getWorldTransform();
		glm::vec3 blright = bt.right();
		glm::vec3 blup = bt.up();
		glm::vec3 blforward = bt.forward();

		//box relative origin
		glm::vec3 bro = (bc->from + bc->to) * bt.getScale() * 0.5f;
		//box scale / 2.
		glm::vec3 bscale = (bc->to - bc->from) * bt.getScale() * 0.5f;

		//sphere pos in box coordinate system
		glm::vec3 slp = bt.pointFromGlobalToLocal(st.getPosition(), true);
		slp -= bro;
		glm::vec3 sslp = glm::sign(slp);
		slp = glm::abs(slp);

		bool underxface = slp.x <= bscale.x;
		bool underyface = slp.y <= bscale.y;
		bool underzface = slp.z <= bscale.z;

		//0 - corner
		//1 - edge
		//2 - face
		//3 - inside
		char collcase = underxface + underyface + underzface;

		float distance = INFINITY;
		glm::vec3 localnormal = glm::vec3(0.0f);
		glm::vec3 localpos;
		switch (collcase)
		{
		case 0: //corner
		{
			glm::vec3 reltocorner = slp - bscale;
			distance = glm::length(reltocorner);
			localnormal = glm::normalize(reltocorner);
			localpos = bscale;
		}
			break;
		case 1: //edge
		{
			glm::vec3 reltoedge = (slp - bscale) * glm::vec3((!underxface), (!underyface), (!underzface));
			distance = glm::length(reltoedge);
			localnormal = glm::normalize(reltoedge);
			localpos = bscale;
			if (underxface)
				localpos.x = slp.x;
			if (underyface)
				localpos.y = slp.y;
			if (underzface)
				localpos.z = slp.z;
		}
			break;
		case 2: //face
		{
			glm::vec3 reltoface = slp - bscale;
			if (!underxface)
			{
				distance = reltoface.x;
				localnormal.x = 1.0f;
				localpos.x = bscale.x;
				localpos.y = slp.y;
				localpos.z = slp.z;
			}
			if (!underyface)
			{
				distance = reltoface.y;
				localnormal.y = 1.0f;
				localpos.x = slp.x;
				localpos.y = bscale.y;
				localpos.z = slp.z;
			}
			if (!underzface)
			{
				distance = reltoface.z;
				localnormal.z = 1.0f;
				localpos.x = slp.x;
				localpos.y = slp.y;
				localpos.z = bscale.z;
			}
		}
			break;
		case 3: //inside
		{
			glm::vec3 reltoface = slp - bscale;
			if (reltoface.x > reltoface.y && reltoface.x > reltoface.z)
			{
				distance = reltoface.x;
				localnormal.x = 1.0f;
			}
			else
			{
				if (reltoface.y > reltoface.z)
				{
					distance = reltoface.y;
					localnormal.y = 1.0f;
				}
				else
				{
					distance = reltoface.z;
					localnormal.z = 1.0f;
				}
			}
			localpos = slp;
		}
			break;
		default:
			break;
		}


		Collision colla, collb;

		colla.collidedWith = sc;
		colla.collider = bc;
		colla.depth = sc->radius - distance;
		colla.normal = bt.pointFromLocalToGlobal(-localnormal * sslp, true) - bt.getPosition();
		colla.pos = bt.pointFromLocalToGlobal((localpos+bro) * sslp, true);
		colla.relativePos = colla.pos - bt.getPosition();

		collb.collidedWith = bc;
		collb.collider = sc;
		collb.depth = colla.depth;
		collb.normal = -colla.normal;
		collb.relativePos = collb.normal * sc->radius;
		collb.pos = colla.pos;

		return { colla, collb };
	}
}	