#include "DeleteOnUpdate.h"

DeleteOnUpdate::DeleteOnUpdate(int ticks) : ticks(ticks)
{

}

void DeleteOnUpdate::update() 
{
	this->ticks--;
	if(ticks <= 0) this->getParent()->destroy();
};

