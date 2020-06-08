#pragma once

#include <d3d11.h>
#include <d3dx10math.h> 
#include <fstream>
#include "textureclass.h"


using namespace std;

class ModelClass
{
private:
	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
		D3DXVECTOR3 normal;
	};
	struct ModelType
	{
		float vx, vy, vz;
		float tu, tv;
		float nx, ny, nz;
	};
	struct FaceType
	{
		int* vIdx;
		int* tIdx;
		int* nIdx;
	};
public:
	ModelClass();
	ModelClass(const ModelClass&);
	~ModelClass();

	bool Initialize(ID3D11Device*, char*, WCHAR*);
	ID3D11ShaderResourceView* GetTexture();
	void Shutdown();
	void ReleaseModel();

	void Render(ID3D11DeviceContext*);
	int GetIndexCount();

	void GetTransformMatrix(D3DXMATRIX& TransformMatrix);
private:
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

	bool LoadTexture(ID3D11Device*, WCHAR*);
	void ReleaseTexture();

	bool LoadDataStructures(char*);
	bool ReadFileCounts(char*);



private:
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	TextureClass * m_Texture;
	ModelType* m_model;
	D3DXMATRIX transformMatrix;

	int m_faceVertexCount;
	int m_vertexCount, m_indexCount,m_textureCount,m_normalCount,m_faceCount;
};