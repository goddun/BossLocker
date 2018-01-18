////////////////////////////////////////////////////////////////////////////////
// Filename: Scene.cpp
////////////////////////////////////////////////////////////////////////////////
#include"kpu.h"
#include "Scene.h"


Scene::Scene()
{

}


Scene::Scene(const Scene& other)
{
}


Scene::~Scene()
{
}


////////////////////////////Á¤Á¡ ³Ö±â///////////////////////////////////////////////////////////



bool Scene::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;
	D3DXMATRIX baseViewMatrix;

	// Create the Direct3D object.
	m_D3D = new D3DClass;
	if(!m_D3D)
	{
		return false;
	}

	// Initialize the Direct3D object.
	result = m_D3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D.", L"Error", MB_OK);
		return false;
	}

	// Create the camera object.
	m_Camera = new CameraClass;
	if(!m_Camera)
	{
		return false;
	}
	// Set the initial position of the camera.
	m_Camera->SetPosition(0.0f, 0.0f, -10.0f);






	// Create the model object.
	m_Model = new TextureModel();
	// Initialize the model object.
	if(!m_Model->Initialize(m_D3D->GetDevice(), "../Asset/zebu.fbx",FBX))
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}
	m_Model->LoadTexture(m_D3D->GetDevice(), L"data/seafloor.dds",DDS);

	m_ColorModel = new ColorModel();
	if (!m_ColorModel->Initialize(m_D3D->GetDevice(), "../Asset/zebu.fbx", FBX))
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}
	

	// Create the color shader object.
	m_LightShader = new LightShaderClass;
	if(!m_LightShader)
	{
		return false;
	}
	// Initialize the color shader object.
	result = m_LightShader->Initialize(m_D3D->GetDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the light shader object.", L"Error", MB_OK);
		return false;
	}
	m_ColorShader = new ColorShaderClass;
	if (!m_ColorShader)
	{
		return false;
	}
	result = m_ColorShader->Initialize(m_D3D->GetDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the color shader object.", L"Error", MB_OK);
		return false;
	}

	m_light = new Light();
	m_light->SetDiffuseColor(1.0f, 0.0f, 1.0f, 1.0f);
	m_light->SetDirection(0.0f, 0.0f, 1.0f);

	// Initialize a base view matrix with the camera for 2D user interface rendering. 
	m_Camera->SetPosition(0.0f, 0.0f, -1.0f);
	m_Camera->Render();
	m_Camera->GetViewMatrix(baseViewMatrix);

	m_Text = new TextClass;
	if (!m_Text)
	{
		return false;
	}
	result = m_Text->Initialize(m_D3D->GetDevice(), m_D3D->GetDeviceContext(), hwnd, screenWidth, screenHeight, baseViewMatrix);
	if (!result) 
	{ 
		MessageBox(hwnd, L"Could not initialize the text object.", L"Error", MB_OK); 
		return false; 
	}

	return true;
}


void Scene::Shutdown()
{
	// Release the color shader object.
	if(m_ColorShader)
	{
		m_ColorShader->Shutdown();
		delete m_ColorShader;
		m_ColorShader = nullptr;
	}
	if (m_TextureShader)
	{
		m_TextureShader->Shutdown();
		delete m_TextureShader;
		m_TextureShader = nullptr;
	}
	// Release the model object.
	if(m_Model)
	{
		m_Model->Shutdown();
		delete m_Model;
		m_Model = nullptr;
	}
	if (m_ColorModel)
	{
		m_ColorModel->Shutdown();
		delete m_ColorModel;
		m_ColorModel = nullptr;
	}
	// Release the camera object.
	if(m_Camera)
	{
		delete m_Camera;
		m_Camera = nullptr;
	}

	// Release the D3D object.
	if(m_D3D)
	{
		m_D3D->Shutdown();
		delete m_D3D;
		m_D3D = nullptr;
	}
	if (m_light)
	{
		delete m_light;
		m_light = nullptr;
	}
	if (m_Text)
	{
		m_Text->Shutdown();
		delete m_Text;
		m_Text = nullptr;
	}
	return;
}


bool Scene::Frame(int fps, int cpu, float frameTime)
{
	bool result;

	result = m_Text->SetCpu(cpu, m_D3D->GetDeviceContext());
	if (!result) { return false; }

	result = m_Text->SetFps(fps, m_D3D->GetDeviceContext());
	if (!result) { return false; }

	// Render the graphics scene.
	result = Render();
	if(!result)
	{
		return false;
	}

	m_Camera->SetPosition(0.0f, 0.0f, -10.0f);


	return true;
}


bool Scene::Render()
{
	D3DXMATRIX worldMatrix, viewMatrix, projectionMatrix,orthoMatrix;
	bool result;


	// Clear the buffers to begin the scene.
	m_D3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetWorldMatrix(worldMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);
	m_D3D->GetOrthoMatrix(orthoMatrix);



	//========================3D·»´õ¸µ====================================================
	D3DXMATRIX matTranslate;
	D3DXMatrixTranslation(&matTranslate, 0, -0.2, 0);

	D3DXMATRIX matScale;
	D3DXMatrixScaling(&matScale, 0.001, 0.001, 0.001);

	D3DXMATRIX matRotate;
	D3DXMatrixRotationYawPitchRoll(&matRotate, 90 * PI / 180 ,0, 0);

	worldMatrix *= matScale * matRotate * matTranslate;
	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_Model->Render(m_D3D->GetDeviceContext());
	// Render the model using the color shader.
	result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Model->GetVertexCount(), worldMatrix, viewMatrix, projectionMatrix, m_Model->GetTexture(),m_light->GetDirection() ,m_light->GetDiffuseColor());

	if(!result)
	{
		return false;
	}

	//========================2D·»´õ¸µ====================================================
	D3DXMatrixIdentity(&worldMatrix);
	// Turn off the Z buffer to begin all 2D rendering. 
	m_D3D->TurnZBufferOff();
	// Turn on the alpha blending before rendering the text. 
	m_D3D->TurnOnAlphaBlending();
	// Render the text strings. 
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