#pragma once
#include "GameNode/GameNode3D.h"
namespace TealEngine
{
	template<class T>
	class AAQTree : public GameNode3D 
	{
	private:
		vector<T> items;
		vector<AABB> itemsAABB;
		vector<AAQTree<T>> subTrees;
		unsigned int maxItems;
		unsigned int maxDepth;
		unsigned short id;
		bool subdivided;
		void subdivide() 
		{
			for (int i = 0; i < 8; i++)
			{
				subTrees.push_back(AAQTree(this->aabb.position + this->aabb.scale * vec3(float(i % 2), float((i % 4) > 1), float(i > 3)) / 2.0f, this->aabb.scale / 2.0f, this->maxDepth, this->maxItems));
				subTrees[i].depth = this->depth + 1;
				for (int j = 0; j < this->items.size(); j++)
				{
					subTrees[i].addItem(this->items[j], this->itemsAABB[j]);
				}
			}
			this->items.clear();
			this->itemsAABB.clear();
			this->subdivided = true;
		}

		void join() 
		{
			for (int i = 0; i < 4; i++)
			{
				this->items = subTrees[i].getItems();
				this->itemsAABB = subTrees[i].getItemsAABB();
			}
			this->subTrees.clear();
			this->subdivided = false;
		}
	protected:
		unsigned int depth;
		vector<AABB> getItemsAABB()
		{
			return this->itemsAABB;
		}
	public:
		AAQTree(vec3 pos, vec3 scale, unsigned int maxDepth = 8, int maxItems = 4)
		{
			this->maxDepth = maxDepth;
			this->depth = 0;
			this->aabb = AABB(pos, scale);
			this->subdivided = false;
			this->maxItems = maxItems;
		}

		bool addItem(T item, AABB aabb) 
		{
			if (getAABB(SINGLE_NODE_AABB).checkIntersection(aabb)) 
			{
				if (isBottomLayer())
				{
					this->items.push_back(item);
					this->itemsAABB.push_back(aabb);

					if (this->items.size() > this->maxItems && this->depth < this->maxDepth && this->subTrees.size() == 0)
					{
						this->subdivide();
					}
				}
				else 
				{
					for (int i = 0; i < this->subTrees.size(); i++)
						subTrees[i].addItem(item, aabb);
				}
				return true;
			}
			else 
			{
				return false;
			}
		}

		vector<T> getItems() 
		{
			return this->items;
		}

		T getItem(unsigned int index) 
		{
			return items[index];
		}

		unsigned int getDepth() 
		{
			return depth;
		}

		bool isBottomLayer() 
		{
			return !this->subdivided;
		}

		vector<AAQTree<T>> getTree()
		{
			vector<AAQTree<T>> sub;
			for (int i = 0; i < this->subTrees.size(); i++)
			{
				if (this->subTrees[i].isBottomLayer()) sub.push_back(this->subTrees[i]);
				vector<AAQTree<T>> subTree = this->subTrees[i].getTree();
				sub.insert(sub.end(), subTree.begin(), subTree.end());
			}
			return sub;
		}
	};
}