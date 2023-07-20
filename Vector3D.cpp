#include "Vector3D.h"

Vector3D::Vector3D()
{
	_x = 0.0f;
	_y = 0.0f;
	_z = 0.0f;
}

Vector3D::Vector3D(float x, float y, float z)
{
	_x = x;
	_y = y;
	_z = z;
}

float Vector3D::GetX() const
{
	return _x;
}

void Vector3D::SetX(const float x)
{
	_x = x;
}

float Vector3D::GetY() const
{
	return _y;
}

void Vector3D::SetY(const float y)
{
	_y = y;
}

float Vector3D::GetZ() const
{
	return _z;
}

void Vector3D::SetZ(const float z)
{
	_z = z;
}

const Vector3D Vector3D::operator+(const Vector3D& rhs) const
{
	return Vector3D(_x + rhs.GetX(), _y + rhs.GetY(), _z + rhs.GetZ());
}

float Vector3D::DotProduct(Vector3D a, Vector3D b)
{
	return (a.GetX() * b.GetX()) + (a.GetY() * b.GetY()) + (a.GetZ() * b.GetZ());
}

Vector3D Vector3D::CrossProduct(Vector3D a, Vector3D b)
{
	return Vector3D((a.GetY() * b.GetZ()) - (a.GetZ() * b.GetY()), 
					(a.GetZ() * b.GetX()) - (a.GetX() * b.GetZ()), 
					(a.GetX() * b.GetY()) - (a.GetY() * b.GetX()));
}

Vector3D Vector3D::Normalise(Vector3D a)
{
	//double magnitude = sqrt(pow(a.GetX(), 2) + pow(a.GetY(), 2) + pow(a.GetZ(), 2));

	double magnitude = sqrt((a.GetX() * a.GetX()) + (a.GetY() * a.GetY()) +(a.GetZ() * a.GetZ()));
	
	return Vector3D(a.GetX()/(float)magnitude,
					a.GetY()/(float)magnitude,
					a.GetZ()/(float)magnitude);
}
