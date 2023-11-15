#include "std.h"
#include <random>
using namespace std;

//-----------�����Լ�

void make_vertexShaders();
void make_fragmentShaders();
void make_shaderProgram();
void InitBuffer();

double xMove = 0.0, yMove = 0.0, zMove = 0.0;
float xRotate = 0.0f;
float yRotate = 0.0f;
float self_yRotate = 0.0f;
float zRotate = 0.0f;

float cameraPosZ = 3.0f;
float cameraPosX = 3.0f;
float cameraPosY = 3.0f;

float cameraRotation = 0.0f;

GLvoid drawScene();
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);
GLvoid Mouse(int button, int state, int x, int y);
GLvoid Motion(int x, int y);

void MyDisplay();



GLint width, height;
GLuint shaderProgramID; //--- ���̴� ���α׷� �̸�
GLuint vertexShader;	//--- ���ؽ� ���̴� ��ü
GLuint fragmentShader;	//--- �����׸�Ʈ ���̴� ��ü
GLchar* vertexSource, * fragmentSource; //--- �ҽ��ڵ� ���� ����

GLuint m_vao;
GLuint m_vbo;
GLuint m_VBOvertex;
GLuint m_VBOcolor;

GLuint m_Axixvertex;
GLuint m_Axixcolor;
GLuint m_vao2;

std::random_device rd;
std::default_random_engine dre(rd());
std::uniform_real_distribution<float> post(-0.05, 0.05);
std::uniform_int_distribution<int> pos(0, 5);

const float axix[] =
{ 0.0f,0.0f,0.0f,
2.0f,0.0f,0.0f,

0.0f,0.0f,0.0f,
-2.0f,0.0f,0.0f,        // x��

 0.0f,0.0f,0.0f,
0.0f,2.0f,0.0f,

0.0f,0.0f,0.0f,
0.0f,-2.0f,0.0f,        // y��

 0.0f,0.0f,0.0f,
0.0f,0.0f,2.0f,

0.0f,0.0f,0.0f,
0.0f,0.0f,-2.0f,       //z��

10.0,0.0, -10.0,
 -10.0, 0.0, -10.0,
10.0, 0.0, 10.0,

-10.0, 0.0, -10.0,
-10.0,0.0, 10.0,
10.0, 0.0, 10.0

};
const float axix_color[] =
{
	1.0f,0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,

	1.0f,0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,

	0.0f,0.0f, 1.0f,
	0.0f, 0.0f,1.0f,

	0.0f,0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,

	0.0f,1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,

	0.0f,1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,

	0.0f, 0.0f, 1.0f,//0
	0.5f, 0.0f, 0.5f,//4
	0.0f, 0.5f, 0.5f,//1

	0.5f, 0.0f, 0.5f,//4
	1.0f, 1.0f, 1.0f,//5
	0.0f, 0.5f, 0.5f,//1


};

const float vertex[] =
{
0.500000, 0.500000, -0.500000,
0.500000, -0.500000, -0.500000,
-0.500000, -0.500000, -0.500000,

0.500000, 0.500000, -0.500000,
-0.500000, -0.500000, -0.500000,
-0.500000, 0.500000, -0.500000,     // �ո�

-0.500000, -0.500000, 0.500000,
-0.500000, 0.500000, 0.500000,
-0.500000, 0.500000, -0.500000,

 -0.500000, -0.500000, 0.500000,
-0.500000, 0.500000, -0.500000,
-0.500000, -0.500000, -0.500000,		//���� ����

0.500000, -0.500000, 0.500000,
0.500000, 0.500000, 0.500000,
-0.500000, -0.500000, 0.500000,

 0.500000, 0.500000, 0.500000,
 -0.500000, 0.500000, 0.500000,
-0.500000, -0.500000, 0.500000,		//����

 0.500000, -0.500000, -0.500000,
0.500000, 0.500000, -0.500000,
0.500000, -0.500000, 0.500000,

0.500000, 0.500000, -0.500000,
0.500000, 0.500000, 0.500000,
0.500000, -0.500000, 0.500000,		// ���� ����

0.500000, 0.500000, -0.500000,
 -0.500000, 0.500000, -0.500000,
0.500000, 0.500000, 0.500000,

-0.500000, 0.500000, -0.500000,
 -0.500000, 0.500000, 0.500000,
0.500000, 0.500000, 0.500000,		// �ظ�

0.500000, -0.500000, -0.500000,
0.500000, -0.500000, 0.500000,
-0.500000, -0.500000, 0.500000,

0.500000, -0.500000, -0.500000,
-0.500000, -0.500000, 0.500000,
-0.500000, -0.500000, -0.500000,			//�޸�
};

const float color[] =
{
	0.5f, 0.0f, 0.5f,//4
	0.0f, 0.0f, 1.0f,//0
	0.0f, 0.0f, 0.0f,//3

	0.5f, 0.0f, 0.5f,//4
	0.0f, 0.0f, 0.0f,//3
	1.0f, 0.0f, 0.0f,//7		//1

	0.0f, 1.0f, 0.0f,//2
	0.5f, 0.5f, 0.0f,//6
	1.0f, 0.0f, 0.0f,//7

	0.0f, 1.0f, 0.0f,//2
	1.0f, 0.0f, 0.0f,//7
	0.0f, 0.0f, 0.0f,//3    //2

	0.0f, 0.5f, 0.5f,//1
	1.0f, 1.0f, 1.0f,//5
	0.0f, 1.0f, 0.0f,//2

	1.0f, 1.0f, 1.0f,//5
	0.5f, 0.5f, 0.0f,//6
	0.0f, 1.0f, 0.0f,//2   //3

	0.0f, 0.0f, 1.0f,//0
	0.5f, 0.0f, 0.5f,//4
	0.0f, 0.5f, 0.5f,//1

	0.5f, 0.0f, 0.5f,//4
	1.0f, 1.0f, 1.0f,//5
	0.0f, 0.5f, 0.5f,//1  //4

	0.5f, 0.0f, 0.5f,//4
	1.0f, 0.0f, 0.0f,//7
	1.0f, 1.0f, 1.0f,//5

	1.0f, 0.0f, 0.0f,//7
	0.5f, 0.5f, 0.0f,//6
	1.0f, 1.0f, 1.0f,//5 //5

	0.0f, 0.0f, 1.0f,//0
	0.0f, 0.5f, 0.5f,//1
	0.0f, 1.0f, 0.0f,//2 

	0.0f, 0.0f, 1.0f,//0
	0.0f, 1.0f, 0.0f,//2
	0.0f, 0.0f, 0.0f,//3  //6
};
int index = 0;
int cnt = 0;


void InitBuffer()
{
	glGenVertexArrays(1, &m_vao); //--- VAO ��ü ����, params(����, GLuint*)// n���� �޾Ƶ� �����ǰ� �Ϸ�
	glBindVertexArray(m_vao); //--- VAO�� ���ε��ϱ�

	// VBO ��ü ����
	glGenBuffers(1, &m_VBOvertex);
	//�̷� vbo�� ���ſ��並 �˷��ְ�
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOvertex);
	//�̷� �� �����͸� �ø��ſ���
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 18 * 6, vertex, GL_STATIC_DRAW);
	int positionAttrib = glGetAttribLocation(shaderProgramID, "vPos");
	glEnableVertexAttribArray(positionAttrib);
	glVertexAttribPointer(positionAttrib, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

	// VBO ��ü ����
	glGenBuffers(1, &m_VBOcolor);
	//�̷� vbo�� ���ſ��並 �˷��ְ�
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOcolor);
	//�̷� �� �����͸� �ø��ſ���
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 18 * 6, color, GL_STATIC_DRAW);
	//vertex shader���� �ø� ������ ��ġ
	int vColorLocation = glGetAttribLocation(shaderProgramID, "vColor");
	//� �Ӽ��� ų ������
	glEnableVertexAttribArray(vColorLocation);
	//� �Ӽ��� � �����͸� �ѱ� ������
	glVertexAttribPointer(vColorLocation, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

	glGenVertexArrays(1, &m_vao2); //--- VAO ��ü ����, params(����, GLuint*)// n���� �޾Ƶ� �����ǰ� �Ϸ�
	glBindVertexArray(m_vao2); //--- VAO�� ���ε��ϱ�

	// VBO ��ü ����
	glGenBuffers(1, &m_Axixvertex);
	//�̷� vbo�� ���ſ��並 �˷��ְ�
	glBindBuffer(GL_ARRAY_BUFFER, m_Axixvertex);
	//�̷� �� �����͸� �ø��ſ���
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * 18, axix, GL_STATIC_DRAW);
	int axixLocation = glGetAttribLocation(shaderProgramID, "vPos");
	glEnableVertexAttribArray(axixLocation);
	glVertexAttribPointer(axixLocation, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

	// VBO ��ü ����
	glGenBuffers(1, &m_Axixcolor);
	//�̷� vbo�� ���ſ��並 �˷��ְ�
	glBindBuffer(GL_ARRAY_BUFFER, m_Axixcolor);
	//�̷� �� �����͸� �ø��ſ���
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * 18, axix_color, GL_STATIC_DRAW);
	//vertex shader���� �ø� ������ ��ġ
	int axixColor = glGetAttribLocation(shaderProgramID, "vColor");
	//� �Ӽ��� ų ������
	glEnableVertexAttribArray(axixColor);
	//� �Ӽ��� � �����͸� �ѱ� ������
	glVertexAttribPointer(axixColor, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

	glEnable(GL_DEPTH_TEST);
}


void main(int argc, char** argv)	//--- ������ ����ϰ� �ݹ��Լ� ����
{
	width = 1024;
	height = 800;
	//--- ������ �����ϱ�
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(width, height);
	glutCreateWindow("Example1");
	//--- GLEW �ʱ�ȭ�ϱ�
	glewExperimental = GL_TRUE;
	glewInit();
	make_shaderProgram();
	glutDisplayFunc(MyDisplay);
	glutReshapeFunc(Reshape);
	glutMouseFunc(Mouse);
	glutKeyboardFunc(Keyboard);
	glutMotionFunc(Motion);

	InitBuffer();

	glutMainLoop();

}
//--- ���ؽ� ���̴� ��ü �����
void make_vertexShaders()
{
	vertexSource = filetobuf("vertex.glsl");
	//--- ���ؽ� ���̴� ��ü �����
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	//--- ���̴� �ڵ带 ���̴� ��ü�� �ֱ�
	glShaderSource(vertexShader, 1, (const GLchar**)&vertexSource, 0);
	//--- ���ؽ� ���̴� �������ϱ�
	glCompileShader(vertexShader);
	//--- �������� ����� ���� ���� ���: ���� üũ
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, errorLog);
		cerr << "ERROR: vertex shader ������ ����\n" << errorLog << endl;
		return;
	}
}

void make_fragmentShaders()
{
	fragmentSource = filetobuf("fragment.glsl");
	//--- �����׸�Ʈ ���̴� ��ü �����
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	//--- ���̴� �ڵ带 ���̴� ��ü�� �ֱ�
	glShaderSource(fragmentShader, 1, (const GLchar**)&fragmentSource, 0);
	//--- �����׸�Ʈ ���̴� ������
	glCompileShader(fragmentShader);
	//--- �������� ����� ���� ���� ���: ������ ���� üũ
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, errorLog);
		cerr << "ERROR: fragment shader ������ ����\n" << errorLog << endl;
		return;
	}
}


//--- ���̴� ���α׷� �����
void make_shaderProgram()
{
	make_vertexShaders(); //--- ���ؽ� ���̴� �����
	make_fragmentShaders(); //--- �����׸�Ʈ ���̴� �����
	//-- shader Program
	shaderProgramID = glCreateProgram();
	glAttachShader(shaderProgramID, vertexShader);
	glAttachShader(shaderProgramID, fragmentShader);
	glLinkProgram(shaderProgramID);
	//--- ���̴� �����ϱ�
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	//--- Shader Program ����ϱ�
	glUseProgram(shaderProgramID);
}



//--- ��� �ݹ� �Լ�
GLvoid drawScene()
//--- �ݹ� �Լ�: �׸��� �ݹ� �Լ�
{
	glUseProgram(shaderProgramID);

	//--- ����� ���� ����
	//--- ������ ���������ο� ���̴� �ҷ�����

	glm::mat4 Rx = glm::mat4(1.0f);
	glm::mat4 Ry = glm::mat4(1.0f);
	glm::mat4 Rz = glm::mat4(1.0f);
	glm::mat4 Tx = glm::mat4(1.0f);
	glm::mat4 TR = glm::mat4(1.0f);
	glm::mat4 self_Ry = glm::mat4(1.0f);

	glm::mat4 axix_Rx = glm::mat4(1.0f);
	glm::mat4 axix_Ry = glm::mat4(1.0f);
	glm::mat4 axix_Rz = glm::mat4(1.0f);
	glm::mat4 axix_Tx = glm::mat4(1.0f);
	glm::mat4 axix_TR = glm::mat4(1.0f);

	axix_Tx = glm::translate(axix_Tx, glm::vec3(0.0f, 0.0f, 0.0f));
	axix_Rx = glm::rotate(axix_Rx, glm::radians(0.0f), glm::vec3(1.0, 0.0, 0.0));
	axix_Ry = glm::rotate(axix_Ry, glm::radians(0.0f), glm::vec3(0.0, 1.0, 0.0));
	axix_Rz = glm::rotate(axix_Rz, glm::radians(0.0f), glm::vec3(0.0, 0.0, 1.0));

	axix_TR = (axix_Rx * axix_Ry * axix_Rz) * (axix_Tx);

	unsigned int modelLocation = glGetUniformLocation(shaderProgramID, "Transform"); // ���ؽ� ���̴����� �𵨸� ��ȯ ��ġ ��������
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(axix_TR));
	glBindVertexArray(m_vao2);

	glDrawArrays(GL_LINES, 0, 3 * 4);
	glDrawArrays(GL_TRIANGLES, 12, 6);

	Tx = glm::translate(Tx, glm::vec3(xMove, yMove, zMove));
	Rx = glm::rotate(Rx, glm::radians(xRotate), glm::vec3(1.0, 0.0, 0.0));
	Ry = glm::rotate(Ry, glm::radians(yRotate), glm::vec3(0.0, 1.0, 0.0));
	Rz = glm::rotate(Rz, glm::radians(zRotate), glm::vec3(0.0, 0.0, 1.0));
	self_Ry = glm::rotate(self_Ry, glm::radians(self_yRotate), glm::vec3(0.0, 1.0, 0.0));
	TR = (Rx * Ry * Rz) * (Tx * self_Ry);

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR));
	glBindVertexArray(m_vao);
	glDrawArrays(GL_TRIANGLES, 0, 6 * 4);

	
}

void MyDisplay()
{
	glUseProgram(shaderProgramID);
	glClearColor(1.0, 1.0, 1.0, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 200, width / 2, height / 2);

	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 50.0f); //--- ���� ���� ����: fovy, aspect, near, far
	projection = glm::translate(projection, glm::vec3(0.0, 0.0, -5.0)); //--- ������ z�� �̵�
	unsigned int projectionLocation = glGetUniformLocation(shaderProgramID, "projection");
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);

	glm::vec3 cameraPos = glm::vec3(cameraPosX, cameraPosY, cameraPosZ); //--- ī�޶� ��ġ
	glm::vec3 cameraDirection = glm::vec3(0.0f, 0.0f, 0.0f); //--- ī�޶� �ٶ󺸴� ����
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f); //--- ī�޶� ���� ����
	glm::mat4 view = glm::mat4(1.0f);

	view = glm::lookAt(cameraPos, cameraDirection, cameraUp);
	view = glm::rotate(view, glm::radians(cameraRotation), glm::vec3(0.0, 1.0, 0.0));
	unsigned int viewLocation = glGetUniformLocation(shaderProgramID, "viewTransform"); //--- ���� ��ȯ ����
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);

	drawScene();

	glViewport(width/2 ,0 , width/2, height/2);

	glm::vec3 cameraPos2 = glm::vec3(0.0f, 0.0f, 1.0f); //--- ī�޶� ��ġ
	view = glm::lookAt(cameraPos2, cameraDirection, cameraUp);
	view = glm::rotate(view, glm::radians(cameraRotation), glm::vec3(0.0, 1.0, 0.0));
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);
	projection = glm::ortho(-2.0f, 2.0f, -2.0f, 2.0f, -2.0f, 200.0f); //--- ���� ���� ���� 
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);
	drawScene();


	glViewport(width / 2, height/2, width / 2, height / 2);

	glm::vec3 cameraPos3 = glm::vec3(0.0f, 3.0f, 0.0f); //--- ī�޶� ��ġ
	cameraUp = glm::vec3(0.0f, 0.0f, 1.0f);
	view = glm::lookAt(cameraPos3, cameraDirection, cameraUp);
	view = glm::rotate(view, glm::radians(cameraRotation), glm::vec3(0.0, 1.0, 0.0));
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);
	projection = glm::ortho(-2.0f, 2.0f, -2.0f, 2.0f, -2.0f, 200.0f); //--- ���� ���� ���� 
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);
	drawScene();

	glutSwapBuffers(); //--- ȭ�鿡 ����ϱ�

}




//--- �ٽñ׸��� �ݹ� �Լ�
GLvoid Reshape(int w, int h)
//--- �ݹ� �Լ�: �ٽ� �׸��� �ݹ� �Լ�
{
	glViewport(0, 0, w, h);
}

GLvoid Keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'q':
		exit(0);
		break;
	case 'z':
		cameraPosZ += 0.1;
		break;
	case 'Z':
		cameraPosZ -= 0.1;
		break;
	case 'x':
		cameraPosX += 0.1;
		break;
	case 'X':
		cameraPosX -= 0.1;
		break;
	case 'y':
		cameraPosY += 0.1;
		break;
	case 'Y':
		cameraPosY -= 0.1;
		break;
	case 'r':
		cameraRotation += 0.5f;
		break;
	case 'R':
		cameraRotation -= 0.5f;
		break;
	case 'b':
		xMove += 0.1f;
		break;
	case 'B':
		xMove -= 0.1f;
		break;
	case 'm':
		self_yRotate += 1.0f;
		break;
	case 'M':
		self_yRotate -= 1.0f;
		break;
	}
	glutPostRedisplay();
}

GLvoid Mouse(int button, int state, int x, int y)
{
	float X = (float)x / width * 2.0f - 1.0f;
	float Y = -(float)y / height * 2.0f + 1.0f;
	glutPostRedisplay();
}


GLvoid Motion(int x, int y)
{
	float X = (float)x / width * 2.0f - 1.0f;
	float Y = -(float)y / height * 2.0f + 1.0f;
	glutPostRedisplay();
}



