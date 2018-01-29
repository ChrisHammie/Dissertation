
#include "InputClass.h"
#include "CameraClass.h"
#include "GraphicsClass.h"


InputClass::InputClass()
{
	m_directInput = 0;
	m_keyboard = 0;
	m_mouse = 0;

	m_Camera = 0;
	


}


InputClass::InputClass(const InputClass& other)
{
}


InputClass::~InputClass()
{
}


bool InputClass::Initialize(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight)
{
	HRESULT result;

	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;

	m_mouseX = 0;
	m_mouseY = 0;

	result = DirectInput8Create(hinstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_directInput, NULL);
	if (FAILED(result))
	{
		return false;
	}

	result = m_directInput->CreateDevice(GUID_SysKeyboard, &m_keyboard, NULL);
	if (FAILED(result))
	{
		return false;
	}

	
	result = m_keyboard->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(result))
	{
		return false;
	}

	//Allows only this program to see input
	result = m_keyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE); //EXCLUSIVE means only this application sees the input
	if (FAILED(result))
	{
		return false;
	}

	//Acquire the use of the keyboard
	result = m_keyboard->Acquire();
	if (FAILED(result))
	{
		return false;
	}
	
	result = m_directInput->CreateDevice(GUID_SysMouse, &m_mouse, NULL);
	if (FAILED(result))
	{
		return false;
	}

	// Set the data format for the mouse using the pre-defined mouse data format.
	result = m_mouse->SetDataFormat(&c_dfDIMouse);
	if (FAILED(result))
	{
		return false;
	}

	result = m_mouse->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);	//NONEXCLUSIVE means any application can see the mosue input
	if (FAILED(result))
	{
		return false;
	}

	//Acquires the use of the mouse
	result = m_mouse->Acquire();
	if (FAILED(result))
	{
		return false;
	}

	m_Camera = new CameraClass;
	if (!m_Camera)
	{
		return false;
	}

	return true;
}

void InputClass::Shutdown()
{
	
	if (m_mouse)
	{
		m_mouse->Unacquire();
		m_mouse->Release();
		m_mouse = 0;
	}

	if (m_keyboard)
	{
		m_keyboard->Unacquire();
		m_keyboard->Release();
		m_keyboard = 0;
	}

	if (m_directInput)
	{
		m_directInput->Release();
		m_directInput = 0;
	}

	return;
}

bool InputClass::Frame()
{
	bool result;


	// Read the current state of the keyboard.
	result = ReadKeyboard();
	if (!result)
	{
		return false;
	}

	// Read the current state of the mouse.
	result = ReadMouse();
	if (!result)
	{
		return false;
	}

// Process the changes in the mouse and keyboard.
ProcessInput();

return true;
}

bool InputClass::ReadKeyboard()
{
	HRESULT result;


	// Read the keyboard device.
	result = m_keyboard->GetDeviceState(sizeof(m_keyboardState), (LPVOID)&m_keyboardState);
	if (FAILED(result))
	{
		// If the keyboard lost focus or was not acquired then try to get control back.
		if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
		{
			m_keyboard->Acquire();
		}
		else
		{
			return false;
		}
	}

	return true;
}

bool InputClass::ReadMouse()
{
	HRESULT result;


	// Read the mouse device.
	result = m_mouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&m_mouseState);
	if (FAILED(result))
	{
		// If the mouse lost focus or was not acquired then try to get control back.
		if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
		{
			m_mouse->Acquire();
		}
		else
		{
			return false;
		}
	}

	return true;
}

void InputClass::ProcessInput()
{
	//Update mouse location based on change of the mouse location during the frame
	m_mouseX += m_mouseState.lX;
	m_mouseY += m_mouseState.lY;

	//mouse doesnt go off of the screen
	if (m_mouseX < 0) { m_mouseX = 0; }
	if (m_mouseY < 0) { m_mouseY = 0; }

	if (m_mouseX > m_screenWidth) { m_mouseX = m_screenWidth; }
	if (m_mouseY > m_screenHeight) { m_mouseY = m_screenHeight; }

	return;
}

void InputClass::DetectInput(HWND hwnd)
{

	DIMOUSESTATE mouseCurrentState;

	m_mouse->GetDeviceState(sizeof(DIMOUSESTATE), &mouseCurrentState);


	//Check if escape is pressed
	if (m_keyboardState[DIK_ESCAPE] & 0x80)
	{
		PostMessage(hwnd, WM_QUIT, 0, 0);
	}
	if (m_keyboardState[DIK_W] & 0x80)
	{
		tempGraphics->GetCamera()->SetPosition(tempGraphics->GetCamPos().x, tempGraphics->GetCamPos().y, tempGraphics->GetCamPos().z + 1.0f);
	}
	if (m_keyboardState[DIK_S] & 0x80)
	{
		tempGraphics->GetCamera()->SetPosition(tempGraphics->GetCamPos().x, tempGraphics->GetCamPos().y, tempGraphics->GetCamPos().z - 1.0f);
	}
	if (m_keyboardState[DIK_A] & 0x80)
	{
		tempGraphics->GetCamera()->SetPosition(tempGraphics->GetCamPos().x - 1.0f, tempGraphics->GetCamPos().y, tempGraphics->GetCamPos().z);
	}
	if (m_keyboardState[DIK_D] & 0x80)
	{
		tempGraphics->GetCamera()->SetPosition(tempGraphics->GetCamPos().x + 1.0f, tempGraphics->GetCamPos().y, tempGraphics->GetCamPos().z);
	}
	if (m_keyboardState[DIK_Q] & 0x80)
	{
		tempGraphics->GetCamera()->SetRotation(tempGraphics->GetCamRot().x - 1.0f, tempGraphics->GetCamRot().y , tempGraphics->GetCamRot().z);
		//tempGraphics->GetCamera()->SetPosition(tempGraphics->GetCamPos().x, tempGraphics->GetCamPos().y + 1.0f, tempGraphics->GetCamPos().z);
	}
	if (m_keyboardState[DIK_E] & 0x80)
	{
		tempGraphics->GetCamera()->SetRotation(tempGraphics->GetCamRot().x + 1.0f, tempGraphics->GetCamRot().y , tempGraphics->GetCamRot().z);
		//tempGraphics->GetCamera()->SetPosition(tempGraphics->GetCamPos().x, tempGraphics->GetCamPos().y - 1.0f, tempGraphics->GetCamPos().z);
	}
	if (m_keyboardState[DIK_V] & 0x80)
	{
		//tempGraphics->GetCamera()->SetRotation(tempGraphics->GetCamRot().x - 1.0f, tempGraphics->GetCamRot().y, tempGraphics->GetCamRot().z);
		tempGraphics->GetCamera()->SetPosition(tempGraphics->GetCamPos().x, tempGraphics->GetCamPos().y + 1.0f, tempGraphics->GetCamPos().z);
	}
	if (m_keyboardState[DIK_B] & 0x80)
	{
		//tempGraphics->GetCamera()->SetRotation(tempGraphics->GetCamRot().x + 1.0f, tempGraphics->GetCamRot().y, tempGraphics->GetCamRot().z);
		tempGraphics->GetCamera()->SetPosition(tempGraphics->GetCamPos().x, tempGraphics->GetCamPos().y - 1.0f, tempGraphics->GetCamPos().z);
	}

	if (m_keyboardState[DIK_SPACE] & 0x80)
	{
		tempModel->SetCube();
	}
	


	//if ((mouseCurrentState.lX != m_mouseState.lX) || (mouseCurrentState.lY != m_mouseState.lY))
	//{
	//	tempRotX = tempGraphics->GetCamera()->GetRotX();
	//	tempRotY = tempGraphics->GetCamera()->GetRotY();
	//	//tempGraphics->GetCamera()->SetPitchYawRoll(tempGraphics->GetCamera()->GetPitch() * 0.01f, tempGraphics->GetCamera()->GetYaw() += m_mouseState.lX * 0.01f, tempGraphics->GetCamera()->GetYaw() * 0.01f);
	//	
	//	tempGraphics->GetCamera()->SetRotY(tempRotX += m_mouseState.lY * 0.01f);
	//	tempGraphics->GetCamera()->SetRotX(tempRotY += m_mouseState.lX * 0.01f);

	//	m_mouseState = mouseCurrentState;
	//}
	

	return ;
}

void InputClass::GetMouseLocation(int& mouseX, int& mouseY)	//Returns mosue location 
{
	mouseX = m_mouseX;
	mouseY = m_mouseY;
	return;
}

void InputClass::SetTempGraphics(GraphicsClass * graphic)
{
	tempGraphics = graphic;
}

void InputClass::SetTempModel(ModelClass * model)
{
	tempModel = model;
}


