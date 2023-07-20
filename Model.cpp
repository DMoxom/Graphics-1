#include "Model.h"

using namespace std;

Model::Model()
{
	for (int i = 0; i < 3; i++)
	{
		_ka[i] = 0.0f;
		_kd[i] = 0.0f;
		_ks[i] = 0.0f;
	}
}

Model::~Model()
{
}

const vector<Polygon3D>& Model::GetPolygons()
{
	return _polygons;
}

const vector<Vertex>& Model::GetVertices()
{
	return _vertices;
}

const vector<Vertex>& Model::GetUpdatedVertices()
{
	return _updatedVertices;
}

size_t Model::GetPolygonCount() const
{
	return _polygons.size();
}

size_t Model::GetVertexCount() const
{
	return _vertices.size();
}

void Model::AddVertex(float x, float y, float z)
{
	_vertices.push_back(Vertex(x, y, z, 1));
}

void Model::AddPolygon(int i0, int i1, int i2)
{
	_polygons.push_back(Polygon3D(i0, i1, i2));
}

void Model::AddUpdatedVertex(float x, float y, float z)
{
	_updatedVertices.push_back(Vertex(x, y, z, 1));
}

void Model::SetReflection(float ka[3], float kd[3], float ks[3])
{
	for (int i = 0; i < 3; i++)
	{
		_ka[i] = ka[i];
		_kd[i] = kd[i];
		_ks[i] = ks[i];
	}
}

void Model::ApplyTransformToLocalVertices(const Matrix& transform)
{
	_updatedVertices.clear();
	for (int i = 0; i < GetVertexCount(); i++)
	{
		_updatedVertices.push_back(transform * _vertices[i]);
	}
}

void Model::ApplyTransformToTransformedVertices(const Matrix& transform)
{
	for (int i = 0; i < GetVertexCount(); i++)
	{
		_updatedVertices[i] = transform * _updatedVertices[i];
	}
}

void Model::DehomogenizeVertices()
{
	for (int i = 0; i < GetVertexCount(); i++)
	{
		_updatedVertices[i].Dehomogenize();
	}
}

void Model::CalculateBackfaces(const Vertex& position)
{
	for (size_t i = 0; i < GetPolygonCount(); i++)
	{
		Vertex vertices[3];

		for (int n = 0; n < MAX_INDICES; n++)
		{
			vertices[n] = _updatedVertices[_polygons[i].GetIndex(n)];
		}

		Vector3D a = vertices[0].Subtract(vertices[1]);
		Vector3D b = vertices[0].Subtract(vertices[2]);

		Vector3D normal = Vector3D::CrossProduct(b, a);

		Vector3D eyeVector = vertices[0].Subtract(position);

		float dotProduct = Vector3D::DotProduct(normal, eyeVector);

		_polygons[i].SetNormal(Vector3D::Normalise(normal));

		if (dotProduct < 0)
		{
			_polygons[i].SetCulled(true);
		}
		else
		{
			_polygons[i].SetCulled(false);
		}
	}
}

bool compare(Polygon3D a, Polygon3D b)
{
	return a.GetZDepth() > b.GetZDepth();
}

void Model::Sort(void)
{
	for (size_t i = 0; i < GetPolygonCount(); i++)
	{
		Vertex vertices[3];

		for (int n = 0; n < MAX_INDICES; n++)
		{
			vertices[n] = _updatedVertices[_polygons[i].GetIndex(n)];
		}

		float zDepth = (vertices[0].GetZ() + vertices[1].GetZ() + vertices[2].GetZ()) / MAX_INDICES;

		_polygons[i].SetZDepth(zDepth);
	}

	sort(_polygons.begin(), _polygons.end(), compare);
}

float clamp(float n, float lower, float upper)
{
	return max(lower, min(n, upper));
}

void Model::CalculateLightingDirectional(vector<DirectionalLight> directionalLight)
{
	float totalRGB[3];
	float tempRGB[3];

	for (size_t i = 0; i < GetPolygonCount(); i++)
	{
		totalRGB[0] = (float)_polygons[i].GetColour(0);
		totalRGB[1] = (float)_polygons[i].GetColour(1);
		totalRGB[2] = (float)_polygons[i].GetColour(2);

		for (int n = 0; n < directionalLight.size(); n++)
		{
			Vector3D normalisedLight = { Vector3D::Normalise(directionalLight[n].GetLightDirection()) };

			float dotProduct = Vector3D::DotProduct(normalisedLight, _polygons[i].GetNormal());

			for (int c = 0; c < 3; c++)
			{
				tempRGB[c] = (float)directionalLight[n].GetColour(c);

				tempRGB[c] *= _kd[c];

				tempRGB[c] *= dotProduct;

				totalRGB[c] += tempRGB[c];
			}
		}

		totalRGB[0] = clamp(totalRGB[0], 0, 255);
		totalRGB[1] = clamp(totalRGB[1], 0, 255);
		totalRGB[2] = clamp(totalRGB[2], 0, 255);

		_polygons[i].SetColour((int)totalRGB[0], (int)totalRGB[1], (int)totalRGB[2]);
	}
}

void Model::CalculateLightingAmbient(AmbientLight ambientLight)
{
	float totalRGB[3];
	float tempRGB[3];

	for (size_t i = 0; i < GetPolygonCount(); i++)
	{
		totalRGB[0] = 0;
		totalRGB[1] = 0;
		totalRGB[2] = 0;

		for (int c = 0; c < 3; c++)
		{
			tempRGB[c] = (float)ambientLight.GetColour(c);

			tempRGB[c] *= _kd[c];

			totalRGB[c] += tempRGB[c];
		}

		totalRGB[0] = clamp(totalRGB[0], 0, 255);
		totalRGB[1] = clamp(totalRGB[1], 0, 255);
		totalRGB[2] = clamp(totalRGB[2], 0, 255);

		_polygons[i].SetColour((int)totalRGB[0], (int)totalRGB[1], (int)totalRGB[2]);
	}
}

void Model::CalculateLightingPoint(vector<PointLight> pointLight)
{
	float totalRGB[3];
	float tempRGB[3];

	for (size_t i = 0; i < GetPolygonCount(); i++)
	{
		totalRGB[0] = (float)_polygons[i].GetColour(0);
		totalRGB[1] = (float)_polygons[i].GetColour(1);
		totalRGB[2] = (float)_polygons[i].GetColour(2);

		for (int n = 0; n < pointLight.size(); n++)
		{
			Vertex polygonCorner = _updatedVertices[_polygons[i].GetIndex(0)];

			Vector3D subtract = polygonCorner.Subtract(pointLight[n].GetLightPosition());

			Vector3D lightSourceVector = { Vector3D::Normalise(subtract) };

			float dotProduct = Vector3D::DotProduct(lightSourceVector, _polygons[i].GetNormal());

			dotProduct = clamp(dotProduct, 0, 1);

			double distance = sqrt(pow(subtract.GetX(), 2) + pow(subtract.GetY(), 2) + pow(subtract.GetZ(), 2));

			double attenuation = (1 / (pointLight[n].GetAttentuation(0) + pointLight[n].GetAttentuation(1) * distance + pointLight[n].GetAttentuation(2) * pow(distance, 2)));

			for (int c = 0; c < 3; c++)
			{
				tempRGB[c] = (float)pointLight[n].GetColour(c);

				tempRGB[c] *= _kd[c];

				tempRGB[c] *= dotProduct;

				tempRGB[c] *= (float)attenuation * pointLight[n].GetBrightness();

				totalRGB[c] += tempRGB[c];
			}
		}

		totalRGB[0] = clamp(totalRGB[0], 0, 255);
		totalRGB[1] = clamp(totalRGB[1], 0, 255);
		totalRGB[2] = clamp(totalRGB[2], 0, 255);

		_polygons[i].SetColour((int)totalRGB[0], (int)totalRGB[1], (int)totalRGB[2]);
	}
}

void Model::CalculateVertexNormals()
{
	for (size_t v = 0; v < GetVertexCount(); v++)
	{
		_updatedVertices[v].SetNormal(Vector3D(0, 0, 0));
		_updatedVertices[v].SetContribution(0);
	}
	
	for (size_t i = 0; i < GetPolygonCount(); i++)
	{
		Vertex vertices[3];

		for (int n = 0; n < MAX_INDICES; n++)
		{
			vertices[n] = _updatedVertices[_polygons[i].GetIndex(n)];

			vertices[n].SetNormal(vertices[n].GetNormal() + _polygons[i].GetNormal());

			vertices[n].SetContribution(vertices[n].GetContribution() + 1);

			_updatedVertices[_polygons[i].GetIndex(n)] = vertices[n];
		}
	}

	for (size_t v = 0; v < GetVertexCount(); v++)
	{
		Vector3D normal = _updatedVertices[v].GetNormal();
		normal.SetX(normal.GetX() / _updatedVertices[v].GetContribution());
		normal.SetY(normal.GetY() / _updatedVertices[v].GetContribution());
		normal.SetZ(normal.GetZ() / _updatedVertices[v].GetContribution());

		_updatedVertices[v].SetNormal(Vector3D::Normalise(normal));
	}
}

void Model::CalculateVertexLightingAmbient(AmbientLight ambientLight)
{
	float totalRGB[3];
	float tempRGB[3];

	for (size_t i = 0; i < GetVertexCount(); i++)
	{
		totalRGB[0] = 0;
		totalRGB[1] = 0;
		totalRGB[2] = 0;

		for (int c = 0; c < 3; c++)
		{
			tempRGB[c] = (float)ambientLight.GetColour(c);

			tempRGB[c] *= _kd[c];

			totalRGB[c] += tempRGB[c];
		}

		totalRGB[0] = clamp(totalRGB[0], 0, 255);
		totalRGB[1] = clamp(totalRGB[1], 0, 255);
		totalRGB[2] = clamp(totalRGB[2], 0, 255);

		_updatedVertices[i].SetColour((int)totalRGB[0], (int)totalRGB[1], (int)totalRGB[2]);
	}
}

void Model::CalculateVertexLightingDirectional(vector<DirectionalLight> directionalLight)
{
	float totalRGB[3];
	float tempRGB[3];

	for (size_t i = 0; i < GetVertexCount(); i++)
	{
		totalRGB[0] = (float)_updatedVertices[i].GetColour(0);
		totalRGB[1] = (float)_updatedVertices[i].GetColour(1);
		totalRGB[2] = (float)_updatedVertices[i].GetColour(2);

		for (int n = 0; n < directionalLight.size(); n++)
		{
			Vector3D normalisedLight = { Vector3D::Normalise(directionalLight[n].GetLightDirection()) };

			float dotProduct = Vector3D::DotProduct(normalisedLight, _updatedVertices[i].GetNormal());

			for (int c = 0; c < 3; c++)
			{
				tempRGB[c] = (float)directionalLight[n].GetColour(c);

				tempRGB[c] *= _kd[c];

				tempRGB[c] *= dotProduct;

				totalRGB[c] += tempRGB[c];
			}
		}	

		totalRGB[0] = clamp(totalRGB[0], 0, 255);
		totalRGB[1] = clamp(totalRGB[1], 0, 255);
		totalRGB[2] = clamp(totalRGB[2], 0, 255);

		_updatedVertices[i].SetColour((int)totalRGB[0], (int)totalRGB[1], (int)totalRGB[2]);
	}
}

void Model::CalculateVertexLightingPoint(vector<PointLight> pointLight)
{
	float totalRGB[3];
	float tempRGB[3];

	for (size_t i = 0; i < GetVertexCount(); i++)
	{
		totalRGB[0] = (float)_updatedVertices[i].GetColour(0);
		totalRGB[1] = (float)_updatedVertices[i].GetColour(1);
		totalRGB[2] = (float)_updatedVertices[i].GetColour(2);

		for (int n = 0; n < pointLight.size(); n++)
		{
			Vector3D subtract = _updatedVertices[i].Subtract(pointLight[n].GetLightPosition());

			Vector3D lightSourceVector = { Vector3D::Normalise(subtract) };

			float dotProduct = Vector3D::DotProduct(lightSourceVector, _updatedVertices[i].GetNormal());

			dotProduct = clamp(dotProduct, 0, 1);

			double distance = sqrt(pow(subtract.GetX(), 2) + pow(subtract.GetY(), 2) + pow(subtract.GetZ(), 2));

			double attenuation = (1 / (pointLight[n].GetAttentuation(0) + pointLight[n].GetAttentuation(1) * distance + pointLight[n].GetAttentuation(2) * pow(distance, 2)));

			for (int c = 0; c < 3; c++)
			{
				tempRGB[c] = (float)pointLight[n].GetColour(c);

				tempRGB[c] *= _kd[c];

				tempRGB[c] *= dotProduct;

				tempRGB[c] *= (float)attenuation * pointLight[n].GetBrightness();

				totalRGB[c] += tempRGB[c];
			}

			totalRGB[0] = clamp(totalRGB[0], 0, 255);
			totalRGB[1] = clamp(totalRGB[1], 0, 255);
			totalRGB[2] = clamp(totalRGB[2], 0, 255);
		}

		_updatedVertices[i].SetColour((int)totalRGB[0], (int)totalRGB[1], (int)totalRGB[2]);
	}
}
