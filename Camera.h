#pragma once
#include "Matrix.h"
#include <vector>

class Camera
{
public:
	Camera(float xRotation, float yRotation, float zRotation, const Vertex& position);

	const Matrix GetViewingTransformation();

	const Vertex GetCameraPosition();

private:
	Matrix _viewingTransformation;
	Vertex _position;
};
