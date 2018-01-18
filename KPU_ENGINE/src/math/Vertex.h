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

struct Vertex
{
	D3DXVECTOR3 position;
	D3DXVECTOR3 normal;
	D3DXVECTOR2 texture;
	D3DXVECTOR4 color;
};

