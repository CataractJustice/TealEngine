#pragma once
#include "GameNode/GameNode3D.h"
#include "Graphics/Mesh/Mesh.h"
#include "BulletPhysicsConverting.h"
#include <btBulletCollisionCommon.h>
namespace TealEngine 
{
	class Collider : public GameNode3D
	{
	protected:
		btCollisionShape* shape;
	public:

		virtual void setTransform(Transform transform) 
		{
			this->transform = transform;
			glm::vec3 scale = transform.getScale();
			shape->setLocalScaling(btVector3(scale.x, scale.y, scale.z));
		}

		btCollisionShape* getShape() 
		{
			return shape;
		}

		Collider() 
		{
		
		}

		Collider(btCollisionShape* shape)
		{
			this->shape = shape;
		}

		void update() 
		{
			
		}
	};




	class MeshCollider : public Collider
	{
	private:
		btTriangleMesh* btMesh;
		btBvhTriangleMeshShape* meshShape;
	public:
		MeshCollider(Mesh* mesh)
		{
			btMesh = new btTriangleMesh();
			vector<vec3> triangles = mesh->getTriangles();
			for (unsigned int i = 0; i < triangles.size(); i += 3)
			{
				btVector3 vert0 = BulletPhysicsConverting::vec3GLMtoBT(triangles[i]);
				btVector3 vert1 = BulletPhysicsConverting::vec3GLMtoBT(triangles[i + 1]);
				btVector3 vert2 = BulletPhysicsConverting::vec3GLMtoBT(triangles[i + 2]);
				btMesh->addTriangle(vert0, vert1, vert2, false);
			}
			shape = new btBvhTriangleMeshShape(btMesh, false);
		}

		void setMesh(Mesh* mesh) 
		{
			vector<vec3> triangles = mesh->getTriangles();
			for (unsigned int i = 0; i < triangles.size(); i += 3)
			{
				btVector3 vert0 = BulletPhysicsConverting::vec3GLMtoBT(triangles[i]);
				btVector3 vert1 = BulletPhysicsConverting::vec3GLMtoBT(triangles[i + 1]);
				btVector3 vert2 = BulletPhysicsConverting::vec3GLMtoBT(triangles[i + 2]);
				btMesh->addTriangle(vert0, vert1, vert2, false);
			}
		}
	};
}