#pragma once
#include <d3d11.h>
#include <d3dx10math.h>
#include"../math\Vertex.h"
#include"../kpu.h"
class TextureModel
{
public:
	TextureModel(std::vector<TextureVertex>*, std::vector<unsigned long>*);
	TextureModel(const TextureModel&);
	~TextureModel();

	bool Initialize(ID3D11Device*);
	void Shutdown();
	void Render(ID3D11DeviceContext*);
	int GetIndexCount();

private:
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

private:
	ID3D11Buffer * m_vertexBuffer, *m_indexBuffer;
	std::vector<TextureVertex>* verticies;
	std::vector<unsigned long>* indicies;
};

