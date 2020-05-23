#ifndef _GRAPHICSCLASS_H_
#define _GRAPHICSCLASS_H_
#include "d3dclass.h"
#include "cameraclass.h"
#include "modelclass.h"
#include "textureshaderclass.h"


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
	TextureShaderClass* GetTextureShader();
	D3DClass* GetD3D();

	
private:
	bool Render(float);
private:
	D3DClass* m_D3D= nullptr;

	float zRotation;
	D3DXVECTOR3* cameraPos;
	float backGround_RGBA[4];
	CameraClass* m_Camera;
	ModelClass* gunModel;
	ModelClass* skullModel;
	ModelClass* catModel;
	ModelClass* planeModel;
	TextureShaderClass* m_TextureShader;
};
#endif