#include "Vector.h"

Vector::Vector(void)
{
}

Vector::~Vector(void)
{
}

float Vector::signed_triangle_area(Vector vector_1, Vector vector_2)
{
	return ((vector_1.x * vector_2.y) - (vector_1.y * vector_2.x)/2.f);
}

Vector::Vector(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

Vector::Vector(float x, float y)
{
	this->x = x;
	this->y = y;
	this->z = 0;
}

Vector::Vector(const Vector& vector)
{
	x = vector.x;
	y = vector.y;
	z = vector.z;
}

Vector Vector::operator+(const Vector& vector)
{
	return Vector(this->x + vector.x , this->y + vector.y, this->z + vector.z);
}

Vector Vector::operator-(const Vector& vector)
{
	return Vector(this->x - vector.x , this->y - vector.y, this->z - vector.z);
}
