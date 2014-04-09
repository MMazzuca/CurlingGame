#ifndef VECTOR3_H
#define VECTOR3_H

#include <cmath>

namespace CurlingEngine
{
	template<class T>
	class Vector3
	{
	private:
		T vals[3];

	public:
		Vector3() : vals[0](0), vals[1](0), vals[2](0) { }
		Vector3(T x, T y, T z) : vals[0](x), vals[1](y), vals[2](z) { }
		Vector3(Vector3<T> const& other) : vals[0](other.x()), vals[1](other.y()), vals[2](other.z()) { }

		T x() const { return vals[0]; }
		T y() const { return vals[1]; }
		T z() const { return vals[2]; }
		T x(T x) { return vals[0] = x; }
		T y(T y) { return vals[1] = y; }
		T z(T z) { return vals[2] = z; }

		float magnitude() const { return std::sqrt(sqrMagnitude()); }
		float sqrMagnitude() const { return vals[0] * vals[0] + vals[1] * vals[1] + vals[2] * vals[2]; }
		
		Vector3<T> & operator=(Vector3<T> const& other) { vals[0] = other.vals[0]; vals[1] = other.vals[1]; vals[2] = other.vals[2]; return *this; }
		Vector3<T> & operator+=(Vector3<T> const& other) { vals[0] += other.vals[0]; vals[1] += other.vals[1]; vals[2] += other.vals[2]; return *this; }
		Vector3<T> & operator-=(Vector3<T> const& other) { vals[0] -= other.vals[0]; vals[1] -= other.vals[1]; vals[2] -= other.vals[2]; return *this; }

		static T Dot(Vector3 const& vec1, Vector3 const& vec2) { return vec1.vals[0] * vec2.vals[0] + vec1.vals[1] * vec2.vals[1] + vec1.vals[2] * vec2.vals[2]; }
	};

	
	template<class T>
	bool operator==(Vector3<T> const& vec1, Vector3<T> const& vec2)
	{
		return vec1.x() == vec2.x() && vec1.y() == vec2.y() && vec1.z() == vec2.z();
	}
	template<class T>
	bool operator!=(Vector3<T> const& vec1, Vector3<T> const& vec2)
	{
		return !(vec1 == vec2);
	}

	template<class T>
	Vector3<T> operator+(Vector3<T> const& vec1, Vector3<T> const& vec2)
	{
		return Vector3<T>(vec1.x() + vec2.x(), vec1.y() + vec2.y(), vec1.z() + vec2.z());
	}
	template<class T>
	Vector3<T> operator-(Vector3<T> const& vec1, Vector3<T> const& vec2)
	{
		return Vector3<T>(vec1.x() - vec2.x(), vec1.y() - vec2.y(), vec1.z() - vec2.z());
	}

	template<class T, class F>
	Vector3<T> operator*(Vector3<T> const& vec, F const& scaler)
	{
		return Vector3<T>(vec.x() * scaler, vec.y() * scaler, vec.z() * scaler);
	}
	template<class T, class F>
	Vector3<T> operator*(F const& scaler, Vector3<T> const& vec)
	{
		return vec * scaler;
	}
	template<class T, class F>
	Vector3<T> operator/(Vector3<T> const& vec, F const& scaler)
	{
		return Vector3<T>(vec.x() / scaler, vec.y() / scaler, vec.z());
	}

}

#endif