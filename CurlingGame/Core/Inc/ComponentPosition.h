#ifndef COMPONENTPOSITION_H
#define COMPONENTPOSITION_H

#include "Component.h"
#include "Vector3.h"

namespace CurlingEngine
{
	class ComponentPosition : public Component
	{
	private:
		Vector3<float> mvec_pos;

	public:
		/*****************************\
		* Constructors, Destructors *
		\*****************************/
		ComponentPosition() : mvec_pos(0, 0, 0) { }
		~ComponentPosition() { }
		
		/*******************\
		* Public Functions *
		\*******************/
		//Called each update by the parent Entity
		virtual void Update(float dt) { }

		/*******************\
		* Getters, Setters *
		\*******************/
		ComponentType type() const { return ComponentType::POSITION; }
		static ComponentType sType() { return ComponentType::POSITION; }

		float x() const { return mvec_pos.x(); }
		float y() const { return mvec_pos.y(); }
		float z() const { return mvec_pos.z(); }

		float x(float x) { return mvec_pos.x(x); }
		float y(float y) { return mvec_pos.y(y); }
		float z(float z) { return mvec_pos.z(z); }

		Vector3<float> & pos() { return mvec_pos; }
		Vector3<float> const& pos() const { return mvec_pos; }

		Vector3<float> & pos(Vector3<float> const& pos) { return mvec_pos = pos; }

	protected:
		void CheckDependencies() { }
	};
}

#endif