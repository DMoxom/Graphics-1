#include "AmbientLight.h"

AmbientLight::AmbientLight(int red, int green, int blue)
{
	_rgb[0] = red;
	_rgb[1] = green;
	_rgb[2] = blue;
}

int AmbientLight::GetColour(int colour) const
{
	return _rgb[colour];
}
