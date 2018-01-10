#include "Texture.h"



Texture::Texture()
{
}

Texture::Texture(const Texture &other)
{
}


Texture::~Texture()
{
}

bool Texture::Load_DDS(ID3D11Device * device, WCHAR * filename)
{
	HRESULT result; // Load the texture in. 
	result = D3DX11CreateShaderResourceViewFromFile(device, filename, NULL, NULL, &m_texture, NULL); 
	if(FAILED(result))
	{ 
		return false; 
	} 
	return true;
}

void Texture::Shutdown()
{
	if (m_texture)
	{
		m_texture->Release();
		m_texture = nullptr;
	}
	return;
}
