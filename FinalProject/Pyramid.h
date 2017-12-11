#pragma once




class Pyramid
{
public:
	Pyramid();
	void Render();
	//---------------회전-----------------------
	void RotateZ(int degree);
	void RotateX(int degree);
	void RotateY(int degree);
	//------------Move------------------------
	void Move(Vector3 direction);
	//------------중점 반환
	void SetCollider();
public:
	Vector3 verticies[5] =
	{
		{ -100.0, -100.0, -100.0 },
		{ -100.0, -100.0, 100.0 },
		{ 100.0, -100.0, 100.0 },
		{ 100.0, -100.0, -100.0 },
		{ 0.0,100.0, 0.0 }
	};
	Vector3 position = {0,0,0};
	Vector3 rotation = {0,0,0};
	Vector3 maxPoint;
	Vector3 minPoint;
};
