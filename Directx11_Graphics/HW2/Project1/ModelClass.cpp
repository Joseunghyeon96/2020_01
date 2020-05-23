#include "ModelClass.h"

ModelClass::ModelClass()
{
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
	m_model = 0;
	
	m_faceVertexCount = 0;
	m_vertexCount = 0;
	m_textureCount = 0;
	m_normalCount = 0;
	m_faceCount = 0;
}
ModelClass::ModelClass(const ModelClass& other)
{
}
ModelClass::~ModelClass()
{
}
bool ModelClass::Initialize(ID3D11Device* device,char* modelFilename,  WCHAR* textureFilename)
{
	bool result;
	// Initialize the vertex and index buffer that hold the geometry for the triangle.

	// Set the number of vertices in the vertex array.
	// Set the number of indices in the index array.
	result = ReadFileCounts(modelFilename);
	if (!result)
	{
		return false;
	}
	result = LoadDataStructures(modelFilename);
	if(!result)
	{
		return false;
	}
	result = InitializeBuffers(device);
	if (!result)
	{
		return false;
	}
	// Load the texture for this model.
	result = LoadTexture(device, textureFilename);
	if (!result)
	{
		return false;
	}

	return true;
}
ID3D11ShaderResourceView * ModelClass::GetTexture()
{
	return m_Texture->GetTexture();
}
void ModelClass::Shutdown()
{
	ReleaseTexture();
	// Release the vertex and index buffers.
	ShutdownBuffers();
	// Release the model data.
	ReleaseModel();
	return;
}

void ModelClass::ReleaseModel()
{
	if (m_model)
	{
		delete[] m_model;
		m_model = 0;
	}
}
void ModelClass::Render(ID3D11DeviceContext* deviceContext)
{
	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
	RenderBuffers(deviceContext);
	return;
}
int ModelClass::GetIndexCount()
{
	return m_indexCount;
}

bool ModelClass::InitializeBuffers(ID3D11Device* device)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	
	// Create the vertex array.
	vertices = new VertexType[m_indexCount];
	if (!vertices)
	{
		return false;
	}
	// Create the index array.
	indices = new unsigned long[m_indexCount];
	if (!indices)
	{
		return false;
	}

	// Load the vertex array and index array with data.
	for (int i = 0; i < m_indexCount; i++)
	{
		vertices[i].position = D3DXVECTOR3(m_model[i].vx, m_model[i].vy, m_model[i].vz);
		vertices[i].texture = D3DXVECTOR2(m_model[i].tu, m_model[i].tv);
		vertices[i].normal = D3DXVECTOR3(m_model[i].nx, m_model[i].ny, m_model[i].nz);
		indices[i] = i;
	}

	// Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_indexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
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
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;
	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;
	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);

	D3DXMatrixIdentity(&transformMatrix);
	if (FAILED(result))
	{
		return false;
	}

	// Release the arrays now that the vertex and index buffers have been created and loaded.
	delete[] vertices;
	vertices = 0;
	delete[] indices;
	indices = 0;


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

void ModelClass::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride;
	unsigned int offset;
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

bool ModelClass::LoadTexture(ID3D11Device * device, WCHAR *filename)
{
	bool result;
	// Create the texture object.
	m_Texture = new TextureClass;
	if (!m_Texture)
	{
		return false;
	}
	// Initialize the texture object.
	result = m_Texture->Initialize(device, filename);
	if (!result)
	{
		return false;
	}
	return true;
}
void ModelClass::ReleaseTexture()
{
	// Release the texture object.
	if (m_Texture)
	{
		m_Texture->Shutdown();
		delete m_Texture;
		m_Texture = 0;
	}
	return;
}

bool ModelClass::LoadDataStructures(char * filename)
{

	D3DXVECTOR3* tempVertices, *tempTexcoords, *tempNormals;
	FaceType *faces;
	ifstream fin;
	int vertexIndex, texcoordIndex, normalIndex, faceIndex, vIndex, tIndex, nIndex;
	char input, input2;

	// Initialize the four data structures.
	tempVertices = new D3DXVECTOR3[m_vertexCount];
	if (!tempVertices)
	{
		return false;
	}
	tempTexcoords = new D3DXVECTOR3[m_textureCount];
	if (!tempTexcoords)
	{
		return false;
	}
	tempNormals = new D3DXVECTOR3[m_normalCount];
	if (!tempNormals)
	{
		return false;
	}
	faces = new FaceType[m_faceCount];
	if (!faces)
	{
		return false;
	}
	for (int i = 0; i < m_faceCount; i++)
	{
		faces[i].vIdx = new int[m_faceVertexCount];
		faces[i].tIdx = new int[m_faceVertexCount];
		faces[i].nIdx = new int[m_faceVertexCount];
	}


	// Initialize the indexes.
	vertexIndex = 0;
	texcoordIndex = 0;
	normalIndex = 0;
	faceIndex = 0;

	// Open the file.
	fin.open(filename);
	// Check if it was successful in opening the file.
	if (fin.fail() == true)
	{
		return false;
	}
	// Read in the vertices, texture coordinates, and tempNormals into the data structures.
	// Important: Also convert to left hand coordinate system since Maya uses right hand coordinate system.
	fin.get(input);
	while (!fin.eof())
	{
		if (input == 'v')
		{
			fin.get(input);
			// Read in the vertices.
			if (input == ' ')
			{
				fin >> tempVertices[vertexIndex].x >> tempVertices[vertexIndex].y >>
					tempVertices[vertexIndex].z;
				// Invert the Z vertex to change to left hand system.
				tempVertices[vertexIndex].z = tempVertices[vertexIndex].z * -1.0f;
				vertexIndex++;
			}
			// Read in the texture uv coordinates.
			if (input == 't')
			{
				fin >> tempTexcoords[texcoordIndex].x >> tempTexcoords[texcoordIndex].y;
				// Invert the V texture coordinates to left hand system.
				tempTexcoords[texcoordIndex].y = 1.0f - tempTexcoords[texcoordIndex].y;
				texcoordIndex++;
			}
			// Read in the tempNormals.
			if (input == 'n')
			{
				fin >> tempNormals[normalIndex].x >> tempNormals[normalIndex].y >>
					tempNormals[normalIndex].z;
				// Invert the Z normal to change to left hand system.
				tempNormals[normalIndex].z = tempNormals[normalIndex].z * -1.0f;
				normalIndex++;
			}
		}
		// Read in the faces.
		if (input == 'f')
		{
			fin.get(input);
			if (input == ' ')
			{
				// Read the face data in backwards to convert it to a left hand system from right hand system.
				for (int i = m_faceVertexCount - 1; i> -1; i--)
				{
					fin >> faces[faceIndex].vIdx[i] >> input2 >> faces[faceIndex].tIdx[i] >> input2 >> faces[faceIndex].nIdx[i];
				}
				faceIndex++;
			}
		}
		// Read in the remainder of the line.
		while (input != '\n')
		{
			fin.get(input);
		}
		// Start reading the beginning of the next line.
		fin.get(input);
	}
	// Close the file.
	fin.close();
	//===============================================================================



	m_model = new ModelType[m_indexCount];

	int mIdx=0;
	for (int i = 0; i < m_faceCount; i++)
	{
		// 삼각형으로 잘라진 obj
		if (m_faceVertexCount == 3) {
			for (int t = 0; t < 3; t++)
			{
				m_model[mIdx++] = {
			tempVertices[faces[i].vIdx[t] - 1].x,
			tempVertices[faces[i].vIdx[t] - 1].y,
			tempVertices[faces[i].vIdx[t] - 1].z,

			tempTexcoords[faces[i].tIdx[t] - 1].x,
			tempTexcoords[faces[i].tIdx[t] - 1].y,

			tempNormals[faces[i].nIdx[t] - 1].x,
			tempNormals[faces[i].nIdx[t] - 1].y,
			tempNormals[faces[i].nIdx[t] - 1].z };
			}
		}

		//사각형으로 잘라진 obj
		else if (m_faceVertexCount == 4)
		{
			for (int t = 0; t < 6; t++)
			{
				if (t < 3)
				{
					m_model[mIdx++] = {
						tempVertices[faces[i].vIdx[t] - 1].x,
						tempVertices[faces[i].vIdx[t] - 1].y,
						tempVertices[faces[i].vIdx[t] - 1].z,

						tempTexcoords[faces[i].tIdx[t] - 1].x,
						tempTexcoords[faces[i].tIdx[t] - 1].y,

						tempNormals[faces[i].nIdx[t] - 1].x,
						tempNormals[faces[i].nIdx[t] - 1].y,
						tempNormals[faces[i].nIdx[t] - 1].z };
				}
				else if (t == 3 || t == 4)
				{
					m_model[mIdx++] = {
					tempVertices[faces[i].vIdx[t-1] - 1].x,
					tempVertices[faces[i].vIdx[t-1] - 1].y,
					tempVertices[faces[i].vIdx[t-1] - 1].z,

					tempTexcoords[faces[i].tIdx[t-1] - 1].x,
					tempTexcoords[faces[i].tIdx[t-1] - 1].y,

					tempNormals[faces[i].nIdx[t-1] - 1].x,
					tempNormals[faces[i].nIdx[t-1] - 1].y,
					tempNormals[faces[i].nIdx[t-1] - 1].z };
				}
				else
				{
					m_model[mIdx++] = {
					tempVertices[faces[i].vIdx[0] - 1].x,
					tempVertices[faces[i].vIdx[0] - 1].y,
					tempVertices[faces[i].vIdx[0] - 1].z,

					tempTexcoords[faces[i].tIdx[0] - 1].x,
					tempTexcoords[faces[i].tIdx[0] - 1].y,

					tempNormals[faces[i].nIdx[0] - 1].x,
					tempNormals[faces[i].nIdx[0] - 1].y,
					tempNormals[faces[i].nIdx[0] - 1].z };
				}
			}

		}
	}
	//================================================================================
	if (tempVertices)
	{
		delete[] tempVertices;
		tempVertices = 0;
	}
	if (tempTexcoords)
	{
		delete[] tempTexcoords;
		tempTexcoords = 0;
	}
	if (tempNormals)
	{
		delete[] tempNormals;
		tempNormals = 0;
	}
	for (int i = 0; i < m_faceCount; i++)
	{
		delete[] faces[i].vIdx;
		delete[] faces[i].tIdx;
		delete[] faces[i].nIdx;

		faces[i].vIdx=0;
		faces[i].tIdx=0;
		faces[i].nIdx=0;

	}
	if (faces)
	{
		delete[] faces;
		faces = 0;
	}
	return true;
}

bool ModelClass::ReadFileCounts(char* filename)
{
		ifstream fin;
		char input;
		// Open the file.
		fin.open(filename);
		// Check if it was successful in opening the file.
		if (fin.fail() == true)
		{
			return false;
		}
		// Read from the file and continue to read until the end of the file is reached.
		fin.get(input);
		while (!fin.eof())
		{
			// If the line starts with 'v' then count either the vertex, the texture coordinates, or the normal vector.
			if (input == 'v')
			{
				fin.get(input);
				if (input == ' ') { m_vertexCount++; }
				if (input == 't') { m_textureCount++; }
				if (input == 'n') { m_normalCount++; }
			}
			// If the line starts with 'f' then increment the face count.
			if (input == 'f')
			{
				fin.get(input);
				if (input == ' ') { 
					m_faceCount++;
					m_faceVertexCount++;
				}
				while (input != '\n')
				{
					fin.get(input);
					if (input == ' ') {
						fin.get(input);
						if(input!='\n')
						m_faceVertexCount++;
					}

				}
			}
			// Otherwise read in the remainder of the line.
			while (input != '\n')
			{
				fin.get(input);

			}
			// Start reading the beginning of the next line.
			fin.get(input);
		}

		m_faceVertexCount = m_faceVertexCount / m_faceCount;
		m_indexCount = m_faceCount* (m_faceVertexCount - 2) * 3;
		// Close the file.
		fin.close();
		return true;
}

void ModelClass::GetTransformMatrix(D3DXMATRIX & TransformMatrix)
{
	{
		TransformMatrix = transformMatrix;
		return;
	}
}



