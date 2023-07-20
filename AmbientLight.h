#pragma once


class AmbientLight
{
public:
	AmbientLight(int red, int green, int blue);

	int GetColour(int colour) const;
private:
	int _rgb[3];
};

