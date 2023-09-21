#include <iostream>
#include <gl/glew.h> // 필요한 헤더파일 include
#include<gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <random>
#include <vector>
GLvoid drawScene();
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);
GLvoid Mouse(int button, int state, int x, int y);
GLvoid Motion(int x, int y);
using namespace std;

float sizeX = 800;
float sizeY = 600;

bool click = false;
class Rect
{
public:
    GLfloat x1, y1, x2, y2;
    GLfloat r, g, b;
    Rect(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2, GLfloat r, GLfloat g, GLfloat b);
};

Rect::Rect(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2, GLfloat r, GLfloat g, GLfloat b)
{
    this->x1 = x1;
    this->x2 = x2;
    this->y1 = y1;
    this->y2 = y2;
    this->r = r;
    this->g = g;
    this->b = b;
}

std::vector<Rect*> v;
//GLclampf distr(eng);
int i = 0;

std::random_device rd;
std::default_random_engine eng(rd());
std::uniform_real_distribution<> distr(0.0f, 1.0f);
bool IsTimerAlive = true;

void main(int argc, char** argv) //--- 윈도우 출력하고 콜백함수 설정
{

    //--- 윈도우 생성하기
    glutInit(&argc, argv); // glut 초기화
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // 디스플레이 모드 설정
    glutInitWindowPosition(0, 0); // 윈도우의 위치 지정
    glutInitWindowSize(800, 600); // 윈도우의 크기 지정
    glutCreateWindow("Example1"); // 윈도우 생성
    //--- GLEW 초기화하기
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK
        ) // glew 초기화
    {
        std::cerr << "Unable to initialize GLEW" << std::endl;
        exit(EXIT_FAILURE);
    }
    else
        std::cout << "GLEW Initialized\n";
    glutDisplayFunc(drawScene); // 출력 콜백함수의 지정
    glutReshapeFunc(Reshape); // 다시 그리기 콜백함수 지정
    glutKeyboardFunc(Keyboard);
    glutMouseFunc(Mouse);
    glutMotionFunc(Motion);
    glutMainLoop(); // 이벤트 처리 시작
}

GLvoid drawScene() //--- 콜백 함수: 그리기 콜백 함수
{
    //--- 변경된 배경색 설정
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f); //--- 바탕색을 변경
    glClear(GL_COLOR_BUFFER_BIT); //--- 설정된 색으로 전체를 칠하기   
    for (i = 0; i < v.size(); i++)
    {
        v[i]->r = (GLclampf)distr(eng);
        v[i]->g = (GLclampf)distr(eng);
        v[i]->b = (GLclampf)distr(eng);
        glColor3f(v[i]->r, v[i]->g, v[i]->b);
        glRectf(v[i]->x1, v[i]->y1, v[i]->x2, v[i]->y2);
    }

    glutSwapBuffers(); //--- 화면에 출력하기

}
GLvoid Reshape(int w, int h) //--- 콜백 함수: 다시 그리기 콜백 함수
{
    glViewport(0, 0, w, h);
}

GLvoid Mouse(int button, int state, int x, int y)
{
    int count = 0;
    float screenx = (x / (float)sizeX) * 2 - 1;
    float screeny = -1 * ((y / (float)sizeY) * 2 - 1);
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {

        if (count <= 4)
        {
            click = true;
            v.push_back(new Rect(screenx - 0.1f, screeny - 0.1f, screenx + 0.1f, screeny + 0.1f, 1.0f, 1.0f, 1.0f));
            count++;
        }
    }
    else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
    {
        click = false;
    }


}

GLvoid Motion(int x, int y)
{
    float screenx = (x / (float)sizeX) * 2 - 1;
    float screeny = -1 * ((y / (float)sizeY) * 2 - 1);
    if (click == true)
    {
        //r1.x1 = screenx - 0.1f;
        //r1.y1 = screeny - 0.1f;
        //r1.x2 = screenx + 0.1f;
        //r1.y2 = screeny + 0.1f;
    }
}

GLvoid Keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 'a':
        break;
    }
    glutPostRedisplay();
}