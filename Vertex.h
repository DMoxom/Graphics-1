#pragma once
#include "Vector3D.h"

class Vertex
{
public:
	Vertex();
	Vertex(float x, float y, float z, float w);
	Vertex(const Vertex& other);

	// Accessors
	float GetX() const;
	void SetX(const float x);
	float GetY() const;
	void SetY(const float y);
	float GetZ() const;
	void SetZ(const float z);
	float GetW() const;
	void SetW(const float w);

	int GetIntX() const;
	int GetIntY() const;

	Vector3D GetNormal() const;
	void SetNormal(const Vector3D normal);

	int GetColour(int colour) const;
	void SetColour(int red, int green, int blue);

	int GetContribution() const;
	void SetContribution(const int contribution);

	// Assignment operator
	Vertex& operator= (const Vertex& rhs);

	bool operator== (const Vertex& rhs) const;

	const Vertex operator+ (const Vertex& rhs) const;

	Vector3D Subtract(Vertex other);

	void Dehomogenize();

private:
	float _x;
	float _y;
	float _z;
	float _w;

	Vector3D _normal;
	int _rgb[3];
	int _contribution;

	void Copy(const Vertex& other);
};
