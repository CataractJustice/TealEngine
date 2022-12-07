#pragma once
#include "GameNode/Component.h"
namespace TealEngine 
{
	namespace Physics 
	{
		class PhysicsWorld;
		class Collider : public Component 
		{
		protected:
			//Mask of layers to which collider is contributiong
			unsigned long int colliderCanBeCollided;
			//Mask of layers with which collider can collide
			unsigned long int colliderCollidesWith;
			bool canBeSelfCollidedWith;
			bool canSelfCollide;
			PhysicsWorld* world;
		public:
			Collider(PhysicsWorld* world);
			virtual ~Collider();

			enum PhysicsLayers
			{
				Default
			};

			//set layers at which other colliders can detect collision with this collider
			void setCollidableAtLayer(unsigned char layer, bool value);
			//set layers at which this collider will check collisions with other colliders
			void setCollideWithLayer(unsigned char layer, bool value);

			//Set true if other colliders attached to the same node can collide with this collider
			inline void setCanBeSelfCollidedWith(bool value) 
			{
				canBeSelfCollidedWith = value;
			}
			//Set true if can collide with other colliders attached to the same node
			inline void setCanSelfCollide(bool value) 
			{
				canSelfCollide = value;
			}

			inline bool canCollideWith(Collider* collider) 
			{
				return (colliderCollidesWith & collider->colliderCanBeCollided) && ((getParrent() != collider->getParrent()) || (canSelfCollide && collider->canBeSelfCollidedWith));
			}
		};
	}
}