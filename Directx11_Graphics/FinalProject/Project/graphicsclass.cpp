#include "graphicsclass.h"

GraphicsClass::GraphicsClass()
{
	m_D3D = 0;
	m_Camera = 0;
	m_LightShader = 0;
	m_Light = 0;
	m_TextureShader = 0;
	m_Text = 0;

	useLightingEffect[0] = true;
	useLightingEffect[1] = true;
	useLightingEffect[2] = true;

	allPolygonCount = 0;

	m_SkyDome = 0;
	m_SkyDomeShader = 0;
	m_SkyPlane = 0;
	m_SkyPlaneShader = 0;

	//모델
	drone = 0;
	city = 0;
	bottom = 0;
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
	result = m_D3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
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

	// Initialize a base view matrix with the camera for 2D user interface rendering.
	m_Camera->SetPosition(0.0f, 0.0f, -15.0f);
	m_Camera->Render(D3DXVECTOR3(0, 0, 1));
	m_Camera->GetViewMatrix(baseViewMatrix);

	// Create the text object.
	m_Text = new TextClass;
	if (!m_Text)
	{
		return false;
	}

	// Initialize the text object.
	result = m_Text->Initialize(m_D3D->GetDevice(), m_D3D->GetDeviceContext(), hwnd, screenWidth, screenHeight, baseViewMatrix);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the text object.", L"Error", MB_OK);
		return false;
	}

	//드론 초기화
	drone = new ModelClass;
	if (!drone)
	{
		return false;
	}

	result = drone->Initialize(m_D3D->GetDevice(), "../Project/data/drone.obj", L"../Project/data/drone.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}
		
	drone->SetScaling(0.05, 0.05, 0.05);
	drone->SetPosition(0, 500, 0);
	allPolygonCount += drone->GetPolygonCount();

	//도시 초기화
	city = new MultiModelClass;
	if (!city)
	{
		return false;
	}

	result = city->Initialize(m_D3D->GetDevice(), "../Project/data/city.obj", "../Project/data/city texture/", "../Project/data/city.mtl");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the city object.", L"Error", MB_OK);
		return false;
	}

	//city->SetPosition(0,0,0);
	city->SetScaling(0.05, 0.05, 0.05);

	allPolygonCount += city->GetPolygonCount();

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
	m_Light->SetAmbientColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
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

	m_SkyDome = new SkyDomeClass;
	result = m_SkyDome->Initialize(m_D3D->GetDevice());
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the sky dome object.", L"Error", MB_OK);
		return false;
	}

	m_SkyDomeShader = new SkyDomeShaderClass;
	result = m_SkyDomeShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the sky dome shader object.", L"Error", MB_OK);
		return false;
	}
	// 하늘 평면 객체를 생성합니다.
	m_SkyPlane = new SkyPlaneClass;
	if (!m_SkyPlane)
	{
		return false;
	}

	// 하늘 평면 객체를 초기화 합니다.
	result = m_SkyPlane->Initialize(m_D3D->GetDevice(), L"../Project/data/cloud001.dds", L"../Project/data/cloud002.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the sky plane object.", L"Error", MB_OK);
		return false;
	}
	// 하늘 평면 쉐이더 객체를 생성합니다.
	m_SkyPlaneShader = new SkyPlaneShaderClass;
	if (!m_SkyPlaneShader)
	{
		return false;
	}

	// 하늘 평면 쉐이더 객체를 초기화 합니다.
	result = m_SkyPlaneShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the sky plane shader object.", L"Error", MB_OK);
		return false;
	}

	return true;
}

void GraphicsClass::Shutdown()
{
	// 하늘 평면 쉐이더 객체를 해제합니다.
	if (m_SkyPlaneShader)
	{
		m_SkyPlaneShader->Shutdown();
		delete m_SkyPlaneShader;
		m_SkyPlaneShader = 0;
	}

	// 하늘 평면 객체를 해제합니다.
	if (m_SkyPlane)
	{
		m_SkyPlane->Shutdown();
		delete m_SkyPlane;
		m_SkyPlane = 0;
	}
	// Release the text object.
	if (m_Text)
	{
		m_Text->Shutdown();
		delete m_Text;
		m_Text = 0;
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

	if (drone)
	{
		drone->Shutdown();
		delete drone;
		drone = 0;
	}

	// Release the texture shader object.
	if (m_TextureShader)
	{
		m_TextureShader->Shutdown();
		delete m_TextureShader;
		m_TextureShader = 0;
	}

	// Release the camera object.
	if (m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	// Release the Direct3D object.
	if (m_D3D)
	{
		m_D3D->Shutdown();
		delete m_D3D;
		m_D3D = 0;
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

bool GraphicsClass::Frame(int fps, int cpu, float frameTime)
{
	bool result;
	static int i = 0;

	if (!drone->IsDroneMoved()) {
		D3DXVECTOR3 droneRotation = drone->GetRotation();
		if (i == 5)
			drone->SetRotation(droneRotation.x, droneRotation.y, 2.0f);
		else if (i == 10)
			drone->SetRotation(droneRotation.x, droneRotation.y, -2.0f);

		if (i > 10) i = 0;

		i++;
	}
	// Render the graphics scene.
	result = Render();

	if (!result)
	{
		return false;
	}

	// Set the frames per second.
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
	// 하늘 평면 프레임 처리를 수행합니다.
	m_SkyPlane->Frame();
	m_SkyDome->Frame();
	m_Light->SetAmbientColor(m_SkyDome->GetCenterColor());
	m_Light->SetDiffuseColor(m_SkyDome->GetCenterColor());
	m_Light->SetSpecularColor(m_SkyDome->GetCenterColor());
	result = m_Text->SetPolygonNum(allPolygonCount, m_D3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}

	return true;
}

CameraClass * GraphicsClass::GetCamera()
{
	return m_Camera;
}

ModelClass * GraphicsClass::GetDrone()
{
	return drone;
}

bool GraphicsClass::Render()
{
	D3DXMATRIX viewMatrix, projectionMatrix, worldMatrix, orthoMatrix, skyDomeMatrix;
	bool result;

	// Clear the buffers to begin the scene.
	m_D3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Set of FillMode
	result = m_D3D->ChangeFillMode();
	if (!result)
	{
		return false;
	}

	// Generate the view matrix based on the camera's position.
	m_Camera->Render(drone->GetPosition());

	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetWorldMatrix(worldMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);
	m_D3D->GetOrthoMatrix(orthoMatrix);

	// Turn off the Z buffer to begin all 2D rendering.
	m_D3D->TurnZBufferOff();
	D3DXVECTOR3 cameraPos = m_Camera->GetPosition();
	m_D3D->TurnOffCulling();
	D3DXMatrixTranslation(&skyDomeMatrix, cameraPos.x, cameraPos.y, cameraPos.z);
	m_SkyDome->Render(m_D3D->GetDeviceContext());
	m_SkyDomeShader->Render(m_D3D->GetDeviceContext(), m_SkyDome->GetIndexCount(), skyDomeMatrix, viewMatrix, projectionMatrix, m_SkyDome->GetApexColor(), m_SkyDome->GetCenterColor());
	m_D3D->TurnOnCulling();

	// 구름이 sky dome color와 혼합되도록 첨가물 블렌딩을 가능하게합니다.
	m_D3D->EnableSecondBlendState();

	// 하늘 평면 쉐이더를 사용하여 하늘 평면을 렌더링합니다.
	//m_SkyPlane->Render(m_D3D->GetDeviceContext());
	//m_SkyPlaneShader->Render(m_D3D->GetDeviceContext(), m_SkyPlane->GetIndexCount(), skyDomeMatrix, viewMatrix, projectionMatrix, m_SkyPlane->GetCloudTexture1(), m_SkyPlane->GetCloudTexture2(), m_SkyPlane->GetTranslation(0), m_SkyPlane->GetTranslation(1), m_SkyPlane->GetTranslation(2), m_SkyPlane->GetTranslation(3), m_SkyPlane->GetBrightness());

	// 블렌드를 끕니다.
	m_D3D->TurnOffAlphaBlending();
	m_D3D->TurnZBufferOn();

	//드론
	drone->Render(m_D3D->GetDeviceContext());

	result = m_LightShader->Render(m_D3D->GetDeviceContext(), drone->GetIndexCount(), drone->GetWorldMatrix(), viewMatrix, projectionMatrix, drone->GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower(), useLightingEffect);
	if (!result)
	{
		return false;
	}

	//도시
	city->SetLightShaderClass(m_LightShader);

	city->Render(m_D3D->GetDeviceContext(), viewMatrix, projectionMatrix, m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower(), useLightingEffect);

	// Turn off the Z buffer to begin all 2D rendering.
	m_D3D->TurnZBufferOff();

	// Turn on the alpha blending before rendering the text. 
	m_D3D->TurnOnAlphaBlending();
	//m_Text->SetViewMatrix(viewMatrix);
	result = m_Text->Render(m_D3D->GetDeviceContext(), worldMatrix, orthoMatrix);
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