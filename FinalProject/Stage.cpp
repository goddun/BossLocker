#include "stdafx.h"
#include "Stage.h"

bool Stage::CollisionCheck( Vector3 & compare)
{
	/*for (auto poly : activeStage)
	{
		auto dist = sqrt(SQUARE(poly->position.x - compare.x) + SQUARE(poly->position.y - compare.y) + SQUARE(poly->position.z - compare.z));
		if (dist < DISTANCE)
			return true;
	}
	return false;*/
	for (auto poly : activeStage)
	{
		if (poly->minPoint.x > compare.x || poly->maxPoint.x < compare.x)
			continue;
		else if (poly->minPoint.y > compare.y || poly->maxPoint.y < compare.y)
			continue;
		else if (poly->minPoint.z > compare.z || poly->maxPoint.z < compare.z)
			continue;
		else
			return true;
	}
	return false;
}

Stage::Stage()
{
	//--------------스테이지 세팅-----------------
	Pyramid* temp[4];
	temp[0] = new Pyramid;
	temp[1] = new Pyramid;
	temp[2] = new Pyramid;
	temp[3]= new Pyramid;
	temp[0]->RotateZ(0);
	temp[0]->Move({ 0,200,0 });
	temp[1]->RotateZ(90);
	temp[1]->Move({ -200,0,0 });
	temp[2]->RotateZ(180);
	temp[2]->Move({ 0,0 - 200,0 });
	temp[3]->RotateZ(270);
	temp[3]->Move({ 200,0,0 });
	for (int i = 0; i < 4; i++)
	{
		temp[i]->SetCollider();
		stage[0].push_back(temp[i]);
	}
	//---------------------2--------------------------
	
}


Stage::~Stage()
{
}

void Stage::SetStage(int stage)
{
	activeStageNumber = stage;
}


void Stage::Render()
{
	for (auto poly : stage[activeStageNumber - 1])
	{
		poly->Render();
		//LOG_3("POINT",poly->position.x ,poly->position.y ,poly->position.z );
	}
}


