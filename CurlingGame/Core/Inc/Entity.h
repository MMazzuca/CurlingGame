#ifndef ENTITY_H
#define ENTITY_H

#include <vector>

#include "Component.h"

namespace CurlingEngine
{
	class Component;

	class Entity
	{
	private:
		std::vector<Component> mlist_components;

	public:
		Entity() : mlist_components() { } 
	};
}

#endif