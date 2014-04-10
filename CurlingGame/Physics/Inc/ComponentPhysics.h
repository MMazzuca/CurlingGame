#ifndef COMPONENTPHYSICS_H
#define COMPONENTPHYSICS_H

#include "Component.h"
#include "ComponentPosition.h"
#include "Vector3.h"

namespace CurlingEngine
{

	class ComponentPhysics : public Component
	{
	private:
		float m_mass;
		Vector3<float> mvec_vel;
		Vector3<float> mvec_force;
		ComponentPosition * mcpt_position;

	public:
		/*****************************\
		* Constructors, Destructors *
		\*****************************/
		ComponentPhysics() : m_mass(0), mvec_vel(0, 0, 0), mvec_force(0, 0, 0), mcpt_position(NULL) { }
		~ComponentPhysics() { }
		
		
		/*******************\
		* Public Functions *
		\*******************/
		//Called each update by the parent Entity
		void Update(float dt);

		/*******************\
		* Getters, Setters *
		\*******************/
		ComponentType type() const { return ComponentType::PHYSICS; }
		static ComponentType sType() { return ComponentType::PHYSICS; }
		
		//mass
		float mass() { return m_mass; }
		float mass(float mass) { return m_mass = mass; } 

		//velocity
		Vector3<float> & vel() { return mvec_vel; }
		Vector3<float> const& vel() const { return mvec_vel; }
		Vector3<float> & vel(Vector3<float> const& vel) { return mvec_vel = vel; }

		//force
		Vector3<float> & force() { return mvec_force; }
		Vector3<float> const& force() const { return mvec_force; }
		Vector3<float> & force(Vector3<float> const& force) { return mvec_force = force; }

	protected:
		void checkDependencies();
		
	};
}

#endif