#include "graphicsclass.h"

GraphicsClass::GraphicsClass()
{
	m_D3D = 0;
	m_Camera = 0;
	skullModel = 0;
	planeModel = 0;

	zRotation = 0.0f;

	m_LightShader = 0;
	m_Light = 0;

	fps = 0;
	m_TextureShader = 0;
	m_Bitmap = 0;
	m_Text = 0;
	m_SkyDome = 0;
	m_SkyDomeShader = 0;

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
	m_Camera->SetPosition(0.0f,0.0f,0.0f);
	

	//Create skull model=========================================================

	skullModel = new ModelClass;
	if (!skullModel)
	{
		return false;
	}
	// Initialize the model object.
	result = skullModel->Initialize(m_D3D->GetDevice()
		, const_cast<char*>("../Project1/data/Skull.obj"),
		const_cast<WCHAR*>(L"../Project1/data/Skull.dds"));
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the skull model object.", L"Error", MB_OK);
		return false;
	}
	////Create plane model =========================================================================
	planeModel = new ModelClass;
	if (!planeModel)
	{
		return false;
	}
	// Initialize the model object.
	result = planeModel->Initialize(m_D3D->GetDevice()
		, const_cast<char*>("../Project1/data/plane.obj"),
		const_cast<WCHAR*>(L"../Project1/data/plane.dds"));
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
	m_Light = new LightClass;
	if (!m_Light)
	{
		return false;
	}
	// Initialize the light object.
	m_Light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetAmbientColor(0.15f, 0.15f, 0.15f, 1.0f);
	m_Light->SetDirection(0.0f, 0.0f, 1.0f);
	m_Light->SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetSpecularPower(32.0f);

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
	m_Camera->SetPosition(0.0f, 0.0f, -10.0f);
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

	m_SkyDome = new SkyDomeClass;
	result = m_SkyDome->Initialize(m_D3D->GetDevice());
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the sky dome object.", L"Error", MB_OK);
		return false;
	}

	m_SkyDomeShader = new SkyDomeShaderClass;
	result = m_SkyDomeShader->Initialize(m_D3D->GetDevice(),hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the sky dome shader object.", L"Error", MB_OK);
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
	if (m_Light)
	{
		delete m_Light;
		m_Light = 0;
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
	if (m_SkyDome)
	{
		m_SkyDome->Shutdown();
		delete m_SkyDome;
		m_SkyDome = 0;
	}
	if (m_SkyDomeShader)
	{
		m_SkyDomeShader->Shutdown();
		delete m_SkyDomeShader;
		m_SkyDomeShader = 0;
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
	D3DXMATRIX skullMat,planeMat;
	D3DXMATRIX rotMatrix_1,rotMatrix_2, scaleMatrix, posMatrix;
	bool result;
	// Clear the buffers to begin the scene.

	m_D3D->BeginScene(backGround_RGBA[0], backGround_RGBA[1], backGround_RGBA[2], backGround_RGBA[3]);
	// Generate the view matrix based on the camera's position.
	m_Camera->Render();
	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetWorldMatrix(worldMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);
	m_D3D->GetOrthoMatrix(orthoMatrix);

	D3DXVECTOR3 cameraPos = m_Camera->GetPosition();
	m_D3D->TurnZBufferOff();
	m_D3D->TurnOnAlphaBlending();
	// Render the text strings.
	result = m_Text->Render(m_D3D->GetDeviceContext(), worldMatrix, orthoMatrix);
	if (!result)
	{
		return false;
	}
	m_D3D->TurnOffAlphaBlending();

	m_D3D->TurnOffCulling();
	D3DXMatrixTranslation(&worldMatrix , cameraPos.x, cameraPos.y-0.5f, cameraPos.z);

	m_SkyDome->Render(m_D3D->GetDeviceContext());
	m_SkyDomeShader->Render(m_D3D->GetDeviceContext(), m_SkyDome->GetIndexCount(), worldMatrix, viewMatrix,
		projectionMatrix, m_SkyDome->GetApexColor(), m_SkyDome->GetCenterColor());
	m_D3D->TurnOnCulling();

	// Turn off alpha blending after rendering the text.

	// Turn the Z buffer back on now that all 2D rendering has completed.
	m_D3D->TurnZBufferOn();


	D3DXMatrixRotationY(&rotMatrix_1, rotation);
	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.


	//plane render =======================================================

	planeModel->GetTransformMatrix(planeMat);
	planeModel->Render(m_D3D->GetDeviceContext());

	D3DXMatrixTranslation(&posMatrix, 0.0f, -10.0f, 0.0f);
	D3DXMatrixRotationX(&rotMatrix_2, (float)D3DX_PI / 2);

	planeMat = posMatrix;
	result = m_LightShader->Render(m_D3D->GetDeviceContext(), planeModel->GetIndexCount(), planeMat,
		viewMatrix, projectionMatrix, planeModel->GetTexture(), m_Light->GetDirection(),
		m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), m_Camera->GetPosition(),
		m_Light->GetSpecularColor(), m_Light->GetSpecularPower());
	if (!result)
	{
		return false;
	}
	//Skull render =======================================================
	
	skullModel->GetTransformMatrix(skullMat);

	D3DXMatrixTranslation(&posMatrix, -10.0f, -5.0f, 0.0f);
	D3DXMatrixScaling(&scaleMatrix, 2.0f, 2.0f, 2.0f);
	skullMat = scaleMatrix * rotMatrix_1 *  posMatrix;
	skullModel->Render(m_D3D->GetDeviceContext());


	result = m_LightShader->Render(m_D3D->GetDeviceContext(), skullModel->GetIndexCount(), skullMat,
		viewMatrix, projectionMatrix, skullModel->GetTexture(), m_Light->GetDirection(),
		m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), m_Camera->GetPosition(),
		m_Light->GetSpecularColor(), m_Light->GetSpecularPower());
	if (!result)
	{
		return false;
	}


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
	return m_Light;
}

D3DClass * GraphicsClass::GetD3D()
{
	return m_D3D;
}


