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


using namespace std;
using namespace DirectX;

#define DDS 0x01
#define WIC 0x02
#define TGA 0x03





#define ASSIMP_LOAD_FLAGS (aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices)
#define NUM_BONES_PER_VEREX 4
#define MAX_BONE 100
#define ZERO_MEM(a) memset(a, 0, sizeof(a))
#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))

typedef unsigned int uint;