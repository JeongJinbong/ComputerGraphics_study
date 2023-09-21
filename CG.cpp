#include <iostream>
#include <gl/glew.h> // �ʿ��� ������� include
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

void main(int argc, char** argv) //--- ������ ����ϰ� �ݹ��Լ� ����
{

    //--- ������ �����ϱ�
    glutInit(&argc, argv); // glut �ʱ�ȭ
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // ���÷��� ��� ����
    glutInitWindowPosition(0, 0); // �������� ��ġ ����
    glutInitWindowSize(800, 600); // �������� ũ�� ����
    glutCreateWindow("Example1"); // ������ ����
    //--- GLEW �ʱ�ȭ�ϱ�
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK
        ) // glew �ʱ�ȭ
    {
        std::cerr << "Unable to initialize GLEW" << std::endl;
        exit(EXIT_FAILURE);
    }
    else
        std::cout << "GLEW Initialized\n";
    glutDisplayFunc(drawScene); // ��� �ݹ��Լ��� ����
    glutReshapeFunc(Reshape); // �ٽ� �׸��� �ݹ��Լ� ����
    glutKeyboardFunc(Keyboard);
    glutMouseFunc(Mouse);
    glutMotionFunc(Motion);
    glutMainLoop(); // �̺�Ʈ ó�� ����
}

GLvoid drawScene() //--- �ݹ� �Լ�: �׸��� �ݹ� �Լ�
{
    //--- ����� ���� ����
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f); //--- �������� ����
    glClear(GL_COLOR_BUFFER_BIT); //--- ������ ������ ��ü�� ĥ�ϱ�   
    for (i = 0; i < v.size(); i++)
    {
        v[i]->r = (GLclampf)distr(eng);
        v[i]->g = (GLclampf)distr(eng);
        v[i]->b = (GLclampf)distr(eng);
        glColor3f(v[i]->r, v[i]->g, v[i]->b);
        glRectf(v[i]->x1, v[i]->y1, v[i]->x2, v[i]->y2);
    }

    glutSwapBuffers(); //--- ȭ�鿡 ����ϱ�

}
GLvoid Reshape(int w, int h) //--- �ݹ� �Լ�: �ٽ� �׸��� �ݹ� �Լ�
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