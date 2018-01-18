///////////////////////////////////////////////////////////////////////////////
// Filename: fontclass.cpp
///////////////////////////////////////////////////////////////////////////////
#include"kpu.h"
#include "fontclass.h"


FontClass::FontClass()
{
	m_Font = 0;
	m_Texture = 0;
}


FontClass::FontClass(const FontClass& other)
{
}


FontClass::~FontClass()
{
}


bool FontClass::Initialize(ID3D11Device* device, char* fontFilename, WCHAR* textureFilename,const int textureType)
{
	bool result;


	// 폰트 데이터가 들어있는 메모장을 읽는다.
	result = LoadFontData(fontFilename);
	if(!result)
	{
		return false;
	}

	// 폰트 텍스처를 읽는다.
	if(textureType == DDS)
		result = LoadTexture(device, textureFilename,DDS);
	if(!result)
	{
		return false;
	}

	return true;
}


void FontClass::Shutdown()
{
	// Release the font texture.
	ReleaseTexture();

	// Release the font data.
	ReleaseFontData();

	return;
}


bool FontClass::LoadFontData(char* filename)
{
	ifstream fin;
	int i;
	char temp;


	// 폰트 타입형 배열을 만듭니다.
	//배열의 크기는 텍스처에 있는 글자 수 그리고 fontData.txt 파일의 인덱스 개수인 95로 합니다.
	m_Font = new FontType[95];
	if(!m_Font)
	{
		return false;
	}

	// 텍스처의 u좌표인  left와 right 그리고 문자의 픽셀 너비를 읽어 들입니다.
	fin.open(filename);
	if(fin.fail())
	{
		return false;
	}

	// Read in the 95 used ascii characters for text.
	for(i=0; i<95; i++)
	{
		fin.get(temp);
		while(temp != ' ')
		{
			fin.get(temp);
		}
		fin.get(temp);
		while(temp != ' ')
		{
			fin.get(temp);
		}

		fin >> m_Font[i].left;
		fin >> m_Font[i].right;
		fin >> m_Font[i].size;
	}

	// Close the file.
	fin.close();

	return true;
}


void FontClass::ReleaseFontData()
{
	// Release the font data array.
	if(m_Font)
	{
		delete [] m_Font;
		m_Font = 0;
	}

	return;
}


bool FontClass::LoadTexture(ID3D11Device* device, WCHAR* filename, const int type)
{
	bool result;


	// Create the texture object.
	m_Texture = new Texture;
	if(!m_Texture)
	{
		return false;
	}
	if (type == DDS)
	{
		//파일 타입이 DDS면 DDS를 로드 합니다.
		result = m_Texture->Load_DDS(device, filename);
	}
	if(!result)
	{
		return false;
	}

	return true;
}


void FontClass::ReleaseTexture()
{
	// Release the texture object.
	if(m_Texture)
	{
		m_Texture->Shutdown();
		delete m_Texture;
		m_Texture = 0;
	}

	return;
}


ID3D11ShaderResourceView* FontClass::GetTexture()
{
	//글꼴이 그려질수 있게 글꼴 텍스쳐의 인터페이스를 반환합니다.
	return m_Texture->GetTexture();
}

//텍스트 클래스의 의해 호충되어 인자로 받은 sentence로 정점버퍼를 만들어 냅니다.
//verticies는 만들어진 정점 배열을 반환합니다.
//drawX, Y 는 그려질 위치 입니다.
void FontClass::BuildVertexArray(_Out_ void* vertices, char* sentence, float drawX, float drawY)
{
	VertexType* vertexPtr;
	int numLetters, index, i, letter;


	// Coerce the input vertices into a VertexType structure.
	vertexPtr = (VertexType*)vertices;

	// Get the number of letters in the sentence.
	numLetters = (int)strlen(sentence);

	// Initialize the index to the vertex array.
	index = 0;

	// 문자 한개당 두개의 삼각형을 만듭니다.
	for(i=0; i<numLetters; i++)
	{
		letter = ((int)sentence[i]) - 32;

		// If the letter is a space then just move over three pixels.
		if(letter == 0)
		{
			drawX = drawX + 3.0f;
		}
		//현재 문자의 아스키 코드를 인데스로 사용하여
		//m_Font의 텍스처 uv좌표와 사이즈를 따옵니다.
		else
		{
			//첫번째 사각형
			vertexPtr[index].position = D3DXVECTOR3(drawX, drawY, 0.0f);  // Top left.
			vertexPtr[index].texture = D3DXVECTOR2(m_Font[letter].left, 0.0f);
			index++;

			vertexPtr[index].position = D3DXVECTOR3((drawX + m_Font[letter].size), (drawY - 16), 0.0f);  // Bottom right.
			vertexPtr[index].texture = D3DXVECTOR2(m_Font[letter].right, 1.0f);
			index++;

			vertexPtr[index].position = D3DXVECTOR3(drawX, (drawY - 16), 0.0f);  // Bottom left.
			vertexPtr[index].texture = D3DXVECTOR2(m_Font[letter].left, 1.0f);
			index++;

			// Second triangle in quad.
			vertexPtr[index].position = D3DXVECTOR3(drawX, drawY, 0.0f);  // Top left.
			vertexPtr[index].texture = D3DXVECTOR2(m_Font[letter].left, 0.0f);
			index++;

			vertexPtr[index].position = D3DXVECTOR3(drawX + m_Font[letter].size, drawY, 0.0f);  // Top right.
			vertexPtr[index].texture = D3DXVECTOR2(m_Font[letter].right, 0.0f);
			index++;

			vertexPtr[index].position = D3DXVECTOR3((drawX + m_Font[letter].size), (drawY - 16), 0.0f);  // Bottom right.
			vertexPtr[index].texture = D3DXVECTOR2(m_Font[letter].right, 1.0f);
			index++;

			// Update the x location for drawing by the size of the letter and one pixel.
			drawX = drawX + m_Font[letter].size + 1.0f;
		}
	}

	return;
}