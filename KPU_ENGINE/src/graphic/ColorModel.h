#pragma once
#include <d3d11.h>
#include <d3dx10math.h>
#include"math\Vertex.h"
#include"fbxsdk.h"
class ColorModel
{
public:
	ColorModel();
	~ColorModel();
	bool Initialize(ID3D11Device*, const char*, const int);

	


	void Shutdown();
	void Render(ID3D11DeviceContext*);
	int GetIndexCount();
	int GetVertexCount() { return verticies.size(); }

private:
	void GetFbxInfo(FbxNode* Node);
	bool LoadFBX(const char*);
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

private:
	//====================FBX============================================
	FbxManager * m_fbxManager = nullptr;
	const char * mFileName;
	//mutable Status mStatus;
	mutable FbxString mWindowMessage;

	FbxManager * mSdkManager = nullptr;
	FbxScene * mScene = nullptr;
	FbxImporter * mImporter = nullptr;
	FbxAnimLayer * mCurrentAnimLayer = nullptr;
	FbxNode * mSelectedNode = nullptr;

	int mPoseIndex=-1;
	FbxArray<FbxString*> mAnimStackNameArray;
	FbxArray<FbxNode*> mCameraArray;
	FbxArray<FbxPose*> mPoseArray;

	mutable FbxTime mFrameTime, mStart, mStop, mCurrentTime;
	mutable FbxTime mCache_Start, mCache_Stop;

	// Data for camera manipulation
	mutable int mLastX, mLastY;
	mutable FbxVector4 mCamPosition, mCamCenter;
	mutable double mRoll;
	//mutable CameraStatus mCameraStatus;

	bool mPause;
	//ShadingMode mShadingMode;
	bool mSupportVBO;

	//===============================================================
	ID3D11Buffer * m_vertexBuffer = nullptr, *m_indexBuffer = nullptr;
	std::vector<ColorVertex> verticies;
	std::vector<unsigned long> indicies;


};

