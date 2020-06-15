#pragma once

#ifndef _GRAPHICSCLASS_H_
#define _GRAPHICSCLASS_H_

#include "d3dclass.h"
#include "cameraclass.h"
#include "modelclass.h"
#include "lightshaderclass.h"
#include "lightclass.h"
#include "textureshaderclass.h"
#include "bitmapclass.h"
#include "textclass.h"

#include "SkydomeClass.h"
#include "ShyDomeShaderClass.h"
#include "SkyPlaneClass.h"
#include "SkyPlaneShaderClass.h"

// Globals
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 20000.0f;
const float SCREEN_NEAR = 0.1f;

class GraphicsClass
{
public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass&);
	~GraphicsClass();

	bool Initialize(int, int, HWND);
	void Shutdown();
	bool Frame(int, int, float);

	CameraClass* GetCamera();		
	ModelClass* GetDrone();

private:
	bool Render();

private:
	D3DClass* m_D3D;
	TextureShaderClass* m_TextureShader;
	CameraClass* m_Camera;
	LightShaderClass* m_LightShader;
	LightClass* m_Light;
	TextClass* m_Text;

	ModelClass* drone;
	ModelClass* city;	
	ModelClass* bottom;

	SkyDomeClass* m_SkyDome;
	SkyDomeShaderClass* m_SkyDomeShader;
	SkyPlaneClass* m_SkyPlane;
	SkyPlaneShaderClass* m_SkyPlaneShader;

public:
	bool useLightingEffect[3];
	int allPolygonCount;
};
#endif