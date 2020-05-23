#include "graphicsclass.h"

GraphicsClass::GraphicsClass()
{
	m_D3D = 0;
	m_Camera = 0;
	gunModel = 0;
	skullModel = 0;
	catModel = 0;
	planeModel = 0;
	m_TextureShader = 0;

	cameraPos = 0;
	zRotation = 0.0f;
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

	cameraPos = new D3DXVECTOR3(0.0f, 0.0f, -40.0f);
	if (!cameraPos)
	{
		return false;
	}
	// Set the initial position of the camera.
	m_Camera->SetPosition(cameraPos->x, cameraPos->y, cameraPos->z);
	
	//Create GunModel =========================================================================
	gunModel = new ModelClass;
	if (!gunModel)
	{
		return false;
	}
	// Initialize the model object.
		result = gunModel->Initialize(m_D3D->GetDevice()
			, const_cast<char*>("../Project1/data/gun.obj"),
			const_cast<WCHAR*>(L"../Project1/data/gun.dds"));
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the gun model object.", L"Error", MB_OK);
		return false;
	}

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

	//create cat model===================================================

	catModel = new ModelClass;
	if (!catModel)
	{
		return false;
	}
	// Initialize the model object.
	result = catModel->Initialize(m_D3D->GetDevice()
		, const_cast<char*>("../Project1/data/cat.obj"),
		const_cast<WCHAR*>(L"../Project1/data/cat.dds"));
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the cat model object.", L"Error", MB_OK);
		return false;
	}


	//Create plane model =========================================================================
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
	
	m_TextureShader = new TextureShaderClass;
	if (!m_TextureShader)
	{
		return false;
	}
	// Initialize the color shader object.
	result = m_TextureShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the texture shader object.", L"Error", MB_OK);
		return false;
	}

	for (int i = 0; i < 3; i++)
	{
		backGround_RGBA[i] = 0.0f;
	}
	backGround_RGBA[3] = 1.0f;


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

	// Release the color shader object.
	if(m_TextureShader)
	{
		m_TextureShader->Shutdown();
		delete m_TextureShader;
		m_TextureShader = 0;
	}
	// Release the model object.
	if (gunModel)
	{
		gunModel->Shutdown();
		delete gunModel;
		gunModel = 0;
	}
	if (skullModel)
	{
		skullModel->Shutdown();
		delete skullModel;
		skullModel = 0;
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
	return;
}
bool GraphicsClass::Frame()
{
	bool result;

	static float rotation = 0.0f;
	// Update the rotation variable each frame.
	rotation += (float)D3DX_PI * 0.01f;
	if (rotation > 360.0f)
	{
		rotation -= 360.0f;
	}

	m_Camera->SetPosition(cameraPos->x, cameraPos->y, cameraPos->z);

	// Render the graphics scene.
	result = Render(rotation);
	if (!result)
	{
		return false;
	}


	return true;
}
bool GraphicsClass::Render(float rotation)
{
	// Clear the buffers to begin the scene.
	D3DXMATRIX viewMatrix, projectionMatrix, worldMatrix;
	D3DXMATRIX gunTMat,catMat,skullMat,planeMat;
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

	D3DXMatrixRotationY(&rotMatrix_1, rotation);
	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.

	gunModel->GetTransformMatrix(gunTMat);
	D3DXMatrixTranslation(&posMatrix, 0.0f, 10.0f, 0.0f);
	D3DXMatrixScaling(&scaleMatrix, 0.5f, 0.5f, 0.5f);
	gunTMat = scaleMatrix * rotMatrix_1 * posMatrix;
	gunModel->Render(m_D3D->GetDeviceContext());
	// Render the model using the texture shader.
	result = m_TextureShader->Render(m_D3D->GetDeviceContext(), gunModel->GetIndexCount(),
		gunTMat, viewMatrix, projectionMatrix, gunModel->GetTexture());
	if (!result)
	{
		return false;
	}

	// Cat render ==================================================
	catModel->GetTransformMatrix(catMat);

	D3DXMatrixScaling(&scaleMatrix, 0.2f, 0.2f, 0.2f);
	D3DXMatrixRotationX(&rotMatrix_2, (float)D3DX_PI/2);
	D3DXMatrixTranslation(&posMatrix, 10.0f, -5.0f, 0.0f);
	catMat = scaleMatrix * rotMatrix_2 *rotMatrix_1 * posMatrix;
	catModel->Render(m_D3D->GetDeviceContext());
	result = m_TextureShader->Render(m_D3D->GetDeviceContext(), catModel->GetIndexCount(),
		catMat, viewMatrix, projectionMatrix, catModel->GetTexture());
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
	result = m_TextureShader->Render(m_D3D->GetDeviceContext(), skullModel->GetIndexCount(),
		skullMat, viewMatrix, projectionMatrix, skullModel->GetTexture());

	//plane render =======================================================

	planeModel->GetTransformMatrix(planeMat);
	planeModel->Render(m_D3D->GetDeviceContext());

	D3DXMatrixTranslation(&posMatrix, 0.0f, -10.0f, 0.0f);
	D3DXMatrixRotationX(&rotMatrix_2, (float)D3DX_PI / 2);

	planeMat = posMatrix;
	result = m_TextureShader->Render(m_D3D->GetDeviceContext(), planeModel->GetIndexCount(),
		planeMat, viewMatrix, projectionMatrix, planeModel->GetTexture());
	if (!result)
	{
		return false;
	}

	// Present the rendered scene to the screen.
	m_D3D->EndScene();

	return true;
}
void GraphicsClass::ChangeBackGroundColor(float red, float green, float blue, float alpha)
{
	backGround_RGBA[0] = red;
	backGround_RGBA[1] = green;
	backGround_RGBA[2] = blue;
	backGround_RGBA[3] = alpha;

}

D3DXVECTOR3 * GraphicsClass::GetCameraPosition()
{
	return cameraPos;
}

TextureShaderClass * GraphicsClass::GetTextureShader()
{
	return m_TextureShader;
}

D3DClass * GraphicsClass::GetD3D()
{
	return m_D3D;
}


