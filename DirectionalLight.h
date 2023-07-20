#pragma once
#include "Vector3D.h"

class DirectionalLight
{
public:
	DirectionalLight(int red, int green, int blue, Vector3D position);

	int GetColour(int colour) const;

	Vector3D GetLightDirection() const;

private:
	int _rgb[3];
	Vector3D _direction;
};

