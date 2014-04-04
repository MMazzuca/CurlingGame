#include "Component.h"

using namespace CurlingEngine;

/****************************************************************\
* Destructor													*
\****************************************************************/
Component::~Component()
{
}

/****************************************************************\
* Update														*
* Traverses through and calls update on all components.		*
*																*
* Paramaters:													*
* dt: time since last update.									*
\****************************************************************/
void Component::AttachTo(Entity * ent)
{
	mptr_parent = ent;
}