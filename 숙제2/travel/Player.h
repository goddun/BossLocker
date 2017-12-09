#pragma once
#include"Utility.h"
class Player
{
public:
	Player();
	~Player();

	void Update();
	void Render();
	//------------º¯¼ö---------------------------
	Vector3 color;
	Vector3 position;
	Vector3 rotation;
	Vector3 scale;
	Vector3 direction;
	float size;
	float prev_time;
	float elapsedTime;
	float wingDegree = 0;
	//----------------------MOVE---------------------
	int moveIndex=1;
	float detail=0.0f;
	vector<Vector3> movePoint;
	int movingCount = 0;
	bool ready = false;
};
