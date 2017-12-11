#pragma once
#include"Pyramid.h"

class Stage
{
public:
	Stage();
	~Stage();
	void SetStage(int stage);
	bool CollisionCheck(Vector3 & compare);
	void Render();

public:
	vector<Pyramid*> stage[8];
	Pyramid stage2[4];
	Pyramid stage3[4];
	Pyramid stage4[4];
	Pyramid stage5[4];
	Pyramid stage6[4];
	Pyramid stage7[4];
	Pyramid stage8[4];

	int activeStageNumber = 1;
	vector<Pyramid*> &activeStage = stage[0];
};

