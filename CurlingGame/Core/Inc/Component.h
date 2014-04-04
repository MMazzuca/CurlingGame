#ifndef COMPONENT_H
#define COMPONENT_H

#include "Entity.h"


namespace CurlingEngine
{
	class Entity;

	class Component
	{
	private:
		Entity m_parent;

	public:
		Component(Entity parent) : m_parent(parent) { }
	};
}

#endif