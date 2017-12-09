#include "stdafx.h"
#include "Player.h"

Player::Player()
{
	color = { 1,0,1 };
	position = { -300,300,300 };
	rotation = { 0.0f,0.0f,0.0f };
	scale = { 1,1,1 };
	size = 10.0f;
}

Player::~Player()
{
}
int BezierRender(vector<Vector3>& vertexList)
{
	glLineWidth(5);

	glBegin(GL_LINES);
	for (int i = 0; i <3 ; i = i + 1)
	{
		Vector3 x1 = vertexList[i];
		Vector3 x2 = vertexList[i + 1];
		Vector3 x3 = vertexList[i + 2];
		Vector3 x4 = vertexList[i + 3];
		for (float detail = 0.0f; detail <= 1.0f; detail += (1.0f / 100.0f))
		{
			Vector3 result = _getBezier(x1, x2, x3, x4, detail);
			glVertex3f(result.x, result.y, result.z);
		}
	}
	glEnd();

	return 0;
}


void Player::Update()
{
	wingDegree += 10;// 프로펠러 돌기

	if(movingCount>0)
	{
		cout << moveIndex << endl;
		Vector3 x1 = movePoint[moveIndex-1];
		Vector3 x2 = movePoint[moveIndex];
		Vector3 x3 = movePoint[moveIndex + 1];
		Vector3 x4 = movePoint[moveIndex + 2];
		Vector3 result = _getBezier(x1, x2, x3, x4, detail);
		//------------------비행기 각도계산--------------------------------
		float dx = result.x - position.x;
		float dy = result.y - position.y;
		float dz = result.z - position.z;
		Vector3 rotateDegree;
		rotateDegree.z = atan(dx / dy)*TO_DEGREE;
		rotateDegree.y = atan(dy / dz)*TO_DEGREE;
		LOG_1(rotateDegree.y);
		LOG_1(rotateDegree.z);
		rotation.y = rotateDegree.y;
		rotation.z = rotateDegree.z;

		position.x = result.x;
		position.y = result.y;
		position.z = result.z;
		detail += (1.0f / 100.0f);
		cout << movePoint.size() - 1 << endl;
		if (detail >= 1.0f&&moveIndex + 3 < movePoint.size())
		{
			moveIndex += 3;
			detail = 0.0f;
		}
		else if (detail >= 1.0f)
		{
			detail = 0.0f;
			movingCount -= 1;
			moveIndex = 1;
		}
	}
}
void Player::Render()
{
	glPushMatrix();
	{
		glTranslatef(position.x, position.y, position.z);
		////glRotatef(rotation.x, 1, 0, 0);
		glRotatef(rotation.y, 0, 1, 0);
		glRotatef(rotation.z, 0, 0, 1);
		glPushMatrix();
		{
			glColor3f(color.x, color.y, color.z);
			glScalef(3, 1, 1);
			glutSolidCube(size);
		}
		glPopMatrix();
		glPushMatrix();
		{
			glColor3f(1, 1, 0);
			glTranslatef(0, 0,  15);
			glScalef(1, 0.5, 2);
			glutSolidCube(size);
		}
		glPopMatrix();
		glPushMatrix();
		{
			glColor3f(1, 1, 0);
			glTranslatef(0,0,  - 15);
			glScalef(1, 0.5, 2);
			glutSolidCube(size);
		}
		glPopMatrix();
		glPushMatrix();
		{
			glColor3f(1, 1, 0);
			glTranslatef( - 10,  + 10, 0);
			glScalef(0.5, 1, 0.5);
			glutSolidCube(size);
		}
		glPopMatrix();
		glPushMatrix();
		{
			glColor3f(1, 1, 1);
			glTranslatef(+ 16, 0, 0);
			glRotatef(wingDegree, 1, 0, 0);
			glRotatef(90, 1, 0, 0);
			glutSolidCone(size - 7, size, 20, 20);
			glRotatef(180, 1, 0, 0);
			glutSolidCone(size - 7, size, 20, 20);
		}
		glPopMatrix();

		glPushMatrix();
		{
			glTranslatef( + 15, 0, 0);
			glRotatef(wingDegree, 1, 0, 0);
			glutSolidCone(size - 7, size, 20, 20);
			glRotatef(180, 1, 0, 0);
			glutSolidCone(size - 7, size, 20, 20);
		}
		glPopMatrix();
	}
	glPopMatrix();


		if (ready)
		{
			glPushMatrix();
			BezierRender(movePoint);
			glPopMatrix();
		}

}