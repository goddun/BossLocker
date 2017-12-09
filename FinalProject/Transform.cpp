#include "stdafx.h"
#include "Transform.h"


Transform::Transform()
{
}


Transform::~Transform()
{
}

void Transform::Translate(const Position& translation)
{
	position + translation;
	glTranslatef(position.x, position.y, position.z);
}

void Transform::Rotate(const Rotation& param)
{
	glRotatef(param.z, 0, 0, 1);
	glRotatef(param.x, 1, 0, 0);
	glRotatef(param.y, 0, 1, 0);
}
