// 실습25.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//


#include "stdafx.h"
#include"Planet.h"
#include"Player.h"




GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
void TimerFunction(int value);
void Mouse(int button, int state, int x, int y);
void Keyboard(unsigned char key, int x, int y);
void KeyboardSpecialKey(int key, int x, int y);
void MenuFunc(int button);
void Motion(int x, int y);
//---------custom function
void Render(int Planet);
void Initialize();
void WorldRotation();
void Initialize();
int BezierRender(vector<Vector3>& vertexList);

Vector3 g_worldRotation = { 0,0,0 };//전체 죄표계 회전
int g_mousePoint[2][2];
bool g_isClick=false;
vector<Vector3> g_movePoints;
vector<Planet*> g_planetList;
vector<Star*> g_starList;


Player* player;
Vector3 g_selectBarPosition = {0,0,0};//-300,-300,-300 시작이라고 보면됨


void Initialize()
{
	player = new Player();
	g_isClick = false;
	g_movePoints.clear();
	g_planetList.clear();
	g_starList.clear();
	
	g_selectBarPosition = { 0,0,0 };
	g_movePoints.push_back({ -300,300,300 });

	//별만들기----------------------------------------------
	for (int i = 0; i < 3000; i++)
	{
		auto newStar = new Star();
		g_starList.push_back(newStar);
	}

	auto newplanet = new Planet();
	newplanet->position = { -300,300,300 };
	newplanet->isTree = false;
	g_planetList.push_back(newplanet);
	for (int i = 0; i < 15;i++)
	{
		auto newplanet = new Planet();
		g_planetList.push_back(newplanet);
	}
	newplanet = new Planet();
	newplanet->position = { 300,-300,-300 };
	newplanet->isTree = false;
	g_planetList.push_back(newplanet);
}

void WorldRender()//-300~300 까지 최대
{
	glLineWidth(5);
	glPushMatrix();
	glTranslatef(g_selectBarPosition.x, g_selectBarPosition.y, g_selectBarPosition.z);
	glBegin(GL_LINES);
	glColor3f(1, 0, 0);
	glVertex3f(-2000, -300, 300);
	glVertex3f(2000, -300, 300);
	glColor3f(0, 1, 0);
	glVertex3f(-300, 2000, 300);
	glVertex3f(-300, -2000, 300);
	glColor3f(0, 0, 1);
	glVertex3f(-300, -300, 2000);
	glVertex3f(-300, -300, -2000);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glColor3f(1, 1, 1);
	glutWireCube(600);
	glPointSize(10);
	glBegin(GL_POINTS);
	for (auto pos : g_movePoints)
	{
		glVertex3f(pos.x, pos.y, pos.z);
	}
	glEnd();
	glPopMatrix();
	for (auto planet : g_planetList)
	{
		planet->Update();
		planet->Render();
	}
	for (auto star : g_starList)
	{
		star->Render();
	}

}


Player player2;


void MainGame()
{
	//cout << g_selectBarPosition.x - 300 << " " << g_selectBarPosition.y - 300 << " " << g_selectBarPosition.z + 300 << endl;
	WorldRender();
	player->Update();
	player->Render();

}
GLvoid drawScene(GLvoid)
{
	
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
	srand((unsigned)time(NULL));
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
void SetReady()
{
	player->movePoint = g_movePoints;
	player->ready = true;
	g_planetList[g_planetList.size() - 1]->markColor = { 0,1,0 };
}
void Keyboard(unsigned char key, int x, int y)
{
	if (key == 'x')
	{
		g_selectBarPosition.x += 10;
	}
	if (key == 'y')
	{
		g_selectBarPosition.y += 10;
	}
	if (key == 'z')
	{
		g_selectBarPosition.z -= 10;
	}
	if (key == 'X')
	{
		g_selectBarPosition.x -= 10;
	}
	if (key == 'Y')
	{
		g_selectBarPosition.y -= 10;
	}
	if (key == 'Z')
	{
		g_selectBarPosition.z += 10;
	}
	if (key == 'i')
	{
		cout << g_movePoints.size()<<endl;
		glLoadIdentity();
		g_worldRotation = { 0,0,0 };
		Initialize();
	}
	if (key == 't')
	{
		g_movePoints.push_back({ -200,290,100 });
		g_movePoints.push_back({ -100,-200,-100 });
		g_movePoints.push_back({ 0,300,100 });
		g_movePoints.push_back({ 100,-100,-200 });
		g_movePoints.push_back({ 200,100,-200 });
		g_movePoints.push_back({ 300,-300,-300 });
		SetReady();
	}
	if (key == ' ')
	{
		cout << g_movePoints.size() << endl;
		if(g_selectBarPosition.x - 300<300&& g_selectBarPosition.x - 300>-300&& g_selectBarPosition.y - 300<300 && g_selectBarPosition.y - 300>-300
			&& g_selectBarPosition.z + 300<300 && g_selectBarPosition.z + 300>-300)
		{
			if (g_selectBarPosition.x - 300>230 && g_selectBarPosition.y - 300<-230 && g_selectBarPosition.z + 300<-230)
			{
				
				g_movePoints.push_back({ g_selectBarPosition.x - 300,g_selectBarPosition.y - 300,g_selectBarPosition.z + 300 });
				if (g_movePoints.size() < 4)
				{
					while(g_movePoints.size()!=4)
						g_movePoints.push_back({ g_selectBarPosition.x - 300,g_selectBarPosition.y - 300,g_selectBarPosition.z + 300 });
				}
				else {
					while ((g_movePoints.size() - 4) % 3 != 0)//300,-300,-300;
					{
						g_movePoints.push_back({ g_selectBarPosition.x - 300,g_selectBarPosition.y - 300,g_selectBarPosition.z + 300 });
					}
				}
				SetReady();
				return;
			}
			g_movePoints.push_back({ g_selectBarPosition.x - 300,g_selectBarPosition.y - 300,g_selectBarPosition.z + 300 });
		}
	}
	if (key == '1')
	{
		
		player->movingCount = 5;
	}
	if (key == '2')
	{

		player->movingCount = 2;
	}
	if (key == '3')
	{

		player->movingCount = 3;
	}
}

void KeyboardSpecialKey(int key, int x, int y)
{
	//if (key == GLUT_KEY_F1)

}


void MenuFunc(int button)
{
	
	glutPostRedisplay();
}
void Render(int Planet)
{
	switch (Planet)
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



