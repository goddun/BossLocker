#pragma once
#include"../05_GAME/shoot/Bullet.h"

#include<vector>
#include<random>


using namespace std;

static char pattern[] =
"                        "
"                        "
"########   #   #########"
"#      #   #   #        "
"#      #   #   #        "
"########   #   #   #####"
"#      #   #   #       #"
"#      #   #   #       #"
"########   #   #########";

class BulletSystem
{
public:
	BulletSystem(Model* mesh, Layer3D* layer);
	~BulletSystem();
	virtual void Update(float elapsedTime) = 0;
	virtual void SetTarget(float x, float y) {}

	void SetPosition(const float x,const float y,const float z) { m_startPosition.x = x; m_startPosition.y = y; m_startPosition.z = z;}
	
	Point GetPosition() const{ return m_startPosition; }
protected:
	Point m_startPosition = { 0,0,0 };
	Model* m_mesh=nullptr;
	Layer3D* m_layer=nullptr;
public:
	vector<Bullet*> bullets;
};




class AmingSystem : public BulletSystem
{
	using BulletSystem::BulletSystem;
	virtual void SetTarget(float x, float y) { m_angle = atan2f(y -m_startPosition.y, x - m_startPosition.x) / 3.1415 / 2; };
protected:
	float m_angle = { 0 };
};

class BiDirectionalSystem : public BulletSystem
{
public:
	using BulletSystem::BulletSystem;
	void Update(float elapsedTime);
public:
	float m_angle[2] = {0,0};
	float m_shotAnglerate[2] = {0.03f,-0.02f};
	float m_firerate = 0;
	float m_speed = 8;
	int multiShotCount = 4;
};


class BentSystem : public BulletSystem
{
public:
	using BulletSystem::BulletSystem;
	void Update(float elapsedTime);
public:
	float m_angle = {0 };
	float m_shotAnglerate = { -0.02f };
	float m_angleAcceleration=-0.03f;
	float m_speedAcceleration=1.f;
	float m_firerate = 0;
	float m_speed = 3;
	int multiShotCount = 4;
};

class NWAYSystem :public BulletSystem
{
public:
	using BulletSystem::BulletSystem;
	void Update(float elapsedTime);
public:
	float m_angle = { 0 };
	float m_shotAnglerate = { -0.02f };
	float m_angleAcceleration = -0.03f;
	float m_speedAcceleration = 1.f;
	float m_firerate = 0;
	float m_speed = 3;
	float m_angleRange=1;
	int multiShotCount = 10;
};

class AimingNWAYSystem :public AmingSystem
{
public:
	using AmingSystem::AmingSystem;
	void Update(float elapsedTime);
public:
	float m_shotAnglerate = { -0.02f };
	float m_angleAcceleration = -0.03f;
	float m_speedAcceleration = 1.f;
	float m_firerate = 0;
	float m_speed = 3;
	float m_angleRange = 0.25;
	float m_totalTime = 0;
	int multiShotCount = 7;
};

class RandomNWAYSystem :public AmingSystem
{
public:
	using AmingSystem::AmingSystem;
	void Update(float elapsedTime);
public:
	float m_shotAnglerate = { -0.02f };
	float m_angleAcceleration = -0.03f;
	float m_speedAcceleration = 1.f;
	float m_firerate = 0;
	float m_speed = 3;
	float m_angleRange = 0.25;
	float m_coolTime = 0;
	float m_totalTime = 0;
	int multiShotCount = 5;

};



class PatternNWAYSystem :public AmingSystem
{
public:
	using AmingSystem::AmingSystem;
	void Update(float elapsedTime);
public:
	float m_shotAnglerate = { -0.02f };
	float m_angleAcceleration = -0.03f;
	float m_speedAcceleration = 1.f;
	float m_firerate = 0;
	float m_speed = 3;
	float m_angleRange = 0.25;
	int multiShotCount = 5;

	int width = 24;
	int height = 9;
	float m_totalTime = 0;
};