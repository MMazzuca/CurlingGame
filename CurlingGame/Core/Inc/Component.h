#ifndef COMPONENT_H
#define COMPONENT_H

#include "Entity.h"


namespace CurlingEngine
{
	//forward declarations
	class Entity;
	enum class EntityType;

	//Enum to differentiate types of components
	//Add new component types to this list
	enum class ComponentType
	{
		BASE
	};

	/****************************************************************\
	* Component class												*
	* Abstract class for all components attached to Entities.		*
	*																*
	\****************************************************************/
	class Component
	{
	private:
		Entity * mptr_parent;	//Entity this Component is attached to

	public:
		/*****************************\
		* Constructors, Destructors *
		\*****************************/
		Component() : mptr_parent(NULL) { }
		virtual ~Component();


		/*******************\
		* Public Functions *
		\*******************/
		//Called each update by the parent Entity
		virtual void Update(float dt) = 0;

		//Attaches this Component to an Entity
		virtual void AttachTo(Entity * ent);


		/*******************\
		* Getters, Setters *
		\*******************/
		virtual ComponentType type() const = 0;
	};
}

#endif