#include "std.h"
#include <random>
using namespace std;

//-----------�����Լ�

void make_vertexShaders();
void make_fragmentShaders();
void make_shaderProgram();
void InitBuffer();

bool Change = false;
bool Fill = true;
int rotateKey = 0;
int self_key = 1;

GLvoid drawScene();
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);
GLvoid Mouse(int button, int state, int x, int y);
GLvoid Motion(int x, int y);

GLint width, height;
GLuint shaderProgramID; //--- ���̴� ���α׷� �̸�
GLuint vertexShader;	//--- ���ؽ� ���̴� ��ü
GLuint fragmentShader;	//--- �����׸�Ʈ ���̴� ��ü
GLchar* vertexSource, * fragmentSource; //--- �ҽ��ڵ� ���� ����

GLuint m_vao;
GLuint m_vbo;
GLuint m_VBOvertex;
GLuint m_VBOcolor;
std::random_device rd;
std::default_random_engine dre(rd());
std::uniform_real_distribution<float> post(-0.05, 0.05);
std::uniform_int_distribution<int> pos(0, 5);
float Rt = 30.0f;

const float vertex[] =
{ 1.0,0.0,0.0,

1.0,0.0,0.0,

1.0,0.0,0.0,

1.0,0.0,0.0,

1.0,0.0,0.0,


};
const float color[] =
{ 0.5f, 0.0f, 0.5f,//4
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

class Sun
{
public:
	GLUquadricObj* qobj;
	glm::mat4 Rx = glm::mat4(1.0f);
	glm::mat4 Ry = glm::mat4(1.0f);
	glm::mat4 SRx = glm::mat4(1.0f);
	glm::mat4 SRy = glm::mat4(1.0f);
	glm::mat4 Tx = glm::mat4(1.0f);
	glm::mat4 TR = glm::mat4(1.0f);
	float self_xRotate = 0.0f;
	float self_yRotate = 0.0f;
	float xRotate = 0.0f;
	float yRotate = 0.0f;
	double xMove = 0.0, yMove = 0.0, zMove = 0.0;

	Sun()
	{
		qobj = gluNewQuadric(); // ��ü �����ϱ�

	}

	void Draw()
	{
		gluQuadricDrawStyle(qobj, Fill ? GLU_FILL : GLU_LINE); // ���� ��Ÿ��
		gluSphere(qobj, 0.25, 50, 50); // ��ü �����
	}

	void Set_Matrix()
	{
		Tx = glm::translate(Tx, glm::vec3(xMove, yMove, zMove));
		Rx = glm::rotate(Rx, glm::radians(30.0f + xRotate), glm::vec3(1.0, 0.0, 0.0));
		Ry = glm::rotate(Ry, glm::radians(-30.0f + yRotate), glm::vec3(0.0, 1.0, 0.0));
		SRx = glm::rotate(SRx, glm::radians(self_xRotate), glm::vec3(1.0, 0.0, 0.0));
		SRy = glm::rotate(SRy, glm::radians(self_yRotate), glm::vec3(0.0, 1.0, 0.0));
		TR = (Rx * Ry) * (Tx * SRx * SRy);

		unsigned int modelLocation = glGetUniformLocation(shaderProgramID, "Transform"); // ���ؽ� ���̴����� �𵨸� ��ȯ ��ġ ��������
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR));
		glBindVertexArray(m_vao);
	}
	void Self_Rotation()
	{

	}
};

class Planet
{
public:
	GLUquadricObj* qobj;
	glm::mat4 Rx = glm::mat4(1.0f);
	glm::mat4 Ry = glm::mat4(1.0f);
	glm::mat4 SRx = glm::mat4(1.0f);
	glm::mat4 SRy = glm::mat4(1.0f);
	glm::mat4 Tx = glm::mat4(1.0f);
	glm::mat4 TR = glm::mat4(1.0f);
	float self_xRotate = 0.0f;
	float self_yRotate = 0.0f;
	float xRotate = 0.0f;
	float yRotate = 0.0f;
	double xMove = 0.0, yMove = 0.0, zMove = 0.0;

	Planet()
	{
		qobj = gluNewQuadric(); // ��ü �����ϱ�

	}

	void Set_Matrix()
	{
		Tx = glm::translate(Tx, glm::vec3(xMove, yMove, zMove));
		Rx = glm::rotate(Rx, glm::radians(30.0f + xRotate), glm::vec3(1.0, 0.0, 0.0));
		Ry = glm::rotate(Ry, glm::radians(-30.0f + yRotate), glm::vec3(0.0, 1.0, 0.0));
		SRx = glm::rotate(SRx, glm::radians(self_xRotate), glm::vec3(1.0, 0.0, 0.0));
		SRy = glm::rotate(SRy, glm::radians(self_yRotate), glm::vec3(0.0, 1.0, 0.0));
		TR = (Rx * Ry) * (Tx * SRx * SRy);

		unsigned int modelLocation = glGetUniformLocation(shaderProgramID, "Transform"); // ���ؽ� ���̴����� �𵨸� ��ȯ ��ġ ��������
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR));
		glBindVertexArray(m_vao);
	}
	void Draw()
	{
		gluQuadricDrawStyle(qobj, Fill ? GLU_FILL : GLU_LINE); // ���� ��Ÿ��
		gluSphere(qobj, 0.125, 50, 50); // ��ü �����
	}

	void Rotation()
	{

	}
	void Self_Rotation()
	{

	}
};

class Moon
{
public:
	GLUquadricObj* qobj;
	glm::mat4 Rx = glm::mat4(1.0f);
	glm::mat4 Ry = glm::mat4(1.0f);
	glm::mat4 SRx = glm::mat4(1.0f);
	glm::mat4 SRy = glm::mat4(1.0f);
	glm::mat4 Tx = glm::mat4(1.0f);
	glm::mat4 TR = glm::mat4(1.0f);
	float self_xRotate = 0.0f;
	float self_yRotate = 0.0f;
	float xRotate = 0.0f;
	float yRotate = 0.0f;
	double xMove = 0.0, yMove = 0.0, zMove = 0.0;

	Moon()
	{
		qobj = gluNewQuadric(); // ��ü �����ϱ�
		gluQuadricDrawStyle(qobj, Fill ? GLU_FILL : GLU_LINE); // ���� ��Ÿ��
		gluSphere(qobj, 0.25, 50, 50); // ��ü �����
	}

	void Set_Matrix()
	{
		Tx = glm::translate(Tx, glm::vec3(xMove, yMove, zMove));
		Rx = glm::rotate(Rx, glm::radians(30.0f + xRotate), glm::vec3(1.0, 0.0, 0.0));
		Ry = glm::rotate(Ry, glm::radians(-30.0f + yRotate), glm::vec3(0.0, 1.0, 0.0));
		SRx = glm::rotate(SRx, glm::radians(30.0f + self_xRotate), glm::vec3(1.0, 0.0, 0.0));
		SRy = glm::rotate(SRy, glm::radians(-30.0f + self_yRotate), glm::vec3(0.0, 1.0, 0.0));
		TR = (Rx * Ry) * (Tx * SRx * SRy);

		unsigned int modelLocation = glGetUniformLocation(shaderProgramID, "Transform"); // ���ؽ� ���̴����� �𵨸� ��ȯ ��ġ ��������
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR));
		glBindVertexArray(m_vao);
	}

	void Rotation()
	{

	}

	void Self_Rotation()
	{

	}
};

Sun sun[1];
Planet planet[3];
Moon moon[3];


void InitBuffer()
{
	glGenVertexArrays(1, &m_vao); //--- VAO ��ü ����, params(����, GLuint*)// n���� �޾Ƶ� �����ǰ� �Ϸ�
	glBindVertexArray(m_vao); //--- VAO�� ���ε��ϱ�

	// VBO ��ü ����
	glGenBuffers(1, &m_VBOvertex);
	//�̷� vbo�� ���ſ��並 �˷��ְ�
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOvertex);
	//�̷� �� �����͸� �ø��ſ���
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * 5, vertex, GL_STATIC_DRAW);
	int positionAttrib = glGetAttribLocation(shaderProgramID, "vPos");
	glEnableVertexAttribArray(positionAttrib);
	glVertexAttribPointer(positionAttrib, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

	// VBO ��ü ����
	glGenBuffers(1, &m_VBOcolor);
	//�̷� vbo�� ���ſ��並 �˷��ְ�
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOcolor);
	//�̷� �� �����͸� �ø��ſ���
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * 5, color, GL_STATIC_DRAW);
	//vertex shader���� �ø� ������ ��ġ
	int vColorLocation = glGetAttribLocation(shaderProgramID, "vColor");
	//� �Ӽ��� ų ������
	glEnableVertexAttribArray(vColorLocation);
	//� �Ӽ��� � �����͸� �ѱ� ������
	glVertexAttribPointer(vColorLocation, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

}


void main(int argc, char** argv)	//--- ������ ����ϰ� �ݹ��Լ� ����
{
	width = 800;
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
	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutMouseFunc(Mouse);
	glutKeyboardFunc(Keyboard);
	glutMotionFunc(Motion);
	glEnable(GL_DEPTH_TEST);

	InitBuffer();
	sun[0] = Sun();
	planet[0] = Planet();
	planet[1] = Planet();
	planet[2] = Planet();

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
	//--- ����� ���� ����
	glClearColor(1.0, 1.0, 1.0, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);

	//--- ������ ���������ο� ���̴� �ҷ�����
	glUseProgram(shaderProgramID);

	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::ortho(-2.0f, 2.0f, -2.0f, 2.0f, -2.0f, 2.0f);

	unsigned int projectionLocation = glGetUniformLocation(shaderProgramID, "projection");
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);
	glBindVertexArray(m_vao);

	glm::mat4 Orbit_Rotate = glm::mat4(1.0f);
	Orbit_Rotate = glm::rotate(Orbit_Rotate, glm::radians(Rt), glm::vec3(1.0, 0.0, 0.0));
	unsigned int modelLocation = glGetUniformLocation(shaderProgramID, "Transform");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(Orbit_Rotate));
	glBindVertexArray(m_vao);
	glDrawArrays(GL_LINE_STRIP, 0, 5);

//	sun->Set_Matrix();
//	sun[0].Draw();

//	planet[0].xMove = 0.1;
//	planet->Set_Matrix();
//	planet[0].Draw();

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

	case 'm':
		Fill = false;
		break;
	case 'M':
		Fill = true;
		break;
	case 'q':
		exit(0);
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

