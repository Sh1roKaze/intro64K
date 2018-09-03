#pragma once

#include <math.h>

#ifndef M_PI
	#define M_PI 3.14159265358979323846
#endif // !M_PI


struct Vec2 
{
	float x;
	float y;

	Vec2() 
	{
		x = 0.f;
		y = 0.f;
	};
	Vec2(float a, float b)
	{
		x = a;
		y = b;
	};
	inline Vec2 operator+(const Vec2 a) const
	{
		return Vec2(x + a.x, y + a.y);
	};
	inline Vec2 operator-(const Vec2 a) const
	{
		return Vec2(x - a.x, y - a.y);
	};
	inline Vec2 operator*(const Vec2 a) const
	{
		return Vec2(x * a.x, y * a.y);
	};
	inline Vec2 operator*(const float a) const 
	{
		return Vec2(x * a, y * a);
	};
};

struct Vec3
{
	float x;
	float y;
	float z;

	Vec3() 
	{
		x = 0.f;
		y = 0.f;
		z = 0.f;
	};
	Vec3(float a)
	{
		x = a;
		y = a;
		z = a;
	};

	Vec3(float a, float b, float c) 
	{
		x = a;
		y = b;
		z = c;
	};

	Vec3(Vec2 a, float c) 
	{
		x = a.x;
		y = a.y;
		z = c;
	};

	inline Vec3 operator+(const Vec3 a) const
	{
		return Vec3(x + a.x, y + a.y, z + a.z);
	};
	inline Vec3 operator-(const Vec3 a) const
	{
		return Vec3(x - a.x, y - a.y, z - a.z);
	};

	inline Vec3 operator*(const Vec3 a) const
	{
		return Vec3(x * a.x, y * a.y, z * a.z);
	};
	inline Vec3 operator*(const float a) const 
	{
		return Vec3(x * a, y * a, z * a);
	};
	inline Vec3 operator/(const Vec3 a) const
	{
		return Vec3(x / a.x, y / a.y, z / a.z);
	};
	inline Vec3 operator/(const float a) const
	{
		return Vec3(x / a, y / a, z / a);
	};
	inline bool operator!=(const Vec3 &right) const
	{
		if (x != right.x)
			return true;
		if (y != right.y)
			return true;
		if (z != right.z)
			return true;
		return false;
	};

};

struct Vec3ui 
{
	unsigned int x;
	unsigned int y;
	unsigned int z;

	Vec3ui()
	{
		x = 0;
		y = 0;
		z = 0;
	};

	Vec3ui(unsigned int a, unsigned int b, unsigned int c)
	{
		x = a;
		y = b;
		z = c;
	}

	inline Vec3ui operator+(const unsigned int a) const
	{
		return Vec3ui(x + a, y + a, z + a);
	}
};

struct Vec4 
{
	float x;
	float y;
	float z;
	float w;

	Vec4()
	{
		x = 0.f;
		y = 0.f;
		z = 0.f;
		w = 0.f;
	};
	Vec4(float a, float b, float c, float d)
	{
		x = a;
		y = b;
		z = c;
		w = d;
	};
	Vec4(Vec3 a, float d)
	{
		x = a.x;
		y = a.y;
		z = a.z;
		w = d;
	};
	inline Vec4 operator+(const Vec4 a) const
	{
		return Vec4(x + a.x, y + a.y, z + a.z, w + a.w);
	};
	inline Vec4 operator-(const Vec4 a) const 
	{
		return Vec4(x - a.x, y - a.y, z - a.z, w - a.w);
	};
	inline Vec4 operator*(const Vec4 a) const 
	{
		return Vec4(x * a.x, y * a.y, z * a.z, w * a.w);
	};
	inline Vec4 operator*(const float a) const 
	{
		return Vec4(x * a, y * a, z * a, w * a);
	};
};

struct Mat4
{
	struct Vec4 col[4];

	Mat4() {};
	Mat4(Vec4 a, Vec4 b, Vec4 c, Vec4 d)
	{
		col[0] = a;
		col[1] = b;
		col[2] = c;
		col[3] = d;
	};

	inline Vec3 operator*(const Vec3 v) const
	{
		Vec3 res;

		res.x = v.x * col[0].x + v.y * col[0].y + v.z * col[0].z;
		res.y = v.x * col[1].x + v.y * col[1].y + v.z * col[1].z;
		res.z = v.x * col[2].x + v.y * col[2].y + v.z * col[2].z;

		return res;
	}
};

Vec3 normalize(Vec3 a);
Vec3 cross(Vec3 a, Vec3 b);
float dot(Vec3 a, Vec3 b);
float length(Vec3 a);
