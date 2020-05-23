#include "graphicsclass.h"

GraphicsClass::GraphicsClass()
{
	m_D3D = 0;
	m_Camera = 0;
	m_TriangleModel = 0;
	m_RectangleModel = 0;
	m_PentagonModel = 0;
	m_ColorShader = 0;

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

	cameraPos = new D3DXVECTOR3(0.0f, 0.0f, -10.0f);
	if (!cameraPos)
	{
		return false;
	}
	// Set the initial position of the camera.
	m_Camera->SetPosition(cameraPos->x, cameraPos->y, cameraPos->z);
	
	//Create TriangleModel =========================================================================
	m_TriangleModel = new ModelClass;
	if (!m_TriangleModel)
	{
		return false;
	}
	// Initialize the model object.
	result = m_TriangleModel->Initialize(m_D3D->GetDevice(),3);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}
	//Create RectangleModel ===========================================================================
	m_RectangleModel = new ModelClass;
	if (!m_RectangleModel)
	{
		return false;
	}
	// Initialize the model object.
	result = m_RectangleModel->Initialize(m_D3D->GetDevice(), 4);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}
	//Create PentagonModel================================================================================
	m_PentagonModel = new ModelClass;
	if (!m_PentagonModel)
	{
		return false;
	}
	// Initialize the model object.
	result = m_PentagonModel->Initialize(m_D3D->GetDevice(), 5);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	//===================================================================================
	// Create the color shader object.
	m_ColorShader = new ColorShaderClass;
	if (!m_ColorShader)
	{
		return false;
	}
	// Initialize the color shader object.
	result = m_ColorShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the color shader object.", L"Error", MB_OK);
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
	if (m_ColorShader)
	{
		m_ColorShader->Shutdown();
		delete m_ColorShader;
		m_ColorShader = 0;
	}
	// Release the model object.
	if (m_TriangleModel)
	{
		m_TriangleModel->Shutdown();
		delete m_TriangleModel;
		m_TriangleModel = 0;
	}
	if (m_RectangleModel)
	{
		m_RectangleModel->Shutdown();
		delete m_RectangleModel;
		m_RectangleModel = 0;
	}
	if (m_PentagonModel)
	{
		m_PentagonModel->Shutdown();
		delete m_PentagonModel;
		m_PentagonModel = 0;
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

	zRotation += ((float)3.14159f*0.01f);
	if (zRotation > 360.0f)
		zRotation = 0.0f;

	m_Camera->SetPosition(cameraPos->x, cameraPos->y, cameraPos->z);

	// Render the graphics scene.
	result = Render();
	if (!result)
	{
		return false;
	}


	return true;
}
bool GraphicsClass::Render()
{
	// Clear the buffers to begin the scene.
	D3DXMATRIX viewMatrix, projectionMatrix, worldMatrix;
	bool result;
	// Clear the buffers to begin the scene.

	m_D3D->BeginScene(backGround_RGBA[0], backGround_RGBA[1], backGround_RGBA[2], backGround_RGBA[3]);
	// Generate the view matrix based on the camera's position.
	m_Camera->Render();
	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetWorldMatrix(worldMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);

	D3DXMatrixRotationAxis(&worldMatrix, new D3DXVECTOR3(0.0f, 0.0f, 1.0f), zRotation);
	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_TriangleModel->Render(m_D3D->GetDeviceContext());
	// Render the model using the color shader.
	result = m_ColorShader->Render(m_D3D->GetDeviceContext(), m_TriangleModel->GetIndexCount(), worldMatrix,
		viewMatrix, projectionMatrix);
	if (!result)
	{
		return false;
	}

	m_RectangleModel->Render(m_D3D->GetDeviceContext());
	result = m_ColorShader->Render(m_D3D->GetDeviceContext(), m_RectangleModel->GetIndexCount(), worldMatrix,
		viewMatrix, projectionMatrix);
	if (!result)
	{
		return false;
	}

	m_PentagonModel->Render(m_D3D->GetDeviceContext());
	result = m_ColorShader->Render(m_D3D->GetDeviceContext(), m_PentagonModel->GetIndexCount(), worldMatrix,
		viewMatrix, projectionMatrix);
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

D3DClass * GraphicsClass::GetD3D()
{
	return m_D3D;
}

ColorShaderClass * GraphicsClass::GetColorShader()
{
	return m_ColorShader;
}
