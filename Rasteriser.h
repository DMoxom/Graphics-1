#pragma once
#include "Framework.h"
#include "Model.h"
#include "MD2Loader.h"
#include "Camera.h"
#include <cmath>
#include <chrono>

class Rasteriser : public Framework
{
public:
    bool Initialise();
    void Update(const Bitmap& bitmap);
    void Render(const Bitmap& bitmap);
    void Shutdown();

    void GeneratePerspectiveMatrix(float d, float aspectRatio);
    void GenerateViewMatrix(float d, int width, int height);

    void DrawString(const Bitmap& bitmap, LPCTSTR text);

    // Wireframe drawing
    void DrawWireFrame(const Bitmap& bitmap);

    // Drawing with built in Polygon function
    void DrawSolidFlat(const Bitmap& bitmap);

    // Drawing with my polygon method
    void MyDrawSolidFlat(const Bitmap& bitmap);

    void DrawPixelLine(const Bitmap& bitmap, float x1, float y1, float x2, float y2, int rgb[3]);

    Vertex* SortVerticesAscendingByY(Vertex v1, Vertex v2, Vertex v3);

    void FillTopFlatTriangle(const Bitmap& bitmap, Vertex v1, Vertex v2, Vertex v3, int rgb[3]);

    void FillBottomFlatTriangle(const Bitmap& bitmap, Vertex v1, Vertex v2, Vertex v3, int rgb[3]);

    void FillPolygonFlat(const Bitmap& bitmap, Vertex v1, Vertex v2, Vertex v3, int rgb[3]);

    // Drawing with my polygon method using gouraud
    void DrawGouraud(const Bitmap& bitmap);

    void FillTopFlatTriangleGouraud(const Bitmap& bitmap, Vertex v1, Vertex v2, Vertex v3, int colourV1[3], int colourV2[3], int colourV3[3]);

    void FillBottomFlatTriangleGouraud(const Bitmap& bitmap, Vertex v1, Vertex v2, Vertex v3, int colourV1[3], int colourV2[3], int colourV3[3]);

    void FillPolygonGouraud(const Bitmap& bitmap, Vertex v1, Vertex v2, Vertex v3);
private:
    Model _model;
    Camera _camera = Camera(0, 0, 0, Vertex(0, 0, 0, 0));
    Matrix _perspective;
    Matrix _view;
    Matrix _transformation;

    int _time = 0;

    float _angle = 0;
    float _scaling = 0;
    float _translation = 0;

    float _ka[3] = { 1.0f, 1.0f, 1.0f };
    float _kd[3] = { 1.0f, 1.0f, 1.0f };
    float _ks[3] = { 1.0f, 1.0f, 1.0f };

    vector<DirectionalLight> _directionalLight;

    vector<PointLight> _pointLights;

    AmbientLight _ambientLight = AmbientLight(0, 0, 0);;
};
