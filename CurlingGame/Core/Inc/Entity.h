#ifndef ENTITY_H
#define ENTITY_H

#include <vector>
#include <ctime>

#include "Component.h"

namespace CurlingEngine
{
	class Component;

	class Entity
	{
		enum EntityType
		{
			BASE
		};

		typedef std::vector<Component *> cList;

	private:
		unsigned m_id;
		EntityType m_type;
		cList mlist_components;

	public:
		Entity() : m_type(EntityType::BASE), m_id(std::time(NULL)), mlist_components() { } 
		Entity(EntityType type) : m_type(type), m_id(std::time(NULL)), mlist_components() { } 

		void virtual Update(float dt);

		void AddComponent(Component * ptr_component);

		//Getters
		EntityType type() const { return m_type; }
		unsigned id() const { return m_id; }
	};
}

#endif