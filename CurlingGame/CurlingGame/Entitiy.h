#ifndef ENTITY_H
#define ENTITY_H

#include <vector>

#include "Component.h"

namespace Curling
{
class Entity
{
private:
	std::vector<Component> mlist_components;


};
}

#endif