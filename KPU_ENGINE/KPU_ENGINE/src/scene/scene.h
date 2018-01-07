////////////////////////////////////////////////////////////////////////////////
// Filename: Scene.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _GRAPHICSCLASS_H_
#define _GRAPHICSCLASS_H_



#include"../kpu.h"
///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "../graphic/d3dclass.h"
#include "../graphic/cameraclass.h"
#include "../graphic/modelclass.h"
#include "../graphic/colorshaderclass.h"
#include"../graphic/Texture.h"
#include"../graphic/TextureModel.h"
#include"../graphic/textureshaderclass.h"
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
	bool Frame();

private:
	bool Render();

private:
	D3DClass* m_D3D=nullptr;
	CameraClass* m_Camera = nullptr;
	//ModelClass* m_Model;
	TextureModel* m_Model = nullptr;
	ColorShaderClass* m_ColorShader = nullptr;
	TextureShaderClass* m_TextureShader = nullptr;



	std::vector<ModelClass*>v_model;
	std::vector<Texture*>v_texture;
	vector<TextureVertex> verticies;
	//vector<ColorVertex> verticies;
	vector<unsigned long> indicies;

};

#endif