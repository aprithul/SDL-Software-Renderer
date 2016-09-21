#pragma once
//#include <glm.hpp>
class Vector
{
public:
	Vector(void);
	Vector(float x, float y, float z);
	Vector(float x, float y);
	Vector(const Vector&);	// copy constructor
	~Vector(void);
	Vector operator+(const Vector& vector);
	Vector operator-(const Vector& vector);
	static float signed_triangle_area(Vector, Vector);

	float x;
	float y;
	float z;
};
