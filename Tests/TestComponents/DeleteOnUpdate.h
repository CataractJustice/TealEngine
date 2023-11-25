#include "GameNode/Component.h"
using namespace TealEngine;
class DeleteOnUpdate : public Component 
{
	private:
	int ticks;
	public:
	DeleteOnUpdate(int ticks = 0);
	void update() override;
};

