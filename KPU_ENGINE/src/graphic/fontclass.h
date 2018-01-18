////////////////////////////////////////////////////////////////////////////////
// Filename: fontclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _FONTCLASS_H_
#define _FONTCLASS_H_


//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <d3dx10math.h>
#include <fstream>
using namespace std;


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "texture.h"


////////////////////////////////////////////////////////////////////////////////
// Class name: FontClass
////////////////////////////////////////////////////////////////////////////////
class FontClass
{
private:
	struct FontType //�۲� �ε��� ������ �ε��� �����͸� �����ϱ� ���� ���
	{
		float left, right;//�ؽ�ó�� uv��ǥ�� �ش�
		int size;//���� �ȼ� �ʺ�
	};

	struct VertexType//���ڰ� ������ �׷��� �簢���� ����µ� �ʿ��� ���� �����͸� ����
	{
		D3DXVECTOR3 position;//�������� ��ġ
	    D3DXVECTOR2 texture;//�ؽ�ó ��ġ
	};

public:
	FontClass();
	FontClass(const FontClass&);
	~FontClass();

	bool Initialize(ID3D11Device*, char*, WCHAR*, const int);
	void Shutdown();

	ID3D11ShaderResourceView* GetTexture();

	void BuildVertexArray(void*, char*, float, float);//���ڿ��� �Է����� �޾� ���ڸ� �׸� �ﰢ������ ���� �迭�� ����� ��ȯ

private:
	bool LoadFontData(char*);
	void ReleaseFontData();
	bool LoadTexture(ID3D11Device*, WCHAR*,const int);
	void ReleaseTexture();

private:
	FontType* m_Font;
	Texture* m_Texture;
};

#endif