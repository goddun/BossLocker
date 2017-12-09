#pragma once
class Bullet
{
public:
	Vector3 position;
public:
	Bullet(Vector3 position);
	~Bullet();
	void Update();
	void Render();
	Vector3 GetCollider();
private:
	Vector3 color;
};

