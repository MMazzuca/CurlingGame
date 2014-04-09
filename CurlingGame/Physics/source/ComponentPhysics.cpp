#include "ComponentPhysics.h"

using namespace CurlingEngine;


/****************************************************************\
* Update														*
* Updates position base on the velocity.						*
\****************************************************************/
void ComponentPhysics::Update(float dt)
{
	assert(NULL != mcpt_position);
	mcpt_position->pos() += mvec_vel * dt;

	if(m_mass > 0  && mvec_force.sqrMagnitude() > 0)
	{
		mvec_vel += mvec_force / m_mass;
	}
}

/****************************************************************\
* checkDependencies												*
* Checks the parent entity to make sure it has all the required *
* components.  If any are missing they are added.				*
\****************************************************************/
void ComponentPhysics::checkDependencies()
{
	mcpt_position = CheckDependency<ComponentPosition>();
}
