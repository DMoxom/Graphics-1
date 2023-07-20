#pragma once
#include <cmath>

class Vector3D
{
public:
	Vector3D();
	Vector3D(float x, float y, float z);

	// Accessors
	float GetX() const;
	void SetX(const float x);
	float GetY() const;
	void SetY(const float y);
	float GetZ() const;
	void SetZ(const float z);

	const Vector3D operator+ (const Vector3D& rhs) const;

	// Vector maths
	static float DotProduct(Vector3D a, Vector3D b);
	static Vector3D CrossProduct(Vector3D a, Vector3D b);
	static Vector3D Normalise(Vector3D a);

private:
	float _x;
	float _y;
	float _z;
};

