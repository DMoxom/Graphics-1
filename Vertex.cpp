#include "Vertex.h"

Vertex::Vertex()
{
	_x = 0.0f;
	_y = 0.0f;
	_z = 0.0f;
	_w = 1.0f;
	_rgb[0] = 0;
	_rgb[1] = 0;
	_rgb[2] = 0;
	_contribution = 0;
}

Vertex::Vertex(float x, float y, float z, float w)
{
	_x = x;
	_y = y;
	_z = z;
	_w = w;
	_rgb[0] = 0;
	_rgb[1] = 0;
	_rgb[2] = 0;
	_contribution = 0;
}

Vertex::Vertex(const Vertex& other)
{
	Copy(other);
}

float Vertex::GetX() const
{
	return _x;
}

void Vertex::SetX(const float x)
{
	_x = x;
}

float Vertex::GetY() const
{
	return _y;
}

void Vertex::SetY(const float y)
{
	_y = y;
}

float Vertex::GetZ() const
{
	return _z;
}

void Vertex::SetZ(const float z)
{
	_z = z;
}

float Vertex::GetW() const
{
	return _w;
}

void Vertex::SetW(const float w)
{
	_w = w;
}

int Vertex::GetIntX() const
{
	return (int)_x;
}

int Vertex::GetIntY() const
{
	return (int)_y;
}

Vector3D Vertex::GetNormal() const 
{
	return _normal;
}

void Vertex::SetNormal(const Vector3D normal) 
{
	_normal = normal;
}

int Vertex::GetColour(int colour) const
{
	return _rgb[colour];
}

void Vertex::SetColour(int red, int green, int blue) 
{
	_rgb[0] = red;
	_rgb[1] = green;
	_rgb[2] = blue;
}

int Vertex::GetContribution() const
{
	return _contribution;
}

void Vertex::SetContribution(const int contribution)
{
	_contribution = contribution;
}

Vertex& Vertex::operator=(const Vertex& rhs)
{
	// Only do the assignment if we are not assigning
	// to ourselves
	if (this != &rhs)
	{
		Copy(rhs);
	}
	return *this;
}

// The const at the end of the declaraion for '==" indicates that this operation does not change
// any of the member variables in this class.

bool Vertex::operator==(const Vertex& rhs) const
{
	return (_x == rhs.GetX() && _y == rhs.GetY() && _z == rhs.GetZ() && _w == rhs.GetW());
}

// You can see three different uses of 'const' here:
//
// The first const indicates that the method changes the return value, but it is not moved in memory
// The second const indicates that the parameter is passed by reference, but it is not modified
// The third const indicates that the operator does not change any of the memory variables in the class.

const Vertex Vertex::operator+(const Vertex& rhs) const
{
	return Vertex(_x + rhs.GetX(), _y + rhs.GetY(), _z + rhs.GetZ(), _w + rhs.GetW());
}

Vector3D Vertex::Subtract(Vertex other)
{
	return Vector3D(_x - other.GetX(), 
					_y - other.GetY(), 
					_z - other.GetZ());
}

void Vertex::Dehomogenize()
{
	_x /= _w;
	_y /= _w;
	_z /= _w;
	_w = 1.0f;
}

void Vertex::Copy(const Vertex& other)
{
	_x = other.GetX();
	_y = other.GetY();
	_z = other.GetZ();
	_w = other.GetW();

	_normal = other.GetNormal();
	
	for (int i = 0; i < 3; i++)
	{
		_rgb[i] = other.GetColour(i);
	}

	_contribution = other.GetContribution();
}
