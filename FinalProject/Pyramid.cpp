#include "stdafx.h"
#include "Pyramid.h"


Pyramid::Pyramid()
{
}


void Pyramid::Render()
{
	srand((unsigned)time(NULL));
	glPushMatrix();
	glTranslatef(position.x, position.y, position.z);
	glRotatef(rotation.x, 1, 0, 0);
	glRotatef(rotation.y, 0, 1, 0);
	glRotatef(rotation.z, 0, 0, 1);
	glBegin(GL_POLYGON);
	//-----关-----
	glNormal3f(0, 1, 0);
	glColor3f(1, 0, 0);
	for (int i = 0; i < 4; i++)
	{
		glVertex3f(verticies[i].x, verticies[i].y, verticies[i].z);
	}
	glEnd();
	glBegin(GL_TRIANGLES);
	//----哭率------
	glColor3f(0, 1, 0.3);
	glNormal3f(0, 1, 0);
	for (int i = 0; i < 2; i++)
	{
		glVertex3f(verticies[i].x, verticies[i].y, verticies[i].z);
	}
	glVertex3f(verticies[4].x, verticies[4].y, verticies[4].z);
	//----坷弗率------
	glColor3f(0, 0.5, 1);
	glNormal3f(0, 1, 0);
	for (int i = 0; i < 2; i++)
	{
		glVertex3f(verticies[i+2].x, verticies[i+2].y, verticies[i+2].z);
	}
	glVertex3f(verticies[4].x, verticies[4].y, verticies[4].z);
	////---缔率------
	glColor3f(1, 0, 1);
	glNormal3f(0, 1, 0);
	for (int i = 0; i < 2; i++)
	{
		glVertex3f(verticies[(i + 3) % 4].x, verticies[(i + 3) % 4].y, verticies[(i + 3) % 4].z);
	}
	glVertex3f(verticies[4].x, verticies[4].y, verticies[4].z);
	////------------菊率------------------
	glColor3f(1, 0.5, 0.3);
	glNormal3f(0, 1, 0);
	for (int i = 0; i < 2; i++)
	{
		glVertex3f(verticies[i+1].x, verticies[i+1].y, verticies[i+1].z);
	}
	glVertex3f(verticies[4].x, verticies[4].y, verticies[4].z);
	glEnd();
	glPopMatrix();
}

void Pyramid::Move(Vector3 direction)
{
	position = position + direction;
}


void Pyramid::SetCollider()
{
	minPoint.x = position.x - 110;
	minPoint.y = position.y - 110;
	minPoint.z = position.z - 110;
	maxPoint.x = position.x + 110;
	maxPoint.y = position.y + 110;
	maxPoint.z = position.z + 110;
}



void Pyramid::RotateZ(int degree)
{
	rotation.z += degree;
}

void Pyramid::RotateY(int degree)
{
	rotation.y += degree;
}

void Pyramid::RotateX(int degree)
{
	rotation.x += degree;
}
