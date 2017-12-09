// 실습25.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//


#include "stdafx.h"
#include"Shape.h"
#include"Player.h"
#include"Bullet.h"





GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
void TimerFunction(int value);
void Mouse(int button, int state, int x, int y);
void Keyboard(unsigned char key, int x, int y);
void KeyboardSpecialKey(int key, int x, int y);
void MenuFunc(int button);
void Motion(int x, int y);
//---------custom function
void Render(int shape);
void Initialize();
void WorldRotation();
void Initialize();
float PlaneCollision(Vector3* verticies, Vector3& point, bool isFirst = true);

Vector3 g_worldRotation = { 0,0,0 };//전체 죄표계 회전
Vector3 g_moving = { -400,0,0 };
bool culling = false, depth = false, shading = false, upAnimation = false, frontAnimation = false;
int g_mousePoint[2][2];
bool g_isClick = false;
bool g_shooting = false;




Player player;
//1 : 4, 2: 
Shape stage1[4];//stage는 기본도형인 shape로 구성된다.


void Initialize()
{
	
	// 메뉴를 만들고 GLUT 에 processMenuEvents 함수가
	// 메뉴이벤트를 다룰 것이라고 알려줍니다.
	glutCreateMenu(MenuFunc);

	// 메뉴에 항목을 추가합니다.
	glutAddMenuEntry("depth", 1);
	glutAddMenuEntry("culling", 2);
	glutAddMenuEntry("shading", 3);
	glutAddMenuEntry("frontAnimation", 4);
	glutAddMenuEntry("upAnimation", 5);

	// 마우스의 오른쪽버튼에 메뉴를 연결합니다.
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	
	//--------------스테이지 세팅-----------------
	stage1[0].RotateZ(0);
	stage1[0].Move({ 0,200,0 });
	stage1[1].RotateZ(90);
	stage1[1].Move({ -200,0,0 });
	stage1[2].RotateZ(180);
	stage1[2].Move({ 0,0 - 200,0 });
	stage1[3].RotateZ(270);
	stage1[3].Move({ 200,0,0 });
	//---------------------2--------------------------

}

void MainGame()
{
	glColor3f(1, 1, 1);
	glPushMatrix();
	glTranslatef(300, 300, 100);
	glutSolidCube(100);
	glPopMatrix();


	player.Update();
	player.Render();

	for (auto data : stage1)
	{
		data.Render();
		auto collider = data.GetCollider();
		LOG_3("bottom ", collider->bottom[0].x, collider->bottom[0].y, collider->bottom[0].z);
		for (auto bullet : player.bulletList)
		{
				if (abs(PlaneCollision(collider->bottom, bullet->position)) < 10.0f)
				{
					cout << "collide Bottom" << endl;
				}
		}

	}
	//auto playerPosition = player.position;
	//LOG_3("left2 ", collider->left[1].x, collider->left[1].y, collider->left[1].z);
	//LOG_3("left2 ", collider->left[2].x, collider->left[2].y, collider->left[2].z);
	
	//LOG_3("playerPosition", player.position.x, player.position.y, player.position.z);
	//player.position = { -50,200.1f,0 };

	

}

float PlaneCollision(Vector3* verticies, Vector3& point, bool isFirst)
{
	Vector3 side1 = verticies[1] - verticies[0];
	Vector3 side2 = verticies[2] - verticies[0];
	Vector3 crossResult;
	if (isFirst)
		crossResult = side1.Cross(side2);
	else
		crossResult = side2.Cross(side1);
	point.x = static_cast<int>(point.x);
	point.y = static_cast<int>(point.y);
	point.z = static_cast<int>(point.z);
	//LOG_3("point", point.x, point.y, point.z);
	Vector3 result = crossResult*(point - verticies[0]);

	return result.x+ result.y+ result.z;
}

GLvoid Light()
{
	GLfloat AmbientLight[] = { 0.5f, 0.5f, 0.5f, 1.0f }; // 
	GLfloat DiffuseLight[] = { 0.1f, 0.1f, 0.1f, 1.0f }; // 적색조명
	GLfloat SpecularLight[] = { 1.0, 1.0, 1.0, 1.0 }; // 백색조명
	GLfloat lightPos[] = { -100.0, 250.0, 300.0, 1.0 }; // 위치: (1, 2, 3)
	
	//GLfloat spotPos[] = { 0.0, 0.0, 0.0, 1.0 };

		// 조명을 사용하도록 설정
	glEnable(GL_LIGHTING);
	// 조명 설정
	glLightfv(GL_LIGHT0, GL_AMBIENT, AmbientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, DiffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, SpecularLight);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	//glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION,spotPos );
	glEnable(GL_LIGHT0);

	GLfloat lightPos1[] = { 0.0, 0.0, -300.0, 1.0 }; // 위치: (1, 2, 3)

													   //GLfloat spotPos[] = { 0.0, 0.0, 0.0, 1.0 };

													   // 조명을 사용하도록 설정
	glEnable(GL_LIGHTING);
	// 조명 설정
	glLightfv(GL_LIGHT1, GL_AMBIENT, AmbientLight);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, DiffuseLight);
	glLightfv(GL_LIGHT1, GL_SPECULAR, SpecularLight);
	glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);
	//glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION,spotPos );
	glEnable(GL_LIGHT1);
}

GLvoid drawScene(GLvoid)
{
	glEnable(GL_LIGHTING);
	GLfloat gray[] = { 0.75f, 0.75f, 0.75f, 1.0f };
	GLfloat specref[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, gray);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specref);
	glMateriali(GL_FRONT, GL_SHININESS, 64);

	GLfloat ambientLight[] = { 0.2f, 0.2f, 0.2f, 1.0f };


	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_NORMALIZE);

	Light();

	

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // 바탕색을 'blue' 로 지정
	glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_DEPTH_BUFFER_BIT);// 설정된 색으로 전체를 칠하기
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	WorldRotation();
	MainGame();
	glPopMatrix();
	glutSwapBuffers(); // 화면에 출력하기
}
int main()
{
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH); // 디스플레이 모드 설정 
	glutInitWindowPosition(100, 30); // 윈도우의 위치지정 
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT); // 윈도우의 크기 지정 
	glutCreateWindow("Example"); // 윈도우 생성 (윈도우 이름) 
	
	Initialize();
	
	glutDisplayFunc(drawScene); // 출력 함수의 지정
	glutReshapeFunc(Reshape); // 다시 그리기 함수의 지정 
	glutKeyboardFunc(Keyboard);
	glutMouseFunc(Mouse);
	glutTimerFunc(100, TimerFunction, 1);
	glutSpecialFunc(KeyboardSpecialKey);
	glutMotionFunc(Motion);
	glutMainLoop();


	return 0;
}
GLvoid Reshape(int w, int h)
{
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//-- 투영은 직각 투영 또는 원근 투영 중 한 개를 설정한다. 
	// . 클리핑 공간 설정: 원근 투영인 경우 
	gluPerspective(60.0f, w / h, 1.0, 2000.0);
	glTranslatef(0.0, 0.0, -1000); //투영 공간을 화면 안쪽으로 이동하여 시야를 확보한다.
								  // 2. 클리핑 공간 설정: 직각 투영인 경우
								  // glOrtho (0, 800.0, 0.0, 600.0, -1.0, 00.0);
								  // 모델 뷰 행렬 스택 재설정 
								  //glOrtho(-500, 500, -500, 500, 0.0,1000 );
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0, 0.0, 0.0, 0.0, 0.0, -1.0, 0, 1, 0);
}

void TimerFunction(int value)
{

	glutPostRedisplay(); // 화면 재 출력, drawscene이 호출
	glutTimerFunc(100, TimerFunction, 1); // 타이머함수 재 설정
}

void Mouse(int button, int state, int x, int y)//x,y값은 윈도우 기준이기 때문에 좌측상단이 0,0이다.
{
	float inputX, inputY;

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		x = x - (float)WINDOW_WIDTH / 2.0;
		y = (WINDOW_HEIGHT - y) - (float)WINDOW_HEIGHT / 2.0;
		//cout << x << " , " << y << endl;
		g_mousePoint[0][0] = x;
		g_mousePoint[0][1] = y;
		g_isClick = true;
	}
	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
		g_isClick = false;
	}

}
void Motion(int x, int y)
{
	x = x - (float)WINDOW_WIDTH / 2.0;
	y = (WINDOW_HEIGHT - y) - (float)WINDOW_HEIGHT / 2.0;
	g_mousePoint[1][0] = x;
	g_mousePoint[1][1] = y;
}

void Keyboard(unsigned char key, int x, int y)
{
	if (key == 'x')
	{
		g_worldRotation.x += 10;
	}
	if (key == 'y')
	{
		g_worldRotation.y += 10;
	}
	if (key == 'z')
	{
		g_worldRotation.z += 10;
	}
	if (key == 'X')
	{
		g_worldRotation.x -= 10;
	}
	if (key == 'Y')
	{
		g_worldRotation.y -= 10;
	}
	if (key == 'Z')
	{
		g_worldRotation.z -= 10;
	}
	if (key == 'i')
	{
		glLoadIdentity();
		g_worldRotation = { 0,0,0 };
	}
	if (key == 'w')
	{
		g_moving.z += 10;
	}
	if (key == 's')
	{
		g_moving.z -= 10;
	}
	if (key == 'a')
	{
		g_moving.x -= 10;
	}
	if (key == 'd')
	{
		g_moving.x += 10;
	}
	if (key == 'q')
	{
		g_moving.y -= 10;
	}
	if (key == 'Q')
	{
		g_moving.y += 10;
	}
	if (key == ' ')
	{
		player.Shoot();
	}
}

void KeyboardSpecialKey(int key, int x, int y)
{
	//if (key == GLUT_KEY_F1)

}


void MenuFunc(int button)
{
	switch (button)
	{
	case 1://은면제거
		depth == true ? depth = false : depth = true;
		break;
	case 2://컬링
		culling == true ? culling = false : culling = true;
		break;
	case 3://쉐이딩
		shading == true ? shading = false : shading = true;
		break;
	case 4://윗면
		upAnimation == true ? upAnimation = false : upAnimation = true;
		break;
	case 5://앞면
		frontAnimation == true ? frontAnimation = false : frontAnimation = true;
		break;

	}
	glutPostRedisplay();
}
void Render(int shape)
{
	switch (shape)
	{
	case CUBE:
		glutSolidCube(0.3);
		break;
	case WIRE_CUBE:
		glutWireCube(0.3);
		break;
	case CONE:
		glRotatef(270, 1, 0, 0);
		glutSolidCone(0.3, 0.3, 20, 20);
		break;
	case WIRE_CONE:
		glRotatef(270, 1, 0, 0);
		glutWireCone(0.3, 0.3, 20, 20);
		break;
	case SPHERE:
		glutSolidSphere(0.3, 20, 20);
		break;
	case WIRE_SPHERE:
		glutWireSphere(0.3, 20, 20);
		break;
	case TEAPOT:
		glutSolidTeapot(0.3);
		break;
	case WIRE_TEAPOT:
		glutWireTeapot(0.3);
		break;
	}
}
void WorldRotation()
{
	if (g_isClick)
	{
		auto yValue = (g_mousePoint[0][0] - g_mousePoint[1][0])*0.05;
		auto xValue = (g_mousePoint[0][1] - g_mousePoint[1][1])*-0.05;
		g_worldRotation.x -= xValue;
		g_worldRotation.y -= yValue;
	}
	glRotatef(g_worldRotation.x, 1, 0, 0);
	glRotatef(g_worldRotation.y, 0, 1, 0);
	glRotatef(g_worldRotation.z, 0, 0, 1);
}