#pragma once

struct ColorVertex
{
	D3DXVECTOR3 position;
	D3DXVECTOR4 color;
};

struct TextureVertex
{
	D3DXVECTOR3 position;
	D3DXVECTOR2 texture;
	D3DXVECTOR3 normal;
};

