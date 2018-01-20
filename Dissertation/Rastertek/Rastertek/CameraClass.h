#ifndef _CAMERACLASS_H_
#define _CAMERACLASS_H_


//////////////
// INCLUDES //
//////////////
#include <directxmath.h>
using namespace DirectX;


////////////////////////////////////////////////////////////////////////////////
// Class name: CameraClass
////////////////////////////////////////////////////////////////////////////////
class CameraClass
{
public:
	CameraClass();
	CameraClass(const CameraClass&);
	~CameraClass();

	void SetPosition(float, float, float);
	void SetRotation(float, float, float);
	void SetPitchYawRoll(double, double, double);
	void SetPitch(double);
	void SetRoll(double);
	void SetYaw(double);

	void SetRotX(float);
	void SetRotY(float);
	void SetRotZ(float);

	XMFLOAT3 GetPosition();
	XMFLOAT3 GetRotation();

	float GetPosX() { return m_positionX; }
	float GetPosY() { return m_positionY; }
	float GetPosZ() { return m_positionZ; }

	float GetRotX() { return m_rotationX; }
	float GetRotY() { return m_rotationY; }
	float GetRotZ() { return m_rotationZ; }

	double GetYaw() { return yaw; }
	double GetPitch() { return pitch; }
	double GetRoll() { return roll; }

	void Render();
	void GetViewMatrix(XMMATRIX&);

	double yaw, pitch, roll;

private:
	float m_positionX, m_positionY, m_positionZ;
	float m_rotationX, m_rotationY, m_rotationZ;
	XMMATRIX m_viewMatrix;
	XMFLOAT3 up, position, lookAt;
	XMVECTOR upVector, positionVector, lookAtVector;
	
	XMMATRIX rotationMatrix;
};

#endif