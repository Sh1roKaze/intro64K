#include "types.h"

Vec3 normalize(Vec3 a)
{
	float length = (float)sqrt((double)(a.x*a.x + a.y*a.y + a.z*a.z));
	return Vec3(a.x / length, a.y / length, a.z / length);
}

Vec3 cross(Vec3 a, Vec3 b) 
{
	float x = a.y * b.z - a.z * b.y;
	float y = a.z * b.x - a.x * b.z;
	float z = a.x * b.y - a.y * b.x;
	return Vec3(x, y, z);
}

float dot(Vec3 a, Vec3 b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

float length(Vec3 a)
{
	return sqrtf(a.x * a.x + a.y * a.y + a.z * a.z);
}


/*

Vec3 vectorByMatrixMultiplication(const Vec3 v, const Mat4 m);

Vec3 vectorByMatrixMultiplication(const Vec3 v, const Mat4 m)
{
Vec3 res;

res.x = v.x * m.col[0].x + v.y * m.col[0].y + v.z * m.col[0].z;
res.y = v.x * m.col[1].x + v.y * m.col[1].y + v.z * m.col[1].z;
res.z = v.x * m.col[2].x + v.y * m.col[2].y + v.z * m.col[2].z;

return res;
}

*/