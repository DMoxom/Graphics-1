#include "PointLight.h"

PointLight::PointLight(int red, int green, int blue, int brightness, float a, float b, float c, Vertex position)
{
	_rgb[0] = red;
	_rgb[1] = green;
	_rgb[2] = blue;

	_brightness = brightness;

	_attentuation[0] = a;
	_attentuation[1] = b;
	_attentuation[2] = c;

	_position = Vertex(position.GetX(), position.GetY(), position.GetZ(), position.GetW());
}

int PointLight::GetColour(int colour) const
{
	return _rgb[colour];
}

int PointLight::GetBrightness() const
{
	return _brightness;
}

Vertex PointLight::GetLightPosition() const
{
	return _position;
}

float PointLight::GetAttentuation(int attentuation) const
{
	return _attentuation[attentuation];
}
