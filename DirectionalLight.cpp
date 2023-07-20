#include "DirectionalLight.h"

DirectionalLight::DirectionalLight(int red, int green, int blue, Vector3D direction)
{
	_rgb[0] = red;
	_rgb[1] = green;
	_rgb[2] = blue;

	_direction = Vector3D(direction.GetX(), direction.GetY(), direction.GetZ());
}

int DirectionalLight::GetColour(int colour) const
{
	return _rgb[colour];
}

Vector3D DirectionalLight::GetLightDirection() const
{
	return _direction;
}
