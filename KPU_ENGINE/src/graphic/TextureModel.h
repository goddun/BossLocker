#pragma once
#include <d3d11.h>
#include <d3dx10math.h>
#include"../graphic/Texture.h"
#include"../math\Vertex.h"
#include"fbxsdk.h"
class TextureModel
{
public:
	TextureModel();
	TextureModel(const TextureModel&);
	~TextureModel();

	bool Initialize(ID3D11Device*, const char*, const int);

	bool LoadTexture(ID3D11Device *device , WCHAR*, const int);

	void Shutdown();
	void Render(ID3D11DeviceContext*);
	int GetIndexCount();
	ID3D11ShaderResourceView * GetTexture()
	{
		
		return m_texture.GetTexture();
	}
	int GetVertexCount()
	{
		return verticies.size();
	}

private:
	bool LoadFBX(const char*);
	void TextureModel::GetFbxInfo(FbxNode* Node);
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

private:
	ID3D11Buffer * m_vertexBuffer=nullptr, *m_indexBuffer=nullptr;
	std::vector<TextureVertex> verticies;
	std::vector<unsigned long> indicies;
	/////////////TEXTURE///////////////
	Texture m_texture;
};

