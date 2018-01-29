#ifndef _GRAPHICSCLASS_H_
#define _GRAPHICSCLASS_H_



//#include <windows.h>
#include "D3DClass.h"
#include "CameraClass.h"
#include "ModelClass.h"
#include "TextureShaderClass.h"
#include "LightClass.h"
#include "LightShaderClass.h"
#include "AntTweakBar.h"

const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;



class GraphicsClass
{
public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass&);
	~GraphicsClass();

	bool Initialize(int, int, HWND);
	void Shutdown();
	bool Frame(int, int);
	bool Render(float);

	CameraClass* GetCamera() { return m_Camera; }

	ModelClass* GetModel() { return m_Model1; }

	XMFLOAT3 GetCamPos() { return m_Camera->GetPosition(); }
	XMFLOAT3 GetCamRot() { return m_Camera->GetRotation(); }
	//void SetCamPos() {	m_Camera->SetPosition() }

private:
	
	float test = 0.0f;
	float camPos = 0.0f;
	bool rot = false;
	D3DClass* m_Direct3D;
	CameraClass* m_Camera;
	ModelClass* m_Model1;
	//ModelClass* m_Model2;
	TextureShaderClass* m_TextureShader;

	LightShaderClass* m_LightShader1;
	LightShaderClass* m_LightShader2;
	LightClass* m_Light;
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
	TwBar* tweakBar;

};

#endif
