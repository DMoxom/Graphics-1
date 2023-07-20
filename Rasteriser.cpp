#include "Rasteriser.h"

using namespace std::chrono;

Rasteriser app;

const char* modelName = "models\\marvin.md2";

bool Rasteriser::Initialise()
{
	if (!MD2Loader::LoadModel(modelName, _model, &Model::AddPolygon, &Model::AddVertex))
	{
		return false;
	}

	// Different camera angles that work with models
	//_camera = Camera(0, 0, 0, Vertex(0, 2, -15, 1)); //ball
	//_camera = Camera(0, 0, 0, Vertex(0, 2, -50, 1)); //megaman
	//_camera = Camera(0, 0, 0, Vertex(0, 0, -60, 1)); //cube
	_camera = Camera(0, 0, 0, Vertex(0, 2, -55, 1)); //marvin

	_model.SetReflection(_ka, _kd, _ks);

	// Grey colours
	//_ambientLight = AmbientLight(100, 100, 100);
	//_directionalLight.push_back(DirectionalLight(255, 255, 255, Vector3D(-1, 0, 0)));

	// Main colours
	_ambientLight = AmbientLight(212, 34, 34);
	_directionalLight.push_back(DirectionalLight(245, 130, 42, Vector3D(-1, 0, 0)));
	_pointLights.push_back(PointLight(255, 214, 64, 250, 0, 0.2f, 0.5f, Vertex(12, 25, -20, 1)));

	return true;
}

// Perform any updates to the structures that will be used
// to render the window (i.e. transformation matrices, etc).

void Rasteriser::Update(const Bitmap& bitmap)
{
	// Time loop
	if (_time <= 775)
	{
		_time += 1;
	}
	else
	{
		_time = 0;
		_scaling = 0;
		_translation = 0;
	}
	
	//Angle spinning loop
	_angle += 1;
	if (_angle > 360)
	{
		_angle = 0;
	}

	// Different model transformations
	if (_time <= 100)
	{
		_transformation = _transformation.XRotationMatrix(_angle);
	}
	else if (_time <= 200)
	{
		_transformation = _transformation.YRotationMatrix(_angle);
	}
	else if (_time <= 300)
	{
		_transformation = _transformation.ZRotationMatrix(_angle);
	}
	else if (_time <= 400)
	{
		_transformation = _transformation.XYZRotationMatrix(_angle, _angle, _angle);
	}
	else if (_time <= 500)
	{
		_scaling += 0.01f;
		_transformation = _transformation.ScalingMatrix(_scaling, _scaling, _scaling);
	}
	else if (_time <= 600)
	{
		_translation += 0.5f;
		_transformation = _transformation.TranslationMatrix(_translation, _translation, _translation);
	}
	else
	{
		_transformation = _transformation.XYZRotationMatrix(0, _angle, 0);
	}

	GeneratePerspectiveMatrix(1, (float)(bitmap.GetWidth() / (float)bitmap.GetHeight()));

	GenerateViewMatrix(1, bitmap.GetWidth(), bitmap.GetHeight());
}

// Render the window. This should be overridden

void Rasteriser::Render(const Bitmap& bitmap)
{
	// Sets the background to the default window colour
	//bitmap.Clear(reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1));
	bitmap.Clear(static_cast<COLORREF>(0x00000000));

	LPCTSTR text = L"";

	_model.ApplyTransformToLocalVertices(_transformation);

	_model.CalculateBackfaces(_camera.GetCameraPosition());

	// Loop for lighting on DrawSolidFlat, MyDrawSolidFlat and DrawGouraud
	if (_time >= 600 && _time <= 725)
	{
		_model.CalculateLightingAmbient(_ambientLight);
		_model.CalculateLightingDirectional(_directionalLight);
		_model.CalculateLightingPoint(_pointLights);
	}
	else if (_time <= 775)
	{
		_model.CalculateVertexNormals();
		_model.CalculateVertexLightingAmbient(_ambientLight);
		_model.CalculateVertexLightingDirectional(_directionalLight);
		_model.CalculateVertexLightingPoint(_pointLights);
	}

	_model.ApplyTransformToTransformedVertices(_camera.GetViewingTransformation());

	_model.Sort();

	_model.ApplyTransformToTransformedVertices(_perspective);

	_model.DehomogenizeVertices();

	_model.ApplyTransformToTransformedVertices(_view);
	
	// Text with different render methods
	if (_time <= 600)
	{
		DrawWireFrame(bitmap);
		if (_time <= 100)
		{
			text = L"Wireframe (X Rotation)";
		}
		else if (_time <= 200)
		{
			text = L"Wireframe (Y Rotation)";
		}
		else if (_time <= 300)
		{
			text = L"Wireframe (Z Rotation)";
		}
		else if (_time <= 400)
		{
			text = L"Wireframe (XYZ Rotation)";
		}
		else if (_time <= 500)
		{
			text = L"Wireframe (Scaling)";
		}
		else if (_time <= 600)
		{
			text = L"Wireframe (Translation)";
		}
	}
	else if (_time <= 700)
	{
		DrawSolidFlat(bitmap);
		text = L"Solid Shading";
	} 
	else if (_time <= 725)
	{
		MyDrawSolidFlat(bitmap);
		text = L"My Solid Shading";
	}
	else if (_time <= 775)
	{
		DrawGouraud(bitmap);
		text = L"Gouraud Shading";
	}

	DrawString(bitmap, text);
}

// Perform any application shutdown that is needed

void Rasteriser::Shutdown()
{
}

void Rasteriser::GeneratePerspectiveMatrix(float d, float aspectRatio)
{
	_perspective = { d/aspectRatio, 0, 0, 0,
					 0, d, 0, 0,
					 0, 0, d, 0,
					 0, 0, 1, 0 };
}

void Rasteriser::GenerateViewMatrix(float d, int width, int height)
{
	_view = { (float)width / 2, 0, 0, (float)width / 2,
			  0, -(float)height / 2, 0, (float)height / 2,
			  0, 0, d / 2, d / 2,
			  0, 0, 0, 1 };
}

void Rasteriser::DrawString(const Bitmap& bitmap, LPCTSTR text)
{
	HDC hdc = bitmap.GetDC();
	HFONT hFont, hOldFont;

	// Retrieve a handle to the variable stock font.  
	hFont = hFont = CreateFont(48, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
		CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, TEXT("Myfont"));

	// Select the variable stock font into the specified device context. 
	if (hOldFont = (HFONT)SelectObject(hdc, hFont))
	{
		SetTextColor(hdc, RGB(255, 255, 255));
		SetBkColor(hdc, RGB(0, 0, 0));

		// Display the text string.  
		TextOut(hdc, 10, 10, text, lstrlen(text));

		// Restore the original font.        
		SelectObject(hdc, hOldFont);
	}
	DeleteObject(hFont);
}

void Rasteriser::DrawWireFrame(const Bitmap& bitmap)
{
	// Local variables for easier readability of code
	size_t polygonCount = _model.GetPolygonCount();

	vector<Vertex> updatedVertices;
	updatedVertices = _model.GetUpdatedVertices();

	vector<Polygon3D> polygon;
	polygon = _model.GetPolygons();

	HPEN polygonLine = CreatePen(PS_SOLID, 2, RGB(255, 140, 255));

	SelectObject(bitmap.GetDC(), polygonLine);

	// Loops through polygons that aren't culled
	for (size_t i = 0; i < polygonCount; i++)
	{
		if (polygon[i].GetCulled() == false)
		{
			int start = polygon[i].GetIndex(2);

			MoveToEx(bitmap.GetDC(), (int)updatedVertices[start].GetX(), (int)updatedVertices[start].GetY(), NULL);

			for (int n = 0; n < MAX_INDICES; n++)
			{
				int index = polygon[i].GetIndex(n);

				LineTo(bitmap.GetDC(), (int)updatedVertices[index].GetX(), (int)updatedVertices[index].GetY());
			}
		}
	}

	DeleteObject(polygonLine);
}

void Rasteriser::DrawSolidFlat(const Bitmap& bitmap)
{
	// Local variables for easier readability of code
	size_t polygonCount = _model.GetPolygonCount();

	vector<Vertex> updatedVertices;
	updatedVertices = _model.GetUpdatedVertices();

	vector<Polygon3D> polygon;
	polygon = _model.GetPolygons();

	// Loops through polygons that aren't culled
	for (size_t i = 0; i < polygonCount; i++)
	{
		if (polygon[i].GetCulled() == false)
		{
			Vertex vertices[3];
			POINT coords[3];

			HBRUSH polygonFillL = CreateSolidBrush(RGB(polygon[i].GetColour(0), polygon[i].GetColour(1), polygon[i].GetColour(2)));
			HPEN polygonBorderL = CreatePen(PS_SOLID, 1, RGB(polygon[i].GetColour(0), polygon[i].GetColour(1), polygon[i].GetColour(2)));

			SelectObject(bitmap.GetDC(), polygonFillL);
			SelectObject(bitmap.GetDC(), polygonBorderL);

			for (int n = 0; n < MAX_INDICES; n++)
			{
				vertices[n] = updatedVertices[polygon[i].GetIndex(n)];
				
				coords[n].x = (long)vertices[n].GetX();
				coords[n].y = (long)vertices[n].GetY();
			}

			Polygon(bitmap.GetDC(), coords, 3);

			DeleteObject(polygonFillL);
			DeleteObject(polygonBorderL);
		}
	}
}

void Rasteriser::MyDrawSolidFlat(const Bitmap& bitmap)
{
	// Local variables for easier readability of code
	size_t polygonCount = _model.GetPolygonCount();

	vector<Vertex> updatedVertices;
	updatedVertices = _model.GetUpdatedVertices();

	vector<Polygon3D> polygon;
	polygon = _model.GetPolygons();

	// Loops through polygons that aren't culled
	for (size_t i = 0; i < polygonCount; i++)
	{
		if (polygon[i].GetCulled() == false)
		{
			Vertex vertices[3];

			int rgb[3] = { polygon[i].GetColour(0), polygon[i].GetColour(1), polygon[i].GetColour(2) };

			for (int n = 0; n < MAX_INDICES; n++)
			{
				vertices[n] = updatedVertices[polygon[i].GetIndex(n)];
			}

			FillPolygonFlat(bitmap, vertices[0], vertices[1], vertices[2], rgb);
		}
	}
}

void Rasteriser::DrawPixelLine(const Bitmap& bitmap, float x1, float y1, float x2, float y2, int rgb[3])
{
	// Method to make lines out of pixels using SetPixel

	float x = x2 - x1;
	float y = y2 - y1;
	double length = sqrt((double)x * (double)x + (double)y * (double)y);

	double addx = x / length;
	double addy = y / length;

	x = x1;
	y = y1;

	if (length > 0)
	{
		for (double i = 0; i < length; i += 1)
		{
			SetPixel(bitmap.GetDC(), (int)x, (int)y, RGB(rgb[0], rgb[1], rgb[2]));
			x += (float)addx;
			y += (float)addy;
		}
	}
}

Vertex* Rasteriser::SortVerticesAscendingByY(Vertex v1, Vertex v2, Vertex v3)
{
	//Method to sort vertices in ascending order by Y

	Vertex vTmp;

	if (v1.GetY() > v2.GetY())
	{
		vTmp = v1;
		v1 = v2;
		v2 = vTmp;
	}

	if (v1.GetY() > v3.GetY())
	{
		vTmp = v1;
		v1 = v3;
		v3 = vTmp;
	}

	if (v2.GetY() > v3.GetY())
	{
		vTmp = v2;
		v2 = v3;
		v3 = vTmp;
	}
	
	static Vertex vertices[3];

	vertices[0] = v1;
	vertices[1] = v2;
	vertices[2] = v3;
	
	return vertices;
}

void Rasteriser::FillTopFlatTriangle(const Bitmap& bitmap, Vertex v1, Vertex v2, Vertex v3, int rgb[3])
{
	// Method to fill a triangle with a flat top

	float invslope1 = (float)(v3.GetIntX() - v1.GetIntX()) / (float)(v3.GetIntY() - v1.GetIntY());
	float invslope2 = (float)(v3.GetIntX() - v2.GetIntX()) / (float)(v3.GetIntY() - v2.GetIntY());

	float curx1 = (float)v3.GetIntX();
	float curx2 = (float)v3.GetIntX();

	for (int scanlineY = v3.GetIntY(); scanlineY > v1.GetIntY(); scanlineY--)
	{
		curx1 -= invslope1;
		curx2 -= invslope2;
		DrawPixelLine(bitmap, curx1, (float)scanlineY, curx2, (float)scanlineY, rgb);
	}
}

void Rasteriser::FillBottomFlatTriangle(const Bitmap& bitmap, Vertex v1, Vertex v2, Vertex v3, int rgb[3])
{
	// Method to fill a triangle with a flat bottom

	float invslope1 = (float)(v2.GetIntX() - v1.GetIntX()) / (float)(v2.GetIntY() - v1.GetIntY());
	float invslope2 = (float)(v3.GetIntX() - v1.GetIntX()) / (float)(v3.GetIntY() - v1.GetIntY());

	float curx1 = (float)v1.GetIntX();
	float curx2 = (float)v1.GetIntX();

	for (int scanlineY = v1.GetIntY(); scanlineY <= v2.GetIntY(); scanlineY++)
	{
		DrawPixelLine(bitmap, curx1, (float)scanlineY, curx2, (float)scanlineY, rgb);
		curx1 += invslope1;
		curx2 += invslope2;
	}
}

void Rasteriser::FillPolygonFlat(const Bitmap& bitmap, Vertex v1, Vertex v2, Vertex v3, int rgb[3])
{	
	Vertex* sort;
	
	sort = SortVerticesAscendingByY(v1, v2, v3);

	v1 = sort[0];
	v2 = sort[1];
	v3 = sort[2];

	if (v2.GetY() == v3.GetY())
	{
		FillBottomFlatTriangle(bitmap, v1, v2, v3, rgb);
	}
	else if (v1.GetY() == v2.GetY())
	{
		FillTopFlatTriangle(bitmap, v1, v2, v3, rgb);
	}
	else
	{
		Vertex v4;

		v4.SetX(v1.GetX() + ((v2.GetY() - v1.GetY()) / (v3.GetY() - v1.GetY())) * (v3.GetX() - v1.GetX()));
		v4.SetY(v2.GetY());
		v4.SetZ(v2.GetZ());
		v4.SetZ(v2.GetZ());
		
		FillBottomFlatTriangle(bitmap, v1, v2, v4, rgb);
		FillTopFlatTriangle(bitmap, v2, v4, v3, rgb);
	}
}

void Rasteriser::DrawGouraud(const Bitmap& bitmap)
{
	// Loops through polygons that aren't culled
	size_t polygonCount = _model.GetPolygonCount();

	vector<Vertex> updatedVertices;
	updatedVertices = _model.GetUpdatedVertices();

	vector<Polygon3D> polygon;
	polygon = _model.GetPolygons();

	// Local variables for easier readability of code
	for (size_t i = 0; i < polygonCount; i++)
	{
		if (polygon[i].GetCulled() == false)
		{
			Vertex vertices[3];

			for (int n = 0; n < MAX_INDICES; n++)
			{
				vertices[n] = updatedVertices[polygon[i].GetIndex(n)];
			}

			FillPolygonGouraud(bitmap, vertices[0], vertices[1], vertices[2]);
		}
	}
}

void Rasteriser::FillTopFlatTriangleGouraud(const Bitmap& bitmap, Vertex v1, Vertex v2, Vertex v3, int colourV1[3], int colourV2[3], int colourV3[3])
{
	float slope1 = (float)(v3.GetIntX() - v1.GetIntX()) / (float)(v3.GetIntY() - v1.GetIntY());
	float slope2 = (float)(v3.GetIntX() - v2.GetIntX()) / (float)(v3.GetIntY() - v2.GetIntY());

	float x1 = (float)v3.GetIntX();
	float x2 = (float)v3.GetIntX() + 2;

	// Get the change of color components along edge (v3,v1)
	float v3v1Diff = (float)(v3.GetIntY() - v1.GetIntY());
	float colourSlopeRed1 = (float)(colourV3[0] - colourV1[0]) / v3v1Diff;
	float colourSlopeGreen1 = (float)(colourV3[1] - colourV1[1]) / v3v1Diff;
	float colourSlopeBlue1 = (float)(colourV3[2] - colourV1[2]) / v3v1Diff;
	// Get the change of color components along edge (v3,v2)
	float v3v2Diff = (float)(v3.GetIntY() - v2.GetIntY());
	float colourSlopeRed2 = (float)(colourV3[0] - colourV2[0]) / v3v2Diff;
	float colourSlopeGreen2 = (float)(colourV3[1] - colourV2[1]) / v3v2Diff;
	float colourSlopeBlue2 = (float)(colourV3[2] - colourV2[2]) / v3v2Diff;
	// Set starting values
	float cRed1 = (float)colourV3[0];
	float cGreen1 = (float)colourV3[1];
	float cBlue1 = (float)colourV3[2];
	float cRed2 = (float)colourV3[0];
	float cGreen2 = (float)colourV3[1];
	float cBlue2 = (float)colourV3[2];

	if (slope1 < slope2)
	{
		float slopeTmp = slope1;
		slope1 = slope2;
		slope2 = slopeTmp;

		slopeTmp = colourSlopeRed1;
		colourSlopeRed1 = colourSlopeRed2;
		colourSlopeRed2 = slopeTmp;

		slopeTmp = colourSlopeGreen1;
		colourSlopeGreen1 = colourSlopeGreen2;
		colourSlopeGreen2 = slopeTmp;

		slopeTmp = colourSlopeBlue1;
		colourSlopeBlue1 = colourSlopeBlue2;
		colourSlopeBlue2 = slopeTmp;
	}

	for (int scanlineY = v3.GetIntY(); scanlineY > v1.GetIntY(); scanlineY--)
	{
		// Get new x-coordinate of endpoints of horizontal line
		for (int xPos = (int)ceil(x1); xPos < (int)x2; xPos++)
		{
			float t = (xPos - x1) / (x2 - x1);

			int red = (int)((1 - t) * cRed1 + t * cRed2);
			int green = (int)((1 - t) * cGreen1 + t * cGreen2);
			int blue = (int)((1 - t) * cBlue1 + t * cBlue2);

			SetPixel(bitmap.GetDC(), xPos, scanlineY, RGB(red, green, blue));
		}
		// Get new x-coordinate of endpoints of horizontal line
		x1 -= slope1;
		x2 -= slope2;
		// Get new color of left endpoint of horizontal line
		cRed1 -= colourSlopeRed1;
		cGreen1 -= colourSlopeGreen1;
		cBlue1 -= colourSlopeBlue1;
		// Get new color of right endpoint of horizontal line
		cRed2 -= colourSlopeRed2;
		cGreen2 -= colourSlopeGreen2;
		cBlue2 -= colourSlopeBlue2;
	}
}

void Rasteriser::FillBottomFlatTriangleGouraud(const Bitmap& bitmap, Vertex v1, Vertex v2, Vertex v3, int colourV1[3], int colourV2[3], int colourV3[3])
{
	float slope1 = (float)(v2.GetIntX() - v1.GetIntX()) / (float)(v2.GetIntY() - v1.GetIntY());
	float slope2 = (float)(v3.GetIntX() - v1.GetIntX()) / (float)(v3.GetIntY() - v1.GetIntY());

	float x1 = (float)v1.GetIntX();
	float x2 = (float)v1.GetIntX() + 2;

	// Get the change of color components along edge (v2,v1) 
	float v2v1Diff = (float)(v2.GetIntY() - v1.GetIntY());
	float colourSlopeRed1 = (float)(colourV2[0] - colourV1[0]) / v2v1Diff;
	float colourSlopeGreen1 = (float)(colourV2[1] - colourV1[1]) / v2v1Diff;
	float colourSlopeBlue1 = (float)(colourV2[2] - colourV1[2]) / v2v1Diff;
	// Get the change of color components along edge (v3,v1) 
	float v3v1Diff = (float)(v3.GetIntY() - v1.GetIntY());
	float colourSlopeRed2 = (float)(colourV3[0] - colourV1[0]) / v3v1Diff;
	float colourSlopeGreen2 = (float)(colourV3[1] - colourV1[1]) / v3v1Diff;
	float colourSlopeBlue2 = (float)(colourV3[2] - colourV1[2]) / v3v1Diff;
	// Get starting values 
	float cRed1 = (float)colourV1[0];
	float cGreen1 = (float)colourV1[1];
	float cBlue1 = (float)colourV1[2];
	float cRed2 = (float)colourV1[0];
	float cGreen2 = (float)colourV1[1];
	float cBlue2 = (float)colourV1[2];

	if (slope2 < slope1)
	{
		float slopeTmp = slope1;
		slope1 = slope2;
		slope2 = slopeTmp;

		slopeTmp = colourSlopeRed1;
		colourSlopeRed1 = colourSlopeRed2;
		colourSlopeRed2 = slopeTmp;

		slopeTmp = colourSlopeGreen1;
		colourSlopeGreen1 = colourSlopeGreen2;
		colourSlopeGreen2 = slopeTmp;

		slopeTmp = colourSlopeBlue1;
		colourSlopeBlue1 = colourSlopeBlue2;
		colourSlopeBlue2 = slopeTmp;
	}

	for (int scanlineY = v1.GetIntY(); scanlineY <= v2.GetIntY(); scanlineY++)
	{
		// Loop over each pixel of horizontal line
		for (int xPos = (int)ceil(x1); xPos < (int)x2; xPos++)
		{
			float t = (xPos - x1) / (x2 - x1);

			int red = (int)((1 - t) * cRed1 + t * cRed2);
			int green = (int)((1 - t) * cGreen1 + t * cGreen2);
			int blue = (int)((1 - t) * cBlue1 + t * cBlue2);

			SetPixel(bitmap.GetDC(), xPos, scanlineY, RGB(red, green, blue));
		}
		// Get new x-coordinate of endpoints of horizontal line 
		x1 += slope1;
		x2 += slope2;
		// Get new color of left endpoint of horizontal line 
		cRed1 += colourSlopeRed1;
		cGreen1 += colourSlopeGreen1;
		cBlue1 += colourSlopeBlue1;
		// Get new color of right endpoint of horizontal line 
		cRed2 += colourSlopeRed2;
		cGreen2 += colourSlopeGreen2;
		cBlue2 += colourSlopeBlue2;
	}
}

void Rasteriser::FillPolygonGouraud(const Bitmap& bitmap, Vertex v1, Vertex v2, Vertex v3)
{
	int colourV1[3];
	int colourV2[3];
	int colourV3[3];
	
	Vertex* sort;

	sort = SortVerticesAscendingByY(v1, v2, v3);

	v1 = sort[0];
	v2 = sort[1];
	v3 = sort[2];

	for (int c = 0; c < 3; c++)
	{
		colourV1[c] = v1.GetColour(c);
		colourV2[c] = v2.GetColour(c);
		colourV3[c] = v3.GetColour(c);
	}

	if (v2.GetY() == v3.GetY())
	{
		FillBottomFlatTriangleGouraud(bitmap, v1, v2, v3, colourV1, colourV2, colourV3);
	}
	else if (v1.GetY() == v2.GetY())
	{
		FillTopFlatTriangleGouraud(bitmap, v1, v2, v3, colourV1, colourV2, colourV3);
	}
	else
	{
		Vertex v4;

		v4.SetX(v1.GetX() + ((v2.GetY() - v1.GetY()) / (v3.GetY() - v1.GetY())) * (v3.GetX() - v1.GetX()));
		v4.SetY(v2.GetY());
		v4.SetZ(v2.GetZ());
		v4.SetZ(v2.GetW());

		float cRed = colourV1[0] + ((float)(v2.GetIntY() - v1.GetIntY()) / (float)(v3.GetIntY() - v1.GetIntY())) * (colourV3[0] - colourV1[0]);
		float cGreen = colourV1[1] + ((float)(v2.GetIntY() - v1.GetIntY()) / (float)(v3.GetIntY() - v1.GetIntY())) * (colourV3[1] - colourV1[1]);
		float cBlue = colourV1[2] + ((float)(v2.GetIntY() - v1.GetIntY()) / (float)(v3.GetIntY() - v1.GetIntY())) * (colourV3[2] - colourV1[2]);
		int cTmp[3] = { (int)cRed, (int)cGreen, (int)cBlue };

		FillBottomFlatTriangleGouraud(bitmap, v1, v2, v4, colourV1, colourV2, cTmp);
		FillTopFlatTriangleGouraud(bitmap, v2, v4, v3, colourV2, cTmp, colourV3);
	}
}
