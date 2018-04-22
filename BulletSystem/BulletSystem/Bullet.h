#pragma once
#include<iostream>
struct Point
{
	float x;
	float y;
	float z;
};


class Bullet
{
public:
	Bullet();
	Bullet(float speed, float angle, float speedAccel=0, float angleAccel=0) :m_speed(speed), m_angle(angle),m_speedAccelation(speedAccel), m_angleAccelation(angleAccel) {}
	~Bullet();
	void Update(float elapsedTime);
	inline Point GetPosition()const { return position; }
private:
	Point position = {0,0,0};
	float m_speed = 1;
	float m_angle = 0;
	float m_angleAccelation = 0;
	float m_speedAccelation = 0;
};

