#ifndef ENTITY_H
#define ENTITY_H

#include <vector>
#include <ctime>

#include <cassert>

#include "Component.h"
#include "Vector3.h"

namespace CurlingEngine
{
	//forward declarations
	class Component;	
	enum class ComponentType;
	
	//Enum to differentiate types of entities
	//Add new entity types to this list
	enum class EntityType
	{
		BASE
	};

	/****************************************************************\
	* Entity class													*
	* Abstract class for all entities used in the game.				*
	*																*
	\****************************************************************/
	class Entity
	{
	private:
		typedef std::vector<Component *> cList;

		unsigned m_id;			//unique id
		cList mlist_components;	//vector of components

	public:
		/*****************************\
		 * Constructors, Destructors *
		\*****************************/
		Entity() : m_id(static_cast<unsigned>(std::time(NULL))), mlist_components() { } 
		virtual ~Entity();


		/*******************\
		* Public Functions *
		\*******************/
		//Calls Update() on all Components
		virtual void Update(float dt);

		//Adds a new Component, if one of the same type doesn't already exist
		//Trying out this template version, should work as long as T inherits from Component and this way
		//This makes for clearier memory management since the Component is created and destroyed by the Entity class.
		template<class T>
		Component * AddComponent()
		{
			Component * ptr_component = new T();

			//If AddComponent does not return 0, then clean up memory
			if (AddComponent(ptr_component))
			{
				delete ptr_component;
				ptr_component = NULL;
			}

			return ptr_component;
		}


		/*******************\
		* Getters, Setters *
		\*******************/
		virtual EntityType type() const = 0;
		unsigned id() const { return m_id; }

		template<class T>
		Component * GetComponent()
		{
			Component * ptr_component;
			ptr_component = GetComponent(T::sType());
			return ptr_component;
		}
		template<class T>
		Component const* GetComponent() const
		{
			Component const* ptr_component;
			ptr_component = GetComponent(T::sType());
			return ptr_component;
		}

	private:

		/********************\
		* Private Functions *
		\********************/
		//Helper functions for GetComponent<T>()
		Component * GetComponent(ComponentType type);
		Component const* GetComponent(ComponentType type) const;

		//Helper function for AddComponent<T>()
		int AddComponent(Component *const ptr_component);

	};
}

#endif