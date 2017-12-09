#pragma once
#include<ctime>
#include"Object.h"
class SceneManager
{
private:
	float previousTime;
public:
	Camera m_camera;
public:
	~SceneManager();

	void Update();
};

