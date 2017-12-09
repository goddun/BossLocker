#pragma once
class InputClass
{
public:
	InputClass();
	~InputClass();
	void handleInput(unsigned char key);
private:
	unsigned char current_key;

};

