#include "stdafx.h"
#include "Player.h"
#include "Bullet.h"

Player::Player()
{
	color = { 1,0,0 };
	position = { 0,600,0 };
	rotation = { 0,0,0 };
	scale = { 1,1,1 };
	size = 30.0f;
}

Player::~Player()
{
}

void Player::Shoot()
{
	Bullet* newBullet = new Bullet({ position.x,position.y,position.z });
	bulletList.push_back(newBullet);
	//LOG_3("position", position.x, position.z, position.z);
}

void Player::RotateZ(int degree)
{
	float radian = degree * ANGLE;
	Vector3 temp;
	temp.x = (cos(radian)*position.x) - (sin(radian)*position.y);
	temp.y = (sin(radian)*position.x) + (cos(radian)*position.y);
	temp.z = position.z;
	position.Copy(temp);
	//LOG_3("position", position.x, position.y, position.z);
}
void Player::RotateY(int degree)
{
	float radian = degree * ANGLE;
	Vector3 temp;
	temp.x = (cos(radian)*position.x) - (sin(radian)*position.z);
	temp.z = (sin(radian)*position.x) + (cos(radian)*position.z);
	temp.y = position.y;
	position.Copy(temp);
	//LOG_3("position", position.x, position.y, position.z);
}
void Player::Update()
{
	for (auto bullet : bulletList)
	{
		bullet->Update();
	}
	RotateZ(5);
}
void Player::Render()
{
	for (auto bullet : bulletList)
	{
		bullet->Render();
	}
	glPushMatrix();
	glColor3f(color.x, color.y, color.z);
	glTranslatef(position.x, position.y, position.z);
	glutSolidCube(size);
	glPopMatrix();
}
void Player::RotateX(int degree)
{
	float radian = degree * ANGLE;
	Vector3 temp;
	temp.z = (cos(radian)*position.y) + (sin(radian)*position.z);
	temp.y = -(sin(radian)*position.y) + (cos(radian)*position.z);
	temp.x = position.x;
	position.Copy(temp);
	//LOG_3("position", position.x, position.y, position.z);
}