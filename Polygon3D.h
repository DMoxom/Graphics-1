#pragma once
#include "Vector3D.h"

int const MAX_INDICES = 3;

class Polygon3D
{
public:
	Polygon3D();
	Polygon3D(int index0, int index1, int index2);
	~Polygon3D();
	Polygon3D(const Polygon3D& p);

	// Accessor to return index of specified vertex
	int GetIndex(int index) const;

	bool GetCulled() const;
	void SetCulled(const bool culled);

	Vector3D GetNormal() const;
	void SetNormal(const Vector3D normal);

	float GetZDepth() const;
	void SetZDepth(const float zDepth);

	int GetColour(int colour) const;
	void SetColour(int red, int green, int blue);

	Polygon3D& operator= (const Polygon3D& rhs);
private:
	int _indices[3];
	bool _culled;
	Vector3D _normal;
	float _zDepth;
	int _rgb[3];

	void Copy(const Polygon3D& other);
};
