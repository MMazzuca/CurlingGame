#include "Entity.h"

using namespace CurlingEngine;

/****************************************************************\
 * Destructor													*
\****************************************************************/
Entity::~Entity()
{
	for(cList::iterator it = mlist_components.begin(); it != mlist_components.end(); ++it)
	{
		delete *it;
	}
}


/****************************************************************\
 * Update														*
 *																*
 * Paramaters:													*
 * dt: time since last update.									*
\****************************************************************/
void Entity::Update(float dt)
{
	for(cList::iterator it = mlist_components.begin(); it != mlist_components.end(); ++it)
	{
		if(NULL != *it)
			(*it)->Update(dt);
	}
}


/****************************************************************\
 * AddComponent													*
 * Adds a component to the entity.  This Entity takes over		*
 * management of the component.									*
 *																*
 * Paramaters:													*
 * ptr_component: pointer to the entity to add.					*
 *																*
 * Returns:														*
 * 0 if successful.												*
 * 1 if ptr_component is NULL.									*
 * 2 if a component of this type is already attached.			*
 * 3 if this component is alraedy attached.						*
\****************************************************************/
int Entity::AddComponent(Component *const ptr_component)
{
	int rtnCode = 1;
	Component * ptr_existingComponent;

	if(NULL != ptr_component)
	{
		ptr_existingComponent = GetComponent(ptr_component->type());
		if(NULL == ptr_existingComponent)
		{
			rtnCode = 0;
			ptr_component->AttachTo(this);
			mlist_components.push_back(ptr_component);
		}
		else if(ptr_component == ptr_existingComponent)
			rtnCode = 3;
		else
			rtnCode = 2;
	}

	return rtnCode;
}


/****************************************************************\
 * GetComponent													*
 * Gets the first component of type type from the Entity.		*
 *																*
 * Paramaters:													*
 * type: type of component to look for.							*
\****************************************************************/
Component * Entity::GetComponent(ComponentType type)
{
	Component * ptr_cmpt = NULL;

	for(cList::iterator it = mlist_components.begin(); NULL == ptr_cmpt && it != mlist_components.end(); ++it)
	{
		if(type == (*it)->type())
			ptr_cmpt = *it;
	}

	return ptr_cmpt;
}