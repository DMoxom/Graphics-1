#pragma once
#include <vector>
#include <algorithm>
#include "Polygon3D.h"
#include "Vertex.h"
#include "Matrix.h"
#include "DirectionalLight.h"
#include "AmbientLight.h"
#include "PointLight.h"

using namespace std;

class Model
{
public:
	Model();
	~Model();

	// Accessors
	const vector<Polygon3D>& GetPolygons();
	const vector<Vertex>& GetVertices();
	const vector<Vertex>& GetUpdatedVertices();
	size_t GetPolygonCount() const;
	size_t GetVertexCount() const;
	void AddVertex(float x, float y, float z);
	void AddPolygon(int i0, int i1, int i2);
	void AddUpdatedVertex(float x, float y, float z);

	void SetReflection(float ka[3], float kd[3], float ks[3]);

	void ApplyTransformToLocalVertices(const Matrix& transform);
	void ApplyTransformToTransformedVertices(const Matrix& transform);

	void DehomogenizeVertices();

	void CalculateBackfaces(const Vertex& position);

	void Sort(void);

	// Lighting calculation methods for flat shading
	void CalculateLightingDirectional(vector<DirectionalLight> directionalLight);

	void CalculateLightingAmbient(AmbientLight ambientLight);

	void CalculateLightingPoint(vector<PointLight> pointLight);

	// Lighting calculation methods for gouraud shading
	void CalculateVertexNormals();

	void CalculateVertexLightingAmbient(AmbientLight ambientLight);

	void CalculateVertexLightingDirectional(vector<DirectionalLight> directionalLight);

	void CalculateVertexLightingPoint(vector<PointLight> pointLight);
private:
	vector<Polygon3D> _polygons;
	vector<Vertex> _vertices;
	vector<Vertex> _updatedVertices;

	float _ka[3];
	float _kd[3];
	float _ks[3];
};
