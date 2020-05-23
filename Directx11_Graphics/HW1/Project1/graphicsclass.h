#ifndef _GRAPHICSCLASS_H_
#define _GRAPHICSCLASS_H_
#include "d3dclass.h"
#include "cameraclass.h"
#include "modelclass.h"
#include "colorshaderclass.h"


// Globals
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;


class GraphicsClass
{
public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass&);
	~GraphicsClass();


	bool Initialize(int, int, HWND);
	void Shutdown();
	bool Frame();

	void ChangeBackGroundColor(float red, float green, float blue, float alpha);
	D3DXVECTOR3* GetCameraPosition();
	D3DClass* GetD3D();
	ColorShaderClass* GetColorShader();
	
private:
	bool Render();
private:
	D3DClass* m_D3D= nullptr;

	float zRotation;
	D3DXVECTOR3* cameraPos;
	float backGround_RGBA[4];
	CameraClass* m_Camera;
	ModelClass* m_TriangleModel;
	ModelClass* m_RectangleModel;
	ModelClass* m_PentagonModel;
	ColorShaderClass* m_ColorShader;
};
#endif