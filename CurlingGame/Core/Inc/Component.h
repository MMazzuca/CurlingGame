#ifndef COMPONENT_H
#define COMPONENT_H

#include "Entity.h"


namespace CurlingEngine
{
	class Entity;
	enum class EntityType;

	enum class ComponentType
	{
		BASE
	};

	class Component
	{
	private:
		Entity * m_parent;

	public:
		Component() : m_parent(NULL) { }

		virtual void Update(float dt) = 0;

		virtual void AttachTo(Entity * ent);

		//Getters
		virtual ComponentType type() const = 0;
	};
}

#endif