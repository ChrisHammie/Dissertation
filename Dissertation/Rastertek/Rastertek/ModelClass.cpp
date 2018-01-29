
#include "ModelClass.h"


ModelClass::ModelClass()
{
	m_vertexBuffer = 0;
	m_indexBuffer = 0;

	m_texture = 0;

	m_model = 0;

	m_Square = 0;
	//vertices = 0;

	indices = 0;

	m_vertexCount = 0;
	m_indexCount = 0;

	prevIndiceCount = 0;
	prevVerticeCount = 0;
	prevModel = 0;

	count = 0;

	m_texture = new TextureClass;
	if (!m_texture)
	{
		return;
	}

	
}


ModelClass::ModelClass(const ModelClass& other)
{
}


ModelClass::~ModelClass()
{
}


bool ModelClass::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* textureFileName, char* modelFilename)
{
	bool result;
	
	
	
	SetVerticeAndIndiceArray();
	

	midpoints.reserve(10);

	// Load in the model data,
	result = LoadModel("square2.txt");
	if (!result)
	{
		return false;
	}

	result = LoadModel("flat2.txt");
	if (!result)
	{
		return false;
	}

	if (cube == true)
	{
		result = LoadModel("Cube.txt");
		if (!result)
		{
			return false;
		}
	}

	
	result = InitializeBuffers(device, deviceContext);
	if (!result)
	{
		return false;
	}
	
		
	
	

	result = LoadTexture(device, deviceContext, textureFileName);
	if (!result)
	{
		return false;
	}

	/*result = LoadTexture(device, deviceContext, textureFileName);
	if (!result)
	{
		return false;
	}*/
	

	return true;
}


void ModelClass::Shutdown()
{
	// Shutdown the vertex and index buffers.
	ShutdownBuffers();
	ReleaseTexture();
	// Release the model data.
	ReleaseModel();
	return;
}


void ModelClass::Render(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
	RenderBuffers(device, deviceContext);

	return;
}


int ModelClass::GetIndexCount()
{
	return m_indexCount;
}

std::vector<ID3D11ShaderResourceView*> ModelClass::GetTexture()
{
	return m_texture->GetTexture();
}

void ModelClass::SetVerticeAndIndiceArray()
{
	ifstream open;
	char input;

	open.open("square2.txt");

	// If it could not open the file then exit.
	if (open.fail())
	{
		return;
	}

	// Read up to the value of vertex count.
	open.get(input);
	while (input != ':')
	{
		open.get(input);
	}

	// Read in the vertex count.
	open >> prevVerticeCount;

	open.close();

	m_vertexCount += prevVerticeCount;
	// prevVerticeCount = m_vertexCount;

	open.open("flat2.txt");

	// If it could not open the file then exit.
	if (open.fail())
	{
		return;
	}

	// Read up to the value of vertex count.
	open.get(input);
	while (input != ':')
	{
		open.get(input);
	}

	// Read in the vertex count.
	open >> prevVerticeCount;
	m_vertexCount += prevVerticeCount;

	open.close();

	if (cube == true)
	{
		open.open("Cube.txt");
		if (open.fail())
		{
			return;
		}

		// Read up to the value of vertex count.
		open.get(input);
		while (input != ':')
		{
			open.get(input);
		}

		// Read in the vertex count.
		open >> prevVerticeCount;

		m_vertexCount += prevVerticeCount;

	}

	//m_vertexCount += prevVerticeCount;
	m_indexCount = m_vertexCount;

	// Create the index array.


	vertices = new VertexType[m_vertexCount];
	if (!vertices)
	{
		return;
	}


	indices = new unsigned long[m_indexCount];
	if (!indices)
	{
		return;
	}
}


bool ModelClass::InitializeBuffers(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	
	m_vertexCount += prevVerticeCount;

					 // Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	

	// Release the arrays now that the vertex and index buffers have been created and loaded.
	/*delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;*/

	return true;
}


void ModelClass::ShutdownBuffers()
{
	// Release the index buffer.
	if (m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}

	// Release the vertex buffer.
	if (m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}

	return;
}


void ModelClass::RenderBuffers(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	unsigned int stride;
	unsigned int offset;

	if (cube == true)
	{
		m_vertexCount = 0;
		D3D11_MAPPED_SUBRESOURCE ms;
		deviceContext->Map(m_vertexBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
		Initialize(device, deviceContext, "texturefile.tga", "");

		memcpy(ms.pData, vertices, sizeof(vertices));

		deviceContext->Unmap(m_vertexBuffer, NULL);

		cube = false;
	}

	// Set vertex buffer stride and offset.
	stride = sizeof(VertexType);
	offset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	

	return;
}

bool ModelClass::LoadTexture(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* fileName)
{
	bool result;

	

	result = m_texture->Initalize(device, deviceContext, fileName);
	if (!result)
	{
		return false;
	}

	return true;
}

void ModelClass::ReleaseTexture()
{
	if (m_texture)
	{
		m_texture->Shutdown();
		delete m_texture;
		m_texture = 0;
	}
	return;
}

bool ModelClass::LoadModel(char* filename)
{
	ifstream fin;
	ifstream grass;
	ifstream dead;
	ifstream watertxt;
	char input;
	char uvCoord;
	//int i;
	int temp = 0;

	// Open the model file.
	fin.open(filename);

	// If it could not open the file then exit.
	if (fin.fail())
	{
		return false;
	}

	

	// Read up to the value of vertex count.
	fin.get(input);
	while (input != ':')
	{
		fin.get(input);
	}

	// Read in the vertex count.
	fin >> m_vertexCount;
	


	
	m_model = new ModelType[m_vertexCount];
	if (!m_model)
	{
		return false;
	}


	// Read up to the beginning of the data.
	fin.get(input);
	while (input != ':')
	{
		fin.get(input);
	}
	fin.get(input);
	fin.get(input);


	// Read in the vertex data.
	for (int i = 0; i< m_vertexCount; i++)
	{
		fin >> m_model[i].x >> m_model[i].y >> m_model[i].z;		//Reads in position and normal values and sets to m_model
		fin >> m_model[i].nx >> m_model[i].ny >> m_model[i].nz;

		if (iCounter == 1)
		{
			one.x = m_model[i].x;								//Stores the second vertex read in of each square1
			one.y = m_model[i].y;
			one.z = m_model[i].z;

		}
		else if (iCounter == 2)									
		{
			two.x = m_model[i].x;						//Stores the third vertex read in of each square1
			two.y = m_model[i].y;
			two.z = m_model[i].z;

			midpoint.x = (one.x + two.x) / 2;			//Calculates the mid point of each square1 using the 2 previous values
			midpoint.y = (one.y + two.y) / 2;
			midpoint.z = (one.z + two.z) / 2;
			if (filename == "square2.txt")
			{
				water = midpoint;
				prevVerticeCount = 0;
			}
			else
			{
				midpoints.push_back(midpoint);
			}
		}
		iCounter++;
		if (iCounter == 6)
		{
			diff.x = std::abs(midpoint.x - water.x);
			diff.y = std::abs(midpoint.y - water.y);
			diff.z = std::abs(midpoint.z - water.z);


			if (filename != "square2.txt")			//Depending on distance from the water, depends on the textue that will be displayed
			{
				if (sqrt(pow(diff.x, 2) + pow(diff.z, 2)) <= sqrt(8)) //accounts for the square diagonally from water
				{
					grass.open("grassUV.txt");

					if (grass.fail())
					{
						return false;
					}
					for (int j = prevModel; j < (prevModel + 6); j++)
					{
						grass >> m_model[j].tu >> m_model[j].tv;
					}
					grass.close();
				}
				else if (sqrt(pow(diff.x, 2) + pow(diff.z, 2)) > sqrt(8))
				{
					dead.open("deadUV.txt");

					if (dead.fail())
					{
						return false;
					}
					for (int j = prevModel; j < (prevModel + 6); j++)
					{
						dead >> m_model[j].tu >> m_model[j].tv;
					}
					dead.close();
				}
			}
			else
			{
				watertxt.open("waterUV.txt");

				if (watertxt.fail())
				{
					return false;
				}
				for (int j = prevModel; j < (prevModel + 6); j++)
				{
					watertxt >> m_model[j].tu >> m_model[j].tv;
				}
				watertxt.close();
				
			}
			iCounter = 0;

			prevModel += 6;
		}
	}
	prevModel = 0;


	// Close the model file.
	fin.close();
	
	
	
	

	/*if (filename == "square1.txt")
	{
		m_indexCount = m_vertexCount;
		for (int i = 0; i<m_vertexCount; i++)
		{
			vertices[i].position = XMFLOAT3(m_model[i].x, m_model[i].y, m_model[i].z);
			vertices[i].texture = XMFLOAT2(m_model[i].tu, m_model[i].tv);
			vertices[i].normal = XMFLOAT3(m_model[i].nx, m_model[i].ny, m_model[i].nz);

			if (iCounter == 1)
			{
				one = vertices[i].position;

			}
			else if (iCounter == 2)
			{
				two = vertices[i].position;

				midpoint.x = (one.x + two.x) / 2;
				midpoint.y = (one.y + two.y) / 2;
				midpoint.z = (one.z + two.z) / 2;

				midpoints.push_back(midpoint);
			}
			iCounter++;
			if (iCounter == 6)
			{
				iCounter = 0;
			}

			indices[i] = i;
		}
		
	}*/

	
	
	//m_indexCount = prevVerticeCount + m_vertexCount;
	//m_vertexCount += prevVerticeCount;
	for (int i = prevVerticeCount, j = 0, k = 0; i<(m_vertexCount + prevVerticeCount); i++, j++, k++)
	{
			if (filename == "square2.txt")
			{
				vertices[i].position = XMFLOAT3(m_model[i].x, m_model[i].y, m_model[i].z);
				vertices[i].texture = XMFLOAT2(m_model[i].tu, m_model[i].tv);
				vertices[i].normal = XMFLOAT3(m_model[i].nx, m_model[i].ny, m_model[i].nz);

				
			}
			else if (filename == "flat2.txt")
			{
				vertices[i].position = XMFLOAT3(m_model[j].x, m_model[j].y, m_model[j].z);
				vertices[i].texture = XMFLOAT2(m_model[j].tu, m_model[j].tv);
				vertices[i].normal = XMFLOAT3(m_model[j].nx, m_model[j].ny, m_model[j].nz);
			}
			else if (filename == "Cube.txt")
			{
				vertices[i].position = XMFLOAT3(m_model[k].x, m_model[k].y, m_model[k].z);
				vertices[i].texture = XMFLOAT2(m_model[k].tu, m_model[k].tv);
				vertices[i].normal = XMFLOAT3(m_model[k].nx, m_model[k].ny, m_model[k].nz);
			}
			indices[i] = i;

	}

	prevVerticeCount += m_vertexCount;
	prevIndiceCount += m_indexCount;

	/*if (count == 0)
	{
		prevVerticeCount = m_vertexCount;
		prevIndiceCount = m_indexCount;
		count++;
	}
	else if (count == 1)
	{
		prevVerticeCount = m_vertexCount;
		prevIndiceCount = m_indexCount;
	}*/


	ReleaseModel();
	return true;
}

void ModelClass::ReleaseModel()
{
	if (m_model)
	{
		delete[] m_model;
		m_model = 0;
	}

	return;
}


