////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include"pch.h"
#include "graphicsclass.h"


GraphicsClass::GraphicsClass()
{
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
	m_Direct3D = new D3DClass;
	if(!m_Direct3D)
	{
		return false;
	}

	// Initialize the Direct3D object.
	result = m_Direct3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D.", L"Error", MB_OK);
		return false;
	}

	// Create the camera object.
	m_Camera = new CameraClass;
	if (!m_Camera)
	{
		return false;
	}

	// Set the initial position of the camera.
	m_Camera->SetPosition(0.0f, 0.0f, -5.0f);

	// Create the color shader object.
	m_shaderSystem = new ShaderSystem(m_Direct3D->GetDevice(),m_Direct3D->GetDeviceContext());
	if (!m_shaderSystem)
	{
#ifdef _DEBUG
		cout << "initialize ShaderSystem Error"<<endl;
#endif
		return false;
	}

	m_timer = new Timer;



	/*Mesh* meshTemp = new Mesh(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), m_Shader);
	meshTemp->LoadMesh("../Asset/wolf.dae");
	meshTemp->position = { 0,0,10 };*/

	StaticMesh* objTest = new StaticMesh(m_Direct3D->GetDevice(),m_Direct3D->GetDeviceContext(),m_shaderSystem);
	objTest->LoadMesh("../Asset/spaceCompound.obj",true,false);

	object.push_back(static_cast<Model*>(objTest));
	

	return true;
}

bool GraphicsClass::SetModel()
{
	
	return false;
}


void GraphicsClass::Shutdown()
{


	// Release the camera object.
	if (m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	// Release the D3D object.
	if(m_Direct3D)
	{
		m_Direct3D->Shutdown();
		delete m_Direct3D;
		m_Direct3D = 0;
	}
	if (m_timer)
	{
		delete m_timer;
		m_timer = nullptr;
	}

	object.clear();

	return;
}


bool GraphicsClass::Frame()
{
	bool result;


	// Render the graphics scene.
	result = Render();
	if(!result)
	{
		return false;
	}

	return true;
}


bool GraphicsClass::Render()
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
	bool result;


	// Clear the buffers to begin the scene.
	m_Direct3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);

	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	for (auto mesh : object)
	{
		//mesh->position.y = -50;
		mesh->position.z = 0;
		mesh->rotation.x = -45*PI/180.0f;
		mesh->Render(m_timer->GetFrameTime(),worldMatrix, viewMatrix, projectionMatrix);
	}

	// Present the rendered scene to the screen.
	m_Direct3D->EndScene();

	return true;
}