////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _GRAPHICSCLASS_H_
#define _GRAPHICSCLASS_H_


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "d3dclass.h"
#include "cameraclass.h"
#include "colorshaderclass.h"
#include"lightshaderclass.h"
#include"ShaderSystem.h"
#include"textureclass.h"
#include"Mesh.h"
#include"StaticMesh.h"
#include"Model.h"
#include"timer.h"




////////////////////////////////////////////////////////////////////////////////
// Class name: GraphicsClass
////////////////////////////////////////////////////////////////////////////////
class GraphicsClass
{
public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass&);
	~GraphicsClass();

	bool Initialize(int, int, HWND);
	bool SetModel();
	void Shutdown();
	bool Frame();

private:
	bool Render();

private:
	D3DClass* m_Direct3D=nullptr;
	CameraClass* m_Camera=nullptr;
	ShaderSystem* m_shaderSystem=nullptr;
	Timer* m_timer=nullptr;

	vector<Model*> object;
	TextureClass* testTexture=nullptr;
};

#endif