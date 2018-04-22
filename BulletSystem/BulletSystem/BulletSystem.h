#pragma once
#include"Bullet.h"
#include<vector>
using namespace std;

class BulletSystem
{
public:
	BulletSystem();
	~BulletSystem();
	virtual void Update(float elapsedTime) = 0;
	virtual void SetTarget(float x, float y) {};

	void SetPosition(const float x,const float y,const float z) { m_startPosition.x = x; m_startPosition.y = y; m_startPosition.z = z;};
	
	Point GetPosition() const{ return m_startPosition; };
	
protected:
	Point m_startPosition = { 0,0,0 };
public:
	vector<Bullet*> bullets;
};

class BiDirectionalSystem : public BulletSystem
{
public:
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
	void Update(float elapsedTime);
public:
	float m_angle = { 0 };
	float m_shotAnglerate = { -0.02f };
	float m_angleAcceleration = -0.03f;
	float m_speedAcceleration = 1.f;
	float m_firerate = 0;
	float m_speed = 3;
	float m_angleRange=0.25;
	int multiShotCount = 7;
};

class AimingNWAYSystem :public BulletSystem
{
public:
	void SetTarget(float x, float y);
	void Update(float elapsedTime);
public:
	float m_angle = { 0 };
	float m_shotAnglerate = { -0.02f };
	float m_angleAcceleration = -0.03f;
	float m_speedAcceleration = 1.f;
	float m_firerate = 0;
	float m_speed = 3;
	float m_angleRange = 0.25;
	int multiShotCount = 7;
};