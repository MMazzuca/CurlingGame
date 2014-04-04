#include "Entity.h"

using namespace CurlingEngine;


void Entity::Update(float dt)
{
	for(cList::iterator it = mlist_components.begin(); it != mlist_components.end(); ++it)
	{
		if(NULL != *it)
			(*it)->Update(dt);
	}
}

void Entity::AddComponent(Component * ptr_component)
{
	ptr_component->AttachTo(this);
	mlist_components.push_back(ptr_component);
}