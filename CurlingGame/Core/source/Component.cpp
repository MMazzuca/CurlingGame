#include "Component.h"

using namespace CurlingEngine;

void Component::AttachTo(Entity * ent)
{
	m_parent = ent;
}