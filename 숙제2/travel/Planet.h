#pragma once



class Planet
{
public:
	Planet();
	void Render();
	//---------------È¸Àü-----------------------
	void RotateZ(int degree);
	void RotateX(int degree);
	void RotateY(int degree);
	//------------Move------------------------
	void Update();
	void SetTree();
	void SetMark();
public:
	Vector3 position;
	Vector3 color;
	Vector3 markColor = {1,0,0};
	Vector3 rotation = {0,0,0};
	int degree = 0;
	Vector3 treeRotation[5];
	int radius;
	bool isTree = true;
};


class Star
{
public:
	Star();
	void Render();
	Vector3 position;
	Vector3 color = {1,1,1};
	Vector3 colorDiffuse;
	int isIncrease = -1;
};
