#ifndef ENTITY_H
#define ENTITY_H

#include <vector>
#include <ctime>

#include "Component.h"
#include "Vector3.h"

namespace CurlingEngine
{
	class Component;	
	enum class ComponentType;
	
	enum class EntityType
	{
		BASE
	};

	class Entity
	{
	private:
		typedef std::vector<Component *> cList;

		unsigned m_id;
		cList mlist_components;

	public:
		Entity() : m_id(std::time(NULL)), mlist_components() { } 
		~Entity();

		virtual void Update(float dt);

		int AddComponent(Component *const ptr_component);

		//Getters
		virtual EntityType type() const = 0;
		unsigned id() const { return m_id; }

		Component * GetComponent(ComponentType type);

	};
}

#endif