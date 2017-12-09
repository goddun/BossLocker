#include "stdafx.h"
#include "Planet.h"

Planet::Planet()
{
	color = { rand() / (float)RAND_MAX,rand() / (float)RAND_MAX ,rand() / (float)RAND_MAX };
	position = { float(rand() % 600 - 300),float(rand() % 600 - 300),float(rand() % 600 - 300) };
	radius = rand() % 20 + 30;
	degree = rand() % 10 + 1;
	for (int i = 0; i < 5; i++)
		treeRotation[i] = { rand() % 160 + 20.0f,rand() % 160 + 20.0f,rand() % 160 + 20.0f };
}


void Planet::Render()
{
	glPushMatrix();
	glTranslatef(position.x, position.y, position.z);
	if (isTree)
	{
		RotateZ(degree);
		SetTree();
	}
	else
		SetMark();
	glColor3f(color.x, color.y, color.z);
	glutSolidSphere(radius,20,20);
	glPopMatrix();
}



void Planet::RotateZ(int degree)
{
	rotation.z += degree;
	glRotatef(rotation.z, 0, 0, 1);
}

void Planet::RotateY(int degree)
{
	rotation.y += degree;
	glRotatef(rotation.y, 0, 1, 0);
}

void Planet::RotateX(int degree)
{
	rotation.x += degree;
	glRotatef(rotation.x, 1, 0, 0);
}
	
void Planet::Update()
{
	
}

void Planet::SetTree()
{
	glPushMatrix();
	glTranslatef(0, 60, 0);
	glColor3f(0, 1, 0);
	glRotatef(270, 1, 0, 0);
	glutSolidCone(10, 20, 20, 20);
	glColor3f(0.7, 0.3, 0);
	glTranslatef(0, 0, -15);
	glScalef(1, 1, 3);
	glutSolidCube(10);
	glPopMatrix();

	for (int i = 0; i < 5; i++)
	{
		glRotatef(treeRotation[i].z, 0, 0, 1);
		glRotatef(treeRotation[i].x, 1, 0, 0);
		glRotatef(treeRotation[i].y, 0, 1 ,0);
		glPushMatrix();
		glTranslatef(0, 60, 0);
		glColor3f(0, 1, 0);
		glRotatef(270, 1, 0, 0);
		glutSolidCone(10, 20, 20, 20);
		glColor3f(0.7, 0.3, 0);
		glTranslatef(0, 0, -15);
		glScalef(1, 1, 3);
		glutSolidCube(10);
		glPopMatrix();
	}
	
}

void Planet::SetMark()
{
	glColor3f(markColor.x, markColor.y, markColor.z);
	glPushMatrix();
	glTranslatef(0, 80, 0);
	glRotatef(90, 1, 0, 0);
	glutSolidCone(15,30,20,20);
	glPopMatrix();
}

Star::Star()
{
	position = { float(rand() % 1600) - 800,float(rand() % 1600) - 800 ,-400 };
	colorDiffuse = { rand()/float(RAND_MAX),rand() / float(RAND_MAX) ,-400 };
}

void Star::Render()
{
	glPushMatrix();
	glTranslatef(position.x, position.y, position.z);
	glColor3f(color.x, color.y, color.z);
	glutSolidCube(5);
	glPopMatrix();
	color.x += isIncrease*colorDiffuse.x;
	color.y += isIncrease*colorDiffuse.y;
	color.z += isIncrease*colorDiffuse.z;
	if (color.x <= 0)
		isIncrease *= -1;
	if (color.x >= 1)
		isIncrease *= -1;
}
