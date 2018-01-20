#ifndef _MODELCLASS_H_
#define _MODELCLASS_H_



#include <d3d11.h>
#include <directxmath.h>
using namespace DirectX;

#include "TextureClass.h"
#include <fstream>
#include <vector>
using namespace std;




class ModelClass
{
private:
	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;
		XMFLOAT3 normal;
		
	};

	struct ModelType
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
	};

public:
	ModelClass();
	ModelClass(const ModelClass&);
	~ModelClass();

	bool Initialize(ID3D11Device*, ID3D11DeviceContext*, char*, char*);
	void Shutdown();
	void Render(ID3D11DeviceContext*);

	int GetIndexCount();
	
	ID3D11ShaderResourceView* GetTexture();

	//float length = 0.0f;

	XMFLOAT3 midpoint;
	XMFLOAT3 one;
	XMFLOAT3 two;
	int iCounter = 0;

private:
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);
	
	bool LoadTexture(ID3D11Device*, ID3D11DeviceContext*, char*);
	void ReleaseTexture();

	bool LoadModel(char*);
	void ReleaseModel();

private:
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;

	int prevVerticeCount;
	int prevIndiceCount;

	TextureClass* m_texture;

	ModelType* m_model;
	ModelType* m_Square;

	VertexType* vertices;
	unsigned long* indices;

	std::vector<XMFLOAT3> midpoints;

};

#endif