#include "BulletSystem.h"



BulletSystem::BulletSystem()
{
}


BulletSystem::~BulletSystem()
{
	for (auto iter = bullets.begin();iter != bullets.end();)
	{
		delete *iter;
		bullets.erase(iter);
	}
	bullets.clear();
}

//}

void BiDirectionalSystem::Update(float elapsedTime)
{
	m_firerate += elapsedTime;

	if (m_firerate > 0.5)
	{
		for (int j = 0; j < 2; j++)
		{
			for (int i = 0; i < multiShotCount; ++i)
			{
				bullets.emplace_back(new Bullet(m_speed, m_angle[j] + float(i) / multiShotCount));

			}
			m_angle[j] += m_shotAnglerate[j];
			m_angle[j] -= floor(m_angle[j]);
		}
	
		m_firerate = 0;
	}

	for (auto& bullet : bullets)
	{
		bullet->Update(elapsedTime);
	}
}

void BentSystem::Update(float elapsedTime)
{
	m_firerate += elapsedTime;

	if (m_firerate > 0.5)
	{
			for (int i = 0; i < multiShotCount; ++i)
			{
				Bullet* bullet = new Bullet(m_speed, m_angle + (float(i) / multiShotCount), m_speedAcceleration, m_angleAcceleration);
				bullets.push_back(bullet);
			}

		m_firerate = 0;
	}

	for (auto& bullet : bullets)
	{
		bullet->Update(elapsedTime);
	}
}

void NWAYSystem::Update(float elapsedTime)
{
	m_firerate += elapsedTime;

	if (m_firerate > 0.5)
	{
		for (int i = 0; i < multiShotCount; ++i)
		{
			Bullet* bullet = new Bullet(m_speed, m_angle + m_angleRange * (float(i) / (multiShotCount - 1) - 0.5f));
			bullets.push_back(bullet);
		}

		m_firerate = 0;
	}

	for (auto& bullet : bullets)
	{
		bullet->Update(elapsedTime);
	}
}


void AimingNWAYSystem::SetTarget(float x, float y)
{
	m_angle = atan2f(y-m_startPosition.y, x-m_startPosition.x) / 3.1415 / 2;
	cout << m_angle << endl;
}

void AimingNWAYSystem::Update(float elapsedTime)
{
	m_firerate += elapsedTime;

	if (m_firerate > 0.5)
	{
		for (int i = 0; i < multiShotCount; ++i)
		{
			Bullet* bullet = new Bullet(m_speed, m_angle + m_angleRange * (float(i) / (multiShotCount - 1) - 0.5f));
			bullets.push_back(bullet);
		}

		m_firerate = 0;
	}

	for (auto& bullet : bullets)
	{
		bullet->Update(elapsedTime);
	}
}
