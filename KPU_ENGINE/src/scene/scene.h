////////////////////////////////////////////////////////////////////////////////
// Filename: Scene.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _GRAPHICSCLASS_H_
#define _GRAPHICSCLASS_H_




///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "../graphic/d3dclass.h"
#include "../graphic/cameraclass.h"
#include"../graphic/TextureModel.h"
#include"../graphic/textureshaderclass.h"
#include"../graphic/colorshaderclass.h"
#include"../graphic/lightshaderclass.h"
#include"../graphic/light.h"
#include"../graphic/ColorModel.h"
#include"../graphic/textclass.h"
/////////////
// GLOBALS //
/////////////
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;


////////////////////////////////////////////////////////////////////////////////
// Class name: Scene
////////////////////////////////////////////////////////////////////////////////
class Scene
{
public:
	Scene();
	Scene(const Scene&);
	~Scene();

	bool Initialize(int, int, HWND);
	void Shutdown();
	bool Frame(int,int, float);

private:
	bool Render();

private:
	D3DClass* m_D3D=nullptr;
	CameraClass* m_Camera = nullptr;
	TextureModel* m_Model = nullptr;
	ColorModel* m_ColorModel = nullptr;
	//================Shader Class===============================
	TextureShaderClass* m_TextureShader = nullptr;
	ColorShaderClass* m_ColorShader = nullptr;
	LightShaderClass* m_LightShader = nullptr;
	//===================LIGHT==================================
	Light* m_light = nullptr;
	//=================≈ÿΩ∫∆Æ=====================================
	TextClass* m_Text=nullptr;
};

#endif