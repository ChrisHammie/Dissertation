

#include "GraphicsClass.h"
#include "AntTweakBar.h"

GraphicsClass::GraphicsClass()
{
	m_Direct3D = 0;
	m_Camera = 0;

	m_Model1 = 0;
	m_Model2 = 0;

	m_TextureShader = 0;

	m_LightShader1 = 0;
	m_LightShader2 = 0;

	m_Light = 0;
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
	if (!m_Direct3D)
	{
		return false;
	}

	// Initialize the Direct3D object.
	result = m_Direct3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D.", L"Error", MB_OK);
		return false;
	}

	TwInit(TW_DIRECT3D11, m_Direct3D->GetDevice());
	TwWindowSize(screenWidth, screenHeight);

	TwBar* tweakBar;
	tweakBar = TwNewBar("Dissertation");
	
	

	// Create the model object.
	m_Model1 = new ModelClass;
	if (!m_Model1)
	{
		return false;
	}

	m_Model2 = new ModelClass;
	if (!m_Model2)
	{
		return false;
	}

	result = m_Model1->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), "stone01.tga","square.txt");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	/*result = m_Model1->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), "water.tga", "flat.txt");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}*/

	// Initialize the model object.
	/*result = m_Model->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), "stone01.tga");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}*/

	m_Camera = new CameraClass;
	if (!m_Camera)
	{
		return false;
	}
	m_Camera->SetPosition(0.0f, 5.0f, -10.0f);
	m_Camera->SetRotation(30.0f, 0.0f, 0.0f);

	

	TwAddVarRW(tweakBar, "Test", TW_TYPE_FLOAT, &test, "");
	
	TwAddVarRW(tweakBar, "CamPos", TW_TYPE_FLOAT, &camPos, "");
	
	TwAddVarRW(tweakBar, "Rotation", TW_TYPE_BOOLCPP, &rot, "");
	
	// Create the color shader object.
	//m_TextureShader = new TextureShaderClass;
	//if (!m_TextureShader)
	//{
	//	return false;
	//}

	//// Initialize the color shader object.
	//result = m_TextureShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	//if (!result)
	//{
	//	MessageBox(hwnd, L"Could not initialize the color shader object.", L"Error", MB_OK);
	//	return false;
	//}

	// Create the light shader object.
	m_LightShader1 = new LightShaderClass;
	if (!m_LightShader1)
	{
		return false;
	}

	m_LightShader2 = new LightShaderClass;
	if (!m_LightShader2)
	{
		return false;
	}

	// Initialize the light shader object.
	result = m_LightShader1->Initialize(m_Direct3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the light shader object.", L"Error", MB_OK);
		return false;
	}

	/*result = m_LightShader2->Initialize(m_Direct3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the light shader object.", L"Error", MB_OK);
		return false;
	}*/

	// Create the light object.
	m_Light = new LightClass;
	if (!m_Light)
	{
		return false;
	}

	m_Light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetDirection(0.0f, 1.0f, 0.0f);

	return true;
}


void GraphicsClass::Shutdown()
{
	// Release the color shader object.
	if (m_TextureShader)
	{
		m_TextureShader->Shutdown();
		delete m_TextureShader;
		m_TextureShader = 0;
	}

	// Release the model object.
	if (m_Model1)
	{
		m_Model1->Shutdown();
		delete m_Model1;
		m_Model1 = 0;
	}

	if (m_Model2)
	{
		m_Model2->Shutdown();
		delete m_Model2;
		m_Model2 = 0;
	}

	// Release the camera object.
	if (m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	// Release the D3D object.
	if (m_Direct3D)
	{
		m_Direct3D->Shutdown();
		delete m_Direct3D;
		m_Direct3D = 0;
	}

	// Release the light object.
	if (m_Light)
	{
		delete m_Light;
		m_Light = 0;
	}

	// Release the light shader object.
	if (m_LightShader1)
	{
		m_LightShader1->Shutdown();
		delete m_LightShader1;
		m_LightShader1 = 0;
	}

	if (m_LightShader2)
	{
		m_LightShader2->Shutdown();
		delete m_LightShader2;
		m_LightShader2 = 0;
	}

	return;
}


bool GraphicsClass::Frame(int mouseX, int mouseY)
{
	bool result;

	static float rotation = 0.0f;


	// Update the rotation variable each frame.
	if (rot)
	{
		rotation += (float)3.14 * 0.01f;
		if (rotation > 360.0f)
		{
			rotation -= 360.0f;
		}
	}
	

	/*result = m_Text->SetMousePosition(mouseX, mouseY, m_D3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}*/

	camPos = m_Camera->GetPosZ();
	
	

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
	bool result;

	
	// Clear the buffers to begin the scene.
	m_Direct3D->BeginScene(0.0f, 0.2f, 0.4f, 1.0f);

	TwDraw();

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();
	

	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);

	
	

	//XMMatrixRotationY(rotation);
	
	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_Model1->Render(m_Direct3D->GetDeviceContext());
	//m_Model2->Render(m_Direct3D->GetDeviceContext());
	// Render the model using the color shader.
	/*result = m_TextureShader->Render(m_Direct3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, m_Model->GetTexture());
	if (!result)
	{
		return false;
	}*/
	if (rot)
	{
		worldMatrix *= XMMatrixRotationY(rotation);
	}
	
	

	// Render the model using the light shader.
	result = m_LightShader1->Render(m_Direct3D->GetDeviceContext(), m_Model1->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
		m_Model1->GetTexture(), m_Light->GetDirection(), m_Light->GetDiffuseColor());
	if (!result)
	{
		return false;
	}

	/*result = m_LightShader2->Render(m_Direct3D->GetDeviceContext(), m_Model2->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
		m_Model2->GetTexture(), m_Light->GetDirection(), m_Light->GetDiffuseColor());
	if (!result)
	{
		return false;
	}*/

	// Present the rendered scene to the screen.
	m_Direct3D->EndScene();

	return true;
}


