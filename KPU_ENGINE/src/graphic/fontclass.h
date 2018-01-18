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
	struct FontType //글꼴 인덱스 파일의 인덱스 데이터를 저장하기 위해 사용
	{
		float left, right;//텍스처의 uv좌표에 해당
		int size;//문자 픽셀 너비
	};

	struct VertexType//문자가 실제로 그려질 사각형을 만드는데 필요한 정점 데이터를 저장
	{
		D3DXVECTOR3 position;//공간상의 위치
	    D3DXVECTOR2 texture;//텍스처 위치
	};

public:
	FontClass();
	FontClass(const FontClass&);
	~FontClass();

	bool Initialize(ID3D11Device*, char*, WCHAR*, const int);
	void Shutdown();

	ID3D11ShaderResourceView* GetTexture();

	void BuildVertexArray(void*, char*, float, float);//문자열을 입력으로 받아 글자를 그릴 삼각형들의 정점 배열을 만들어 반환

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