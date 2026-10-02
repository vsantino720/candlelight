#pragma once

template<typename T>
struct s_vector
{
	T x;
	T y;

	void normalize()
	{
		T length = magnitude();
		if (length > 0)
		{
			x /= length;
			y /= length;
		}
	}

	inline T magnitude() const { return std::sqrt(x * x + y * y); }
	inline void clear() { x = 0; y = 0; }
	inline bool is_clear() const { return x == 0 && y == 0; }

	// Operators

	s_vector<T> operator*(T scalar)
	{
		return { x * scalar, y * scalar };
	}

	s_vector<T>& operator*=(T scalar)
	{
		x *= scalar;
		y *= scalar;
		return *this;
	}

	s_vector<T> operator+(const s_vector<T>& other)
	{
		return { x + other.x, y + other.y };
	}

	s_vector<T> operator-(const s_vector<T>& other)
	{
		return { x - other.x, y - other.y };
	}
};