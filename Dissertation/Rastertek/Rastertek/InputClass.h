#ifndef _INPUTCLASS_H_
#define _INPUTCLASS_H_

#define DIRECTINPUT_VERSION 0x0800	//Version of direct input used
									//Stops compiler generating messages 

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

#include <dinput.h>

class GraphicsClass;
class CameraClass;
class InputClass
{
public:
	InputClass();
	InputClass(const InputClass&);
	~InputClass();

	bool Initialize(HINSTANCE, HWND, int, int);
	void Shutdown();
	bool Frame();

	void DetectInput(HWND);
	void GetMouseLocation(int&, int&);

	void SetTempGraphics(GraphicsClass*);

private:

	bool ReadKeyboard();
	bool ReadMouse();
	void ProcessInput();

	float tempRotX = 0.0f;
	float tempRotY = 0.0f;

	IDirectInput8* m_directInput;
	IDirectInputDevice8* m_keyboard;
	IDirectInputDevice8* m_mouse;

	unsigned char m_keyboardState[256];
	DIMOUSESTATE m_mouseState;

	int m_screenWidth, m_screenHeight;
	int m_mouseX, m_mouseY;
	CameraClass* m_Camera;
	GraphicsClass* tempGraphics;
};

#endif
