#include "Bullet.h"



Bullet::Bullet()
{
}



Bullet::~Bullet()
{
}

void Bullet::Update(float elapsedTime)
{
	float rad = m_angle*3.141592*2;

	position.x += m_speed * cosf(rad) * elapsedTime;
	position.y += m_speed * sinf(rad) * elapsedTime;

	m_angle += m_angleAccelation * elapsedTime;
	m_speed += m_speedAccelation * elapsedTime;

}


