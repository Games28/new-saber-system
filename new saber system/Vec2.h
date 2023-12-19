#ifndef  VEC2_H
#define VEC2_H
#include "olcPixelGameEngine.h"
struct LineData {
	float x, y;
};
template <class T>
class Vec2
{
public:
	Vec2() : x(0.0f), y(0.0f) {}
	Vec2(T x, T y) : x(x), y(y) {}

	~Vec2() = default;

	void Add(const Vec2& v)
	{
		this->x += v.x;
		this->y += v.y;
	}

	void Sub(const Vec2& v)
	{
		this->x -= v.x;
		this->y -= v.y;
	}

	Vec2 min(const Vec2& v) const
	{
		return Vec2(std::min(x, v.x), std::min(y, v.y));
	}

	Vec2 max(const Vec2& v) const
	{
		return Vec2(std::max(x, v.x), std::max(y, v.y));
	}

	void Scale(const T n)
	{
		this->x *= n;
		this->y *= n;

	}

	Vec2 Rotate(const T angle) const
	{
		Vec2 result;
		result.x = x * cos(angle) - y * sin(angle);
		result.y = x * sin(angle) + y * cos(angle);

		return result;
	}

	T Magnitude() const
	{
		return sqrtf(x * x + y * y);
	}

	T MagnitudeSquared() const
	{
		return(x* x + y * y);
	}

	Vec2& Normalize()
	{
		float length = Magnitude();
		if (length != 0.0f)
		{
			x /= length;
			y /= length;
		}
		return *this;
	}

	Vec2 UnitVector() const
	{
		Vec2 result = Vec2(0, 0);
		float length = Magnitude();
		if (length != 0.0f)
		{
			result.x = x / length;
			result.y = y / length;
		}
		return result;
	}

	Vec2 Normal() const
	{
		return Vec2(y, -x).Normalize();
	}

	T Dot(const Vec2& v) const
	{
		return (x * v.x) + (y * v.y);
	}

	T Cross(const Vec2& v) const
	{
		return (x * v.y) - (y * v.x);
	}

	//overload operator
	Vec2& operator =(const Vec2& v)
	{
		x = v.x;
		y = v.y;
		return  *this;

	}

	bool operator ==(const Vec2& v) const
	{
		return x == v.x && y == v.y;
	}

	bool operator !=(const Vec2& v) const
	{
		return !(*this == v);
	}

	Vec2 operator +(const Vec2& v) const
	{
		Vec2 result;
		result.x = x + v.x;
		result.y = y + v.y;

		return result;
	}

	Vec2 operator -(const Vec2& v) const
	{
		Vec2 result;
		result.x = x - v.x;
		result.y = y - v.y;

		return result;
	}

	Vec2 operator *(const T n) const
	{
		Vec2 result;
		result.x = x * n;
		result.y = y * n;

		return result;
	}

	Vec2 operator /(const T n) const
	{
		Vec2 result;
		result.x = x / n;
		result.y = y / n;

		return result;
	}

	Vec2 operator -()
	{
		Vec2 result;
		result.x = x * -1;
		result.y = y * -1;
		return result;
	}

	Vec2& operator += (const Vec2& v)
	{
		this->x += v.x;
		this->y += v.y;
		return *this;
	}

	Vec2& operator -=(const Vec2& v)
	{
		this->x -= v.x;
		this->y -= v.y;
		return *this;
	}

	Vec2& operator *=(const T n)
	{
		this->x *= n;
		this->y *= n;
		return *this;
	}

	Vec2& operator /=(const T n)
	{
		this->x /= n;
		this->y /= n;
		return *this;
	}

	operator Vec2<int32_t>() const 
	{ return { static_cast<int32_t>(this->x), static_cast<int32_t>(this->y) }; }


	operator Vec2<float>() const
	{
		return { static_cast<float>(this->x), static_cast<float>(this->y) };
	}

	operator Vec2<double>() const
	{
		return { static_cast<double>(this->x), static_cast<double>(this->y) };
	}
public:
	T x, y;


};




typedef Vec2<int32_t> Vec2i;
typedef Vec2<uint32_t> Vec2u;
typedef Vec2<float> Vec2f;
typedef Vec2<double> Vec2d;

#endif // ! VEC2_H


