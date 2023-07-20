#include "Polygon3D.h"

Polygon3D::Polygon3D()
{
	_indices[0] = 0;
	_indices[1] = 0;
	_indices[2] = 0;
	_culled = false;
	_zDepth = 0.0f;
	_rgb[0] = 0;
	_rgb[1] = 0;
	_rgb[2] = 0;
}

Polygon3D::Polygon3D(int index0, int index1, int index2)
{
	_indices[0] = index0;
	_indices[1] = index1;
	_indices[2] = index2;
	_culled = false;
	_zDepth = 0.0f;
	_rgb[0] = 0;
	_rgb[1] = 0;
	_rgb[2] = 0;
}

Polygon3D::~Polygon3D()
{
}

Polygon3D::Polygon3D(const Polygon3D& p)
{
	Copy(p);
}

int Polygon3D::GetIndex(int index) const
{
	return _indices[index];
}

bool Polygon3D::GetCulled() const
{
	return _culled;
}

void Polygon3D::SetCulled(const bool culled)
{
	_culled = culled;
}

Vector3D Polygon3D::GetNormal() const
{
	return _normal;
}

void Polygon3D::SetNormal(const Vector3D normal)
{
	_normal = normal;
}

float Polygon3D::GetZDepth() const
{
	return _zDepth;
}

void Polygon3D::SetZDepth(const float zDepth)
{
	_zDepth = zDepth;
}

int Polygon3D::GetColour(int colour) const
{
	return _rgb[colour];
}

void Polygon3D::SetColour(int red, int green, int blue)
{
	_rgb[0] = red;
	_rgb[1] = green;
	_rgb[2] = blue;
}

Polygon3D& Polygon3D::operator=(const Polygon3D& rhs)
{
	if (this != &rhs)
	{
		Copy(rhs);
	}
	return *this;
}

void Polygon3D::Copy(const Polygon3D& other)
{
	for (int i = 0; i < MAX_INDICES; i++)
	{
		_indices[i] = other.GetIndex(i);
	}

	for (int i = 0; i < 3; i++)
	{
		_rgb[i] = other.GetColour(i);
	}

	_culled = other.GetCulled();
	_normal = other.GetNormal();
	_zDepth = other.GetZDepth();
}
