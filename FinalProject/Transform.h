#pragma once


class Transform
{
	
public:
	Transform();
	~Transform();

	void Translate(const Position&  translation);
	void Rotate(const Rotation& param);

	Position position;
	Rotation rotation;
	Color color;
};

