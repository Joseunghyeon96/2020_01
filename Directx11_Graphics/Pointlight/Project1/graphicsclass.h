#ifndef _GRAPHICSCLASS_H_
#define _GRAPHICSCLASS_H_
#include "d3dclass.h"
#include "cameraclass.h"

#include "modelclass.h"
#include "LightShaderClass.h"
#include "LightClass.h"

#include "BitmapClass.h"
#include "TextureShaderClass.h"
#include "TextClass.h"


using namespace std;

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
	bool Frame(int, float);
	void SetFps(int);
	
	void CameraRotControl(D3DXVECTOR2);
	void CameraPosControl(int);
	void ChangeBackGroundColor(float red, float green, float blue, float alpha);
	LightShaderClass* GetLightShader();
	LightClass* GetLight();
	D3DClass* GetD3D();

	
private:
	bool Render(float);
private:
	D3DClass* m_D3D= nullptr;

	int fps;
	float zRotation;
	float backGround_RGBA[4];
	CameraClass* m_Camera;
	ModelClass* gunModel;
	ModelClass* skullModel;
	ModelClass* catModel;
	ModelClass* planeModel;
	LightShaderClass* m_LightShader;
	LightClass* m_directLight;
	LightClass* m_Light1;
	LightClass* m_Light2;
	LightClass* m_Light3;
	LightClass* m_Light4;

	TextureShaderClass* m_TextureShader;
	BitmapClass* m_Bitmap;
	TextClass* m_Text;
};
#endif