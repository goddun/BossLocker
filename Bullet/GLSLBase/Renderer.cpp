#include "stdafx.h"
#include "Renderer.h"
#include<chrono>


Renderer::Renderer(int windowSizeX, int windowSizeY)
{
	Initialize(windowSizeX, windowSizeY);
}

Renderer::~Renderer()
{
}

void Renderer::Initialize(int windowSizeX, int windowSizeY)
{
	//Set window size
	m_WindowSizeX = windowSizeX;
	m_WindowSizeY = windowSizeY;

	//Load shaders
	m_SolidRectShader = CompileShaders("./Shaders/SolidRect.vs", "./Shaders/SolidRect.ps");
	//Create VBOs
	CreateBufferObjects();

	if (m_SolidRectShader > 0)
	{
		m_Initialized = true;
	}
/*
	Bullet* bullet = new Bullet{ 0.01f,0.75f, 0.0f,0.0f };
	Bullet* bullet2 = new Bullet{ 0.005f,0.35f, 0.0f,0.0f };
	bullets.push_back(bullet);
	bullets.push_back(bullet2);*/


}

bool Renderer::IsInitialized()
{
	return m_Initialized;
}




void Renderer::CreateBufferObjects()
{
	


	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
}

void Renderer::ProcessInput(float x, float y)
{
	m_mouseX = (x - 250)/250.0f;
	m_mouseY = (-y+250)/250.0f;
}

void Renderer::AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType)
{
	//���̴� ������Ʈ ����
	GLuint ShaderObj = glCreateShader(ShaderType);

	if (ShaderObj == 0) {
		fprintf(stderr, "Error creating shader type %d\n", ShaderType);
	}

	const GLchar* p[1];
	p[0] = pShaderText;
	GLint Lengths[1];
	Lengths[0] = strlen(pShaderText);
	//���̴� �ڵ带 ���̴� ������Ʈ�� �Ҵ�
	glShaderSource(ShaderObj, 1, p, Lengths);

	//�Ҵ�� ���̴� �ڵ带 ������
	glCompileShader(ShaderObj);

	GLint success;
	// ShaderObj �� ���������� ������ �Ǿ����� Ȯ��
	glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLchar InfoLog[1024];

		//OpenGL �� shader log �����͸� ������
		glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
		fprintf(stderr, "Error compiling shader type %d: '%s'\n", ShaderType, InfoLog);
		printf("%s \n", pShaderText);
	}

	// ShaderProgram �� attach!!
	glAttachShader(ShaderProgram, ShaderObj);
	
}

bool Renderer::ReadFile(char* filename, std::string *target)
{
	std::ifstream file(filename);
	if (file.fail())
	{
		std::cout << filename << " file loading failed.. \n";
		file.close();
		return false;
	}
	std::string line;
	while (getline(file, line)) {
		target->append(line.c_str());
		target->append("\n");
	}
	return true;
}

GLuint Renderer::CompileShaders(char* filenameVS, char* filenameFS)
{
	GLuint ShaderProgram = glCreateProgram(); //�� ���̴� ���α׷� ����

	if (ShaderProgram == 0) { //���̴� ���α׷��� ����������� Ȯ��
		fprintf(stderr, "Error creating shader program\n");
	}

	std::string vs, fs;

	//shader.vs �� vs ������ �ε���
	if (!ReadFile(filenameVS, &vs)) {
		printf("Error compiling vertex shader\n");
		return -1;
	};

	//shader.fs �� fs ������ �ε���
	if (!ReadFile(filenameFS, &fs)) {
		printf("Error compiling fragment shader\n");
		return -1;
	};

	// ShaderProgram �� vs.c_str() ���ؽ� ���̴��� �������� ����� attach��
	AddShader(ShaderProgram, vs.c_str(), GL_VERTEX_SHADER);

	// ShaderProgram �� fs.c_str() �����׸�Ʈ ���̴��� �������� ����� attach��
	AddShader(ShaderProgram, fs.c_str(), GL_FRAGMENT_SHADER);

	GLint Success = 0;
	GLchar ErrorLog[1024] = { 0 };

	//Attach �Ϸ�� shaderProgram �� ��ŷ��
	glLinkProgram(ShaderProgram);

	//��ũ�� �����ߴ��� Ȯ��
	glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &Success);

	if (Success == 0) {
		// shader program �α׸� �޾ƿ�
		glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		std::cout << filenameVS << ", " << filenameFS << " Error linking shader program\n" << ErrorLog;
		return -1;
	}

	glValidateProgram(ShaderProgram);
	glGetProgramiv(ShaderProgram, GL_VALIDATE_STATUS, &Success);
	if (!Success) {
		glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		std::cout << filenameVS << ", " << filenameFS << " Error validating shader program\n" << ErrorLog;
		return -1;
	}

	glUseProgram(ShaderProgram);
	std::cout << filenameVS << ", " << filenameFS << " Shader compiling is done.\n";

	return ShaderProgram;
}


float fireRate = 0;


void BasicSpiral(float time, vector<Bullet*>& bullets)
{

	static float shotAngle = 0.25;
	static float shotAngleRate = 0.02;

	fireRate += time;

	if (fireRate > 0.5)
	{
		Bullet* newBullet = new Bullet(0.1, shotAngle, 0, 0);
		bullets.push_back(newBullet);
		fireRate = 0;
		shotAngle += shotAngleRate;
		shotAngle -= floor(shotAngle);
	}
}

void MultiSpiral(float time, vector<Bullet*>& bullets)
{
	static float shotAngle = 0.25;
	static float shotAngleRate = 0.05;
	fireRate += time;
	static int count = 10;
	if (fireRate>0.5 )
	//if(count!=0)
	{	
		count--;
		for (int i = 0; i < 4; i++)
		{
			Bullet* newBullet = new Bullet(0.3, shotAngle+(float)i/4, 0, 0);
			bullets.push_back(newBullet);	
		}
		shotAngle += shotAngleRate;
		shotAngle -= floor(shotAngle);
		fireRate = 0;
	}
}

void DualDirectionMultiSpiral(float time, vector<Bullet*>& bullets)
{
	static float shotAngle[2] = { 0.25,0.25 };
	static float shotAngleRate[2] = { 0.15,-0.1 };
	fireRate += time;
	static int count = 10;
	if (fireRate>0.5)
		//if(count!=0)
	{
		count--;
		for (int j = 0; j < 2; j++)
		{
			for (int i = 0; i < 4; i++)
			{
				Bullet* newBullet = new Bullet(0.3, shotAngle[j] + (float)i / 4, 0, 0);
				bullets.push_back(newBullet);
			}
			shotAngle[j] += shotAngleRate[j];
			shotAngle[j] -= floor(shotAngle[j]);
		}
		fireRate = 0;
	}
}

void LinearMultiSpiral(float time, vector<Bullet*>& bullets)
{
	static float shotAngle = 0;
	static float shotAngleRate = 0.02;
	static float speed = 0.3;
	static float bullet_angleRate = -0.00003;
	static float bullet_speedRate = 0;
	static float interval = 1;

	fireRate += time;
	static int count = 10;
	if (fireRate>interval)
		//if(count!=0)
	{
		count--;
		Bullet* newBullet = new Bullet(speed, shotAngle, bullet_speedRate, bullet_angleRate, {1,0,0});
			bullets.push_back(newBullet);

			shotAngle += shotAngleRate;
			shotAngle -= floor(shotAngle);

		fireRate = 0;
	}
}

void easyWasherSprial(float time, vector<Bullet*>& bullets)
{
	static float shotAngle =  0.25;
	static float speed = 0.3;
	static int totalframe = 0;
	static int count = 10;

	float maxShotAngleRate = 0.02f;
	float maxBulletAngleRate = 0.00003f;

	float shotAngleRate = 0.02f;
	float bullet_angleRate = -0.00003;
	float bullet_speedRate = 0;
	float interval = 0.5;

	if (totalframe < 250)
	{
		shotAngleRate = maxShotAngleRate;
		bullet_angleRate = maxBulletAngleRate;
	}
	else if (totalframe < 300)
	{
		shotAngleRate = maxShotAngleRate * (275 - totalframe) / 25;
		bullet_angleRate = -maxBulletAngleRate * (275 - totalframe) / 25;
	}
	else if (totalframe < 550)
	{
		shotAngleRate = -maxShotAngleRate;
		bullet_angleRate = maxBulletAngleRate;
	}
	else
	{
		shotAngleRate = -maxShotAngleRate * (575 - totalframe) / 25;
		bullet_angleRate = maxBulletAngleRate * (575 - totalframe) / 25;
	}


	totalframe = (totalframe +1)%600;
	fireRate += time;
	//cout << totalframe << " " << shotAngleRate << endl;
	if (fireRate>interval)
		//if(count!=0)
	{
		count--;
		for (int i = 0; i < 4; i++)
		{
			Bullet* newBullet = new Bullet(speed, shotAngle + (float)i / 4, 0, 0);
			bullets.push_back(newBullet);

		}
		shotAngle += shotAngleRate;
		shotAngle -= floor(shotAngle);
		
		Bullet* newBullet = new Bullet(speed, 0, 0, bullet_angleRate, { 1,0,0 });
		bullets.push_back(newBullet);


		fireRate = 0;
	}
}

void Renderer::FragmentSpline(float time)
{
	glUseProgram(m_SolidRectShader);

	//BasicSpiral(time,bullets);
	// MultiSpiral(time, bullets);
	//DualDirectionMultiSpiral(time, bullets);
	//LinearMultiSpiral(time, bullets);
	easyWasherSprial(time, bullets);


	for (auto iter = bullets.begin();iter!=bullets.end();)
	{
		auto bullet = *iter;
		bullet->Move(time);

		int attribPosition = glGetAttribLocation(m_SolidRectShader, "a_Position");

		glEnableVertexAttribArray(attribPosition);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, 0, 0);

		Transform worldPosition = bullet->position;
		GLuint id = glGetUniformLocation(m_SolidRectShader, "worldPosition");
		glUniform3f(id, worldPosition.x, worldPosition.y, worldPosition.z);
		Transform color = bullet->color;
		id = glGetUniformLocation(m_SolidRectShader, "u_color");
		glUniform3f(id, color.x, color.y, color.z);

		glPointSize(10);
		glDrawArrays(GL_POINTS, 0, 4);

		glDisableVertexAttribArray(attribPosition);
		if (bullet->outRange())
		{
			iter = bullets.erase(iter);
			delete bullet;
			bullet = nullptr;
		}
		else
		{
			iter++;
		}
	}

}
