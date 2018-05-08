#include "Bullet.h"



Bullet::Bullet()
{
}

Bullet::Bullet(Model * mesh, float speed, float angle, float speedAccel, float angleAccel)
{
	m_mesh = mesh;
	m_entity = new Entity(m_mesh->GetMesh(), RGBA(1.0, 0, 0, 1.0));
	m_speed = speed;
	m_angle = angle;
	m_speedAccelation = speedAccel;
	m_angleAccelation = angleAccel;

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

	m_entity->GetTransformComponent()->Translate({ position.x,position.y,0 });
}


