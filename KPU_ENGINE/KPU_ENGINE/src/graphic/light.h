#pragma once
#include <d3dx10math.h>

class Light
{
public:
	Light();
	~Light();
	void SetDiffuseColor(float c1, float c2, float c3, float c4)
	{
		m_diffuseColor[0] = c1;
		m_diffuseColor[1] = c2;
		m_diffuseColor[2] = c3;
		m_diffuseColor[4] = c4;
	}
	void SetDirection(float d1, float d2, float d3)
	{
		m_direction[0] = d1;
		m_direction[1] = d2;
		m_direction[2] = d3;
	}
	D3DXVECTOR4 GetDiffuseColor() { return m_diffuseColor; }
	D3DXVECTOR3 GetDirection() { return m_direction; }

private:
	D3DXVECTOR4 m_diffuseColor;
	D3DXVECTOR3 m_direction;
};

