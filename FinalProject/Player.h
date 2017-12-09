#pragma once
class Bullet;

class Player
{
public:
	Player();
	~Player();

	void Shoot();
	void RotateZ(int degree);
	void RotateX(int degree);
	void RotateY(int degree);
	void Update();
	void Render();
	//------------변수---------------------------
	Vector3 color;
	Vector3 position;
	Vector3 rotation;
	Vector3 scale;
	Vector3 direction;
	float size;
	float prev_time;
	float elapsedTime;
	//------------총알 리스트--------------
	list<Bullet*> bulletList;
};
