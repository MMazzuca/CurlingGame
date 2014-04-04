#ifndef VECTOR2_H
#define VECTOR2_H

#include <cmath>

namespace Curling
{
	template<class T>
	class Vector2
	{
	private:
		T vals[2];

	public:
		Vector2() : vals{0, 0} { }
		Vector2(T x, T y) : vals{x, y} { }
		Vector2(Vector2<T> const& other) : vals{other.vals[0], other.vals[1]} { }

		T x() const { return _x; }
		T y() const { return _y; }
		T x(T x) { return _x = x; }
		T y(T y) { return _y = y; }

		float magnitude() const { return std::sqrt(sqrMagnitude()); }
		float sqrMagnitude() const { return vals[0] * vals[0] + vals[1] * vals[1]; }
		
		Vector2<T> & operator=(Vector2<T> const& other) { vals[0] = other.vals[0]; vals[1] = other.vals[1]; return *this; }
		Vector2<T> & operator+=(Vector2<T> const& other) { vals[0] += other.vals[0]; vals[1] += other.vals[1]; return *this; }
		Vector2<T> & operator-=(Vector2<T> const& other) { vals[0] -= other.vals[0]; vals[1] -= other.vals[1]; return *this; }

		static T Dot(Vector2 const& vec1, Vector2 const& vec2) { return vec1.vals[0] * vec2.vals[0] + vec1.vals[1] * vec2.vals[1]; }
	};

	
	template<class T>
	bool operator==(Vector2<T> const& vec1, Vector2<T> const& vec2)
	{
		return vec1.x() == vec2.x() && vec1.y() == vec2.y();
	}
	template<class T>
	bool operator!=(Vector2<T> const& vec1, Vector2<T> const& vec2)
	{
		return !(vec1 == vec2);
	}

	template<class T>
	Vector2<T> operator+(Vector2<T> const& vec1, Vector2<T> const& vec2)
	{
		return Vector2<T>(vec1.x() + vec2.x(), vec1.y() + vec2.y());
	}
	template<class T>
	Vector2<T> operator-(Vector2<T> const& vec1, Vector2<T> const& vec2)
	{
		return Vector2<T>(vec1.x() + vec2.x(), vec1.y() + vec2.y());
	}

	template<class T, class F>
	Vector2<T> operator*(Vector2<T> const& vec, F const& scaler)
	{
		return Vector2<T>(vec.x() * scaler, vec.y() * scaler);
	}
	template<class T, class F>
	Vector2<T> operator*(F const& scaler, Vector2<T> const& vec)
	{
		return vec * scaler;
	}
	template<class T, class F>
	Vector2<T> operator/(Vector2<T> const& vec, F const& scaler)
	{
		return Vector2<T>(vec.x() / scaler, vec.y() / scaler);
	}




	template<class T>
	class Rect
	{
	private:
		Vector2<T> m_pos;
		Vector2<T> m_size;

	public:
		Rect(): m_pos(), m_size() { }
		Rect(Vector2<T> pos, Vector2<T> size) : m_pos(pos), m_size(size) { }
		Rect(T x1, T y1, T x2, T y2): m_pos(x1, y1), m_size(x2, y2) { }
		Rect(Rect<T> const& other) : m_pos(other.m_pos), m_size(other.m_size) { }

		Vector2<T> const& pos() const { return m_pos; } 
		Vector2<T> & pos() { return m_pos; }
		Vector2<T> & pos(Vector2<T> pos) { return m_pos = pos; }
		Vector2<T> const& size() const { return m_size; }
		Vector2<T> & size() { return m_size; }
		Vector2<T> & size(Vector2<T> size) { return m_size = size; }

		T x() const { return m_pos.x(); }
		T x(T x) { return m_pos.x(x); }
		T y() const { return m_pos.y(); }
		T y(T y) { return m_pos.y(y); }

		T w() const { return m_size.x(); }
		T w(T w) { return m_size.x(w); }
		T h() const { return m_size.y(); }
		T h(T h) { return m_size.y(h); }


		bool IsIn(Vector2<T> const& point) const
		{
			bool isIn = false;
			if(m_size.x() > 0)
				isIn = point.x() > m_pos.x() && point.x() < m_pos.x() + m_size.x();
			else
				isIn = point.x() < m_pos.x() && point.x() > m_pos.x() + m_size.x();

			if(m_size.y() > 0)
				isIn = isIn && point.y() > m_pos.y() && point.y() < m_pos.y() + m_size.y();
			else
				isIn = isIn && point.y() < m_pos.y() && point.y() > m_pos.y() + m_size.y();

			return isIn;
		}

		bool IsIn(Rect<T> const& rect) const
		{
			bool isIn = false;

			if (this->x() + this->w() < rect.x())
				return false;

			if (this->x() > rect.x() + rect.w())
				return false;

			if (this->y() - this->h() > rect.y())
				return false;

			if (this->y() < rect.y() - rect.h())
				return false;

			return true;
		}

	};

}

#endif