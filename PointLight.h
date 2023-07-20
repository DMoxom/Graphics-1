#pragma once
#include "Vertex.h"

class PointLight
{
public:
	PointLight(int red, int green, int blue, int brightness, float a, float b, float c, Vertex position);

	int GetColour(int colour) const;

	int GetBrightness() const;

	Vertex GetLightPosition() const;

	float GetAttentuation(int attentuation) const;

private:
	int _rgb[3];
	int _brightness;
	float _attentuation[3];
	Vertex _position;
};

