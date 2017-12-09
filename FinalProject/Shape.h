#pragma once
struct TriangleList
{
	Vector3 bottom[4];
	Vector3 left[3];
	Vector3 right[3];
	Vector3 front[3];
	Vector3 back[3];
};




class Shape
{
public:
	Shape();
	Vector3* GetVertex();
	void Render();
	//---------------회전-----------------------
	void RotateZ(int degree);
	void RotateX(int degree);
	void RotateY(int degree);
	//------------Move------------------------
	void Move(Vector3 direction);
	//------------충돌면 반환
	TriangleList* GetCollider();
public:
	Vector3 verticies[5] =
	{
		{ -100.0, -100.0, -100.0 },
		{ -100.0, -100.0, 100.0 },
		{ 100.0, -100.0, 100.0 },
		{ 100.0, -100.0, -100.0 },
		{ 0.0,100.0, 0.0 }
	};
	TriangleList colliderPlane;
};
