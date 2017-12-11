#pragma once




class Pyramid
{
public:
	Pyramid();
	void Render();
	//---------------ȸ��-----------------------
	void RotateZ(int degree);
	void RotateX(int degree);
	void RotateY(int degree);
	//------------Move------------------------
	void Move(Vector3 direction);
	//------------���� ��ȯ
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
