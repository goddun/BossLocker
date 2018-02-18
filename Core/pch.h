#pragma once

#pragma comment(linker,"/entry:WinMainCRTStartup /subsystem:console")

//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <directxmath.h>
#include<memory>
#include<cstdio>
#include<iostream>
#include <map>
#include <vector>
#include <d3dcompiler.h>


#include <sstream>
#include <dwrite.h>
#include <dinput.h>
///////////////**************new**************////////////////////
#include <vector>
#include <fstream>
#include <istream>
#include<DirectXTex.h>

using namespace std;
using namespace DirectX;

#define DDS 0x01
#define WIC 0x02
#define TGA 0x03

////////////////////
// WINDOW SETTING //
////////////////////
#define FULL_SCREEN  false
#define VSYNC_ENABLED  true
#define SCREEN_DEPTH     1000.0f
#define SCREEN_NEAR		 0.1f

#define COLOR_SHADER 0x01
#define ANIMATION_SHADER 0x02



#define ASSIMP_LOAD_FLAGS (aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices)
#define NUM_BONES_PER_VEREX 4
#define MAX_BONE 100
#define ZERO_MEM(a) memset(a, 0, sizeof(a))
#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))
#define PI 3.14159
#define DEGREE_TO_RADIAN PI/180.0f

typedef unsigned int uint;

struct Vertex	//Overloaded Vertex Structure
{
	Vertex() {}
	Vertex(float x, float y, float z,
		float u, float v,
		float nx, float ny, float nz)
		: pos(x, y, z), texCoord(u, v), normal(nx, ny, nz) {}

	XMFLOAT3 pos;
	XMFLOAT2 texCoord;
	XMFLOAT3 normal;
};



