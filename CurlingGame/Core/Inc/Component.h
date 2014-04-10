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
		BASE,
		POSITION,
		PHYSICS
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


		/*********************\
		* Abstract Fucntions *
		\*********************/
	public:
		virtual void Update(float dt) = 0; //Called each update by the parent Entity
		virtual ComponentType type() const = 0;  //Returns Type of the component
	protected:
		//This function should call CheckDependency<T>() for each component this one is dependent upon
		virtual void CheckDependencies() = 0;


	public:
		/*****************************\
		* Constructors, Destructors *
		\*****************************/
		Component() : mptr_parent(NULL) { }
		virtual ~Component();


		/*******************\
		* Public Functions *
		\*******************/
		//Attaches this Component to an Entity
		virtual void AttachTo(Entity * ent);


		/*******************\
		* Getters, Setters *
		\*******************/
		static ComponentType sType() { return ComponentType::BASE; }

		Entity * parent() { return mptr_parent; }
		Entity const* parent() const { return mptr_parent; }
		

	protected:
		//Helper function for CheckDependencies().  Checks if a component of class T exists on the parent entity.
		//If the component dose not exist, it is added.
		template<class T>
		T * CheckDependency()
		{
			T * compt;
			compt = static_cast<T *>( parent()->GetComponent<T>() );
			if(NULL == compt)
			{
				compt = static_cast<T *>(parent()->AddComponent<T>());
			}
			return compt;
		}
	};
}

#endif