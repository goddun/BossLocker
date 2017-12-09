#include "stdafx.h"
#include "Shape.h"


Shape::Shape()
{
}

Vector3* Shape::GetVertex()
{
	return verticies;
}

void Shape::Render()
{
	srand((unsigned)time(NULL));
	//color = { rand() / (float)RAND_MAX,rand() / (float)RAND_MAX ,rand() / (float)RAND_MAX };
	glPushMatrix();
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

void Shape::Move(Vector3 direction)
{
	for (int i = 0; i < 5; i++)
	{
		verticies[i].x += direction.x;
		verticies[i].y += direction.y;
		verticies[i].z += direction.z;
	}
}

TriangleList* Shape::GetCollider()
{
	for (int i = 0; i < 4; i++)
	{
		colliderPlane.bottom[i] = verticies[i];
	}
	for (int i = 0; i <= 1; i++)
	{
		colliderPlane.left[i] = verticies[i];
		colliderPlane.front[i] = verticies[i + 1];
		colliderPlane.right[i] = verticies[i + 2];
		colliderPlane.back[i] = verticies[(i + 3) % 4];
	}
	colliderPlane.left[2] = verticies[4];
	colliderPlane.right[2] = verticies[4];
	colliderPlane.front[2] = verticies[4];
	colliderPlane.back[2] = verticies[4];
	return &colliderPlane;
}


void Shape::RotateZ(int degree)
{
	float radian = degree * ANGLE;
	for (int i = 0; i < 5; i++)
	{
		Vector3 temp;
		temp.x = (cos(radian)*verticies[i].x) - (sin(radian)*verticies[i].y);
		temp.y = (sin(radian)*verticies[i].x) + (cos(radian)*verticies[i].y);
		temp.z = verticies[i].z;
		verticies[i].Copy(temp);
	}
}

void Shape::RotateY(int degree)
{
	float radian = degree * ANGLE;
	for (int i = 0; i < 5; i++)
	{
		Vector3 temp;
		temp.x = (cos(radian)*verticies[i].x) - (sin(radian)*verticies[i].z);
		temp.z = (sin(radian)*verticies[i].x) + (cos(radian)*verticies[i].z);
		temp.y = verticies[i].y;
		verticies[i].Copy(temp);
	}
}

void Shape::RotateX(int degree)
{
	float radian = degree * ANGLE;
	for (int i = 0; i < 5; i++)
	{
		Vector3 temp;
		temp.z = (cos(radian)*verticies[i].y) + (sin(radian)*verticies[i].z);
		temp.y = -(sin(radian)*verticies[i].y) + (cos(radian)*verticies[i].z);
		temp.x = verticies[i].x;
		verticies[i].Copy(temp);
	}
}
