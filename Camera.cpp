#include "Camera.h"

Camera::Camera(float xRotation, float yRotation, float zRotation, const Vertex& position)
{
	_viewingTransformation = Matrix::XRotationMatrix(xRotation) * 
							 Matrix::YRotationMatrix(yRotation) * 
							 Matrix::ZRotationMatrix(zRotation) * 
							 Matrix::TranslationMatrix(-position.GetX(), -position.GetY(), -position.GetZ());

	_position = Vertex(position.GetX(), position.GetY(), position.GetZ(), position.GetW());
}

const Matrix Camera::GetViewingTransformation()
{
	return _viewingTransformation;
}

const Vertex Camera::GetCameraPosition()
{
	return _position;
}
