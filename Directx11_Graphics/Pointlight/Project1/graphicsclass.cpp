#include "graphicsclass.h"

GraphicsClass::GraphicsClass()
{
	m_D3D = 0;
	m_Camera = 0;
	gunModel = 0;
	skullModel = 0;
	catModel = 0;
	planeModel = 0;

	zRotation = 0.0f;

	m_LightShader = 0;
	m_directLight = 0;
	m_Light1 = 0;
	m_Light2 = 0;
	m_Light3 = 0;
	m_Light4 = 0;

	fps = 0;
	m_TextureShader = 0;
	m_Bitmap = 0;
	m_Text = 0;

}
GraphicsClass::GraphicsClass(const GraphicsClass& other)
{
}
GraphicsClass::~GraphicsClass()
{
}
bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;
	D3DXMATRIX baseViewMatrix;

	// Create the Direct3D object.
	m_D3D = new D3DClass;
	if (!m_D3D)
	{
		return false;
	}

	// Initialize the Direct3D object.
	result = m_D3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN,
		SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D", L"Error", MB_OK);
		return false;
	}
	// Create the camera object.
	m_Camera = new CameraClass;
	if (!m_Camera)
	{
		return false;
	}

	// Set the initial position of the camera.
	m_Camera->SetPosition(0.0f,2.0f,-12.0f);
	


	////Create plane model =========================================================================
	planeModel = new ModelClass;
	if (!planeModel)
	{
		return false;
	}
	// Initialize the model object.
	result = planeModel->Initialize(m_D3D->GetDevice()
		, const_cast<char*>("../Project1/data/plane01.txt"),
		const_cast<WCHAR*>(L"../Project1/data/stone01.dds"));
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the plane model object.", L"Error", MB_OK);
		return false;
	}
	
	

	for (int i = 0; i < 3; i++)
	{
		backGround_RGBA[i] = 0.0f;
	}
	backGround_RGBA[3] = 1.0f;

	// Create the light shader object.
	m_LightShader = new LightShaderClass;
	if (!m_LightShader)
	{
		return false;
	}
	// Initialize the light shader object.
	result = m_LightShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the light shader object.", L"Error", MB_OK);
		return false;
	}
	// Create the light object.
	m_directLight = new LightClass;
	if (!m_directLight)
	{
		return false;
	}

	m_directLight->SetAmbientColor(0.15f, 0.15f, 0.15f, 1.0f);
	m_directLight->SetDiffuseColor(1.0f, 0.0f, 0.0f, 1.0f);
	m_directLight->SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_directLight->SetDirection(0.0f, -1.0f, 0.0f);
	m_directLight->SetSpecularPower(32.0f);

	m_Light1 = new LightClass;
	if (!m_Light1)
	{
		return false;
	}
	// Initialize the light object.
	m_Light1->SetDiffuseColor(0.0f, 1.0f, 1.0f, 1.0f);
	m_Light1->SetPosition(-3.0f, 0.5f, 3.0f);

	m_Light2 = new LightClass;
	if (!m_Light2)
	{
		return false;
	}
	// Initialize the light object.
	m_Light2->SetDiffuseColor(0.0f, 0.0f, 1.0f, 1.0f);
	m_Light2->SetPosition(3.0f, 0.5f, 3.0f);

	m_Light3 = new LightClass;
	if (!m_Light3)
	{
		return false;
	}
	// Initialize the light object.
	m_Light3->SetDiffuseColor(0.0f, 1.0f, 0.0f, 1.0f);
	m_Light3->SetPosition(-3.0f, 0.5f, -3.0f);

	m_Light4 = new LightClass;
	if (!m_Light4)
	{
		return false;
	}
	// Initialize the light object.
	m_Light4->SetDiffuseColor(1.0f, 0.0f, 0.0f, 1.0f);
	m_Light4->SetPosition(3.0f, 0.5f, -3.0f);


	// Create the texture shader object.
	m_TextureShader = new TextureShaderClass;
	if (!m_TextureShader)
	{
		return false;
	}
	// Initialize the texture shader object.
	result = m_TextureShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the texture shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the bitmap object.
	m_Bitmap = new BitmapClass;
	if (!m_Bitmap)
	{
		return false;
	}
	// Initialize the bitmap object.
	result = m_Bitmap->Initialize(m_D3D->GetDevice(), screenWidth, screenHeight,
		const_cast<WCHAR*>(L"../Project1/data/seafloor.dds"), 256, 256);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the bitmap object.", L"Error", MB_OK);
		return false;
	}

	// Initialize a base view matrix with the camera for 2D user interface rendering.
	m_Camera->SetPosition(3.0f, 2.0f, -12.0f);
	m_Camera->Render();
	m_Camera->GetViewMatrix(baseViewMatrix);

	// Create the text object.
	m_Text = new TextClass;
	if (!m_Text)
	{
		return false;
	}
	// Initialize the text object.
	result = m_Text->Initialize(m_D3D->GetDevice(), m_D3D->GetDeviceContext(), hwnd, screenWidth,
		screenHeight, baseViewMatrix);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the text object.", L"Error", MB_OK);
		return false;
	}

	return true;
}
void GraphicsClass::Shutdown()
{
	if (m_D3D)
	{
		m_D3D->Shutdown();
		delete m_D3D;
		m_D3D = 0;
	}
	// Release the light object.
	if (m_directLight)
	{
		delete m_directLight;
		m_directLight = 0;
	}
	if (m_Light1)
	{
		delete m_Light1;
		m_Light1 = 0;
	}

	if (m_Light2)
	{
		delete m_Light2;
		m_Light2 = 0;
	}

	if (m_Light3)
	{
		delete m_Light3;
		m_Light3 = 0;
	}

	if (m_Light4)
	{
		delete m_Light4;
		m_Light4 = 0;
	}
	// Release the light shader object.
	if (m_LightShader)
	{
		m_LightShader->Shutdown();
		delete m_LightShader;
		m_LightShader = 0;
	}
	// Release the model object.
	if (skullModel)
	{
		skullModel->Shutdown();
		delete skullModel;
		skullModel = 0;
	}
	if (gunModel)
	{
		gunModel->Shutdown();
		delete gunModel;
		gunModel = 0;
	}
	if (catModel)
	{
		catModel->Shutdown();
		delete catModel;
		catModel = 0;
	}

	if (planeModel)
	{
		planeModel->Shutdown();
		delete planeModel;
		planeModel = 0;
	}
	// Release the camera object.
	if (m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}
	// Release the bitmap object.
	if (m_Bitmap)
	{
		m_Bitmap->Shutdown();
		delete m_Bitmap;
		m_Bitmap = 0;
	}
	// Release the texture shader object.
	if (m_TextureShader)
	{
		m_TextureShader->Shutdown();
		delete m_TextureShader;
		m_TextureShader = 0;
	}
	// Release the text object.
	if (m_Text)
	{
		m_Text->Shutdown();
		delete m_Text;
		m_Text = 0;
	}
	return;
}
bool GraphicsClass::Frame(int cpu, float frameTime)
{
	bool result;

	static float rotation = 0.0f;
	// Update the rotation variable each frame.
	rotation += (float)(D3DX_PI) / (float)(fps);
	if (rotation > 360.0f)
	{
		rotation -= 360.0f;
	}


	// Render the graphics scene.
	result = Render(rotation);
	if (!result)
	{
		return false;
	}

	result = m_Text->SetFps(fps, m_D3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}
	// Set the cpu usage.
	result = m_Text->SetCpu(cpu, m_D3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}


	return true;
}
void GraphicsClass::SetFps(int fps)
{
	this->fps = fps;
}
bool GraphicsClass::Render(float rotation)
{
	// Clear the buffers to begin the scene.
	D3DXMATRIX viewMatrix, projectionMatrix, worldMatrix,orthoMatrix;
	D3DXMATRIX gunTMat,catMat,skullMat,planeMat;
	D3DXMATRIX rotMatrix_1,rotMatrix_2, scaleMatrix, posMatrix;
	bool result;
	D3DXVECTOR4 diffuseColor[4];
	D3DXVECTOR4 lightPosition[4];
	// Clear the buffers to begin the scene.

	diffuseColor[0] = m_Light1->GetDiffuseColor();
	diffuseColor[1] = m_Light2->GetDiffuseColor();
	diffuseColor[2] = m_Light3->GetDiffuseColor();
	diffuseColor[3] = m_Light4->GetDiffuseColor();

	lightPosition[0] = m_Light1->GetPosition();
	lightPosition[1] = m_Light2->GetPosition();
	lightPosition[2] = m_Light3->GetPosition();
	lightPosition[3] = m_Light4->GetPosition();


	m_D3D->BeginScene(backGround_RGBA[0], backGround_RGBA[1], backGround_RGBA[2], backGround_RGBA[3]);
	// Generate the view matrix based on the camera's position.
	m_Camera->Render();
	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetWorldMatrix(worldMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);
	m_D3D->GetOrthoMatrix(orthoMatrix);

	D3DXMatrixRotationY(&rotMatrix_1, rotation);
	
	D3DXVECTOR3 att = { 1.0f,0.09f,0.032f };
	planeModel->Render(m_D3D->GetDeviceContext());

	result = m_LightShader->Render(m_D3D->GetDeviceContext(), planeModel->GetIndexCount(), worldMatrix,
		viewMatrix, projectionMatrix, planeModel->GetTexture(), diffuseColor, lightPosition,50.0f,att,
		m_directLight->GetDiffuseColor(), m_directLight->GetAmbientColor(),m_Camera->GetPosition(),
		m_directLight->GetSpecularColor(), m_directLight->GetSpecularPower(), m_directLight->GetDirection());
	if (!result)
	{
		return false;
	}

	m_D3D->TurnZBufferOff();
	m_D3D->TurnOnAlphaBlending();
	// Render the text strings.
	result = m_Text->Render(m_D3D->GetDeviceContext(), worldMatrix,orthoMatrix);
	if (!result)
	{
		return false;
	}
	// Turn off alpha blending after rendering the text.
	m_D3D->TurnOffAlphaBlending();
	// Turn the Z buffer back on now that all 2D rendering has completed.
	m_D3D->TurnZBufferOn();

	// Present the rendered scene to the screen.
	m_D3D->EndScene();

	return true;
}
void GraphicsClass::CameraRotControl(D3DXVECTOR2 deltaMouse)
{
	// ===== rotate control =====
	D3DXVECTOR3 rotation = m_Camera->GetRotation();

	m_Camera->SetRotation(rotation.x + deltaMouse.y/2.0f, rotation.y + deltaMouse.x , rotation.z/2.0f);


}
void GraphicsClass::CameraPosControl(int dir)
{
	D3DXVECTOR3 moveDir;
	D3DXMATRIX viewMat;
	m_Camera->GetViewMatrix(viewMat);
	switch (dir)
	{
	case 0: // foward
		moveDir.x = viewMat._13;
		moveDir.y = viewMat._23;
		moveDir.z = viewMat._33;
		break;
	case 1: // back
		moveDir.x = -viewMat._13;
		moveDir.y = -viewMat._23;
		moveDir.z = -viewMat._33;
		break;
	case 2: // left
		moveDir.x = -viewMat._11;
		moveDir.y = -viewMat._21;
		moveDir.z = -viewMat._31;
		break;
	case 3: // right
		moveDir.x = viewMat._11;
		moveDir.y = viewMat._21;
		moveDir.z = viewMat._31;
		break;
	}
	m_Camera->Translate(moveDir, fps);
}
void GraphicsClass::ChangeBackGroundColor(float red, float green, float blue, float alpha)
{
	backGround_RGBA[0] = red;
	backGround_RGBA[1] = green;
	backGround_RGBA[2] = blue;
	backGround_RGBA[3] = alpha;

}

LightShaderClass * GraphicsClass::GetLightShader()
{
	return m_LightShader;
}

LightClass * GraphicsClass::GetLight()
{
	return m_Light1;
}

D3DClass * GraphicsClass::GetD3D()
{
	return m_D3D;
}


