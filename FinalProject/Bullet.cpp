#include "stdafx.h"
#include "Bullet.h"


Bullet::Bullet(Vector3 position)
{
	srand((unsigned)time(NULL));
	color = { rand() / (float)RAND_MAX,rand() / (float)RAND_MAX ,rand() / (float)RAND_MAX };
	this->position = position;
}


Bullet::~Bullet()
{
}

void Bullet::Update()
{
	float distance = sqrt(pow(position.x, 2) + pow(position.y, 2)+pow(position.z,2));
	if (distance > 10)
	{
		position.x -= (position.x / distance)*10 ;
		position.y -= (position.y / distance)*10 ;
		position.z -= (position.z / distance)*10 ;
	}
}

void Bullet::Render()
{
	glPushMatrix();
	glColor3f(color.x, color.y, color.z);
	glTranslatef(position.x, position.y, position.z);
	glutSolidCube(10);
	glPopMatrix();
}

Vector3 Bullet::GetCollider()
{
	return position;
}
