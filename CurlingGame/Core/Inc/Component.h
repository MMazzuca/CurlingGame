#ifndef COMPONENT_H
#define COMPONENT_H

#include "Entity.h"


namespace CurlingEngine
{
	class Entity;

	class Component
	{
	private:
		Entity * m_parent;

	public:
		Component() : m_parent(NULL) { }

		void virtual Update(float dt) = 0;

		void virtual AttachTo(Entity * ent);
	};
}

#endif