#include "std.h"
#include <random>
using namespace std;

//-----------�����Լ�

void make_vertexShaders();
void make_fragmentShaders();
void make_shaderProgram();
void InitBuffer();

bool tetrahedron = false;

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


const float vertex[] = 
	{0.0, 0.0, 0.0,
	 0.5, 0.5, 0.0,
	0.5,0.0, 0.0,

	0.0, 0.0 , 0.0,
	0.0, 0.5, 0.0,
	0.5,0.5, 0.0,		// �ո�

	0.0, 0.0 ,0.0,
	0.0,0.5,0.5,
	0.0,0.5,0.0,

	0.0,0.0,0.0,
	0.0,0.0,0.5,
	0.0,0.5,0.5,		//���� ����

	0.0,0.5,0.0,
	0.5,0.5,0.5,
	0.5,0.5,0.0,

	0.0,0.5,0.0,
	0.0,0.5,0.5,
	0.5,0.5,0.5,		//����

	0.5,0.0,0.0,
	0.5,0.5,0.0,
	0.5,0.5,0.5,

	0.5,0.0,0.0,
	0.5,0.5,0.5,
	0.5,0.0,0.5,		// ���� ����

	0.0,0.0,0.0,
	0.5,0.0,0.0,
	0.5,0.0,0.5,

	0.0,0.0,0.0,
	0.5,0.0,0.5,
	0.0,0.0,0.5,		// �ظ�

	0.0,0.0,0.5,
	0.5,0.0,0.5,
	0.5,0.5,0.5,

	0.0,0.0,0.5,
	0.5,0.5,0.5,
	0.0,0.5,0.5,			//�޸�

	0.0, 0.0, 3.464101/6,
	0.0, 3.464101 / 6,  -1.732050/6,
	-3/6, -1.732050 / 6, -1.732050 / 6,
	
	0.0, 0.0, 3.464101/6,
	-3/6, -1.732050/6, -1.732050/6,
	3/6, -1.732050/6, -1.732050/6,

	0.0, 0.0, 3.464101 / 6,
	3 / 6, -1.732050/ 6, -1.732050/6,
	0.0, 3.464101/6,  -1.732050 / 6,

	0.0, 3.464101/6,  -1.732050/6,
	3 / 6, -1.732050 / 6, -1.732050/6,
	- 3/6, -1.732050/6, -1.732050/6,
	 };
const float color[] =
	{0.0,0.0,1.0,
	0.0,0.0,1.0,
	0.0,0.0,1.0, 
	0.0,0.0,1.0,
	0.0,0.0,1.0,
	0.0,0.0,1.0, 

	1.0,0.0,0.0,
	1.0,0.0,0.0,
	1.0,0.0,0.0,
	1.0,0.0,0.0,
	1.0,0.0,0.0,
	1.0,0.0,0.0,

	0.0,1.0,0.0,
	0.0,1.0,0.0,
	0.0,1.0,0.0,
	0.0,1.0,0.0,
	0.0,1.0,0.0,
	0.0,1.0,0.0,

	1.0,1.0,0.0,
	1.0,1.0,0.0,
	1.0,1.0,0.0,
	1.0,1.0,0.0,
	1.0,1.0,0.0,
	1.0,1.0,0.0,

	0.0,1.0,1.0,
	0.0,1.0,1.0,
	0.0,1.0,1.0,
	0.0,1.0,1.0,
	0.0,1.0,1.0,
	0.0,1.0,1.0,

	0.0,0.5,1.0,
	0.0,0.5,1.0,
	0.0,0.5,1.0,
	0.0,0.5,1.0,
	0.0,0.5,1.0,
	0.0,0.5,1.0,

	1.0,1.0,0.0,
	1.0,1.0,0.0,
	1.0,1.0,0.0,
	0.0,1.0,1.0,
	0.0,1.0,1.0,
	0.0,1.0,1.0,

	0.0,1.0,0.0,
	0.0,1.0,0.0,
	0.0,1.0,0.0,
	0.0,1.0,0.0,
	0.0,1.0,0.0,
	0.0,1.0,0.0,
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
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 18*8, vertex, GL_STATIC_DRAW);
	int positionAttrib = glGetAttribLocation(shaderProgramID, "vPos");
	glEnableVertexAttribArray(positionAttrib);
	glVertexAttribPointer(positionAttrib, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

	// VBO ��ü ����
	glGenBuffers(1, &m_VBOcolor);
	//�̷� vbo�� ���ſ��並 �˷��ְ�
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOcolor);
	//�̷� �� �����͸� �ø��ſ���
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 18*8, color, GL_STATIC_DRAW);
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

	//--- ������ ���������ο� ���̴� �ҷ�����
	glUseProgram(shaderProgramID);
	glm::mat4 Rx = glm::mat4(1.0f);
	glm::mat4 Ry = glm::mat4(1.0f);
	glm::mat4 Tx = glm::mat4(1.0f);
	glm::mat4 TR = glm::mat4(1.0f);

	Tx = glm::translate(Tx, glm::vec3(0.0, 0.0, 0.0));
	Rx = glm::rotate(Rx, glm::radians(10.0f), glm::vec3(1.0, 0.0, 0.0));
	Ry = glm::rotate(Rx, glm::radians(10.0f), glm::vec3(0.0, 1.0, 0.0));

	TR = Rx * Ry * Tx;

	unsigned int modelLocation = glGetUniformLocation(shaderProgramID, "Transform"); // ���ؽ� ���̴����� �𵨸� ��ȯ ��ġ ��������
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR));
	glBindVertexArray(m_vao);
	if (tetrahedron==false)
		glDrawArrays(GL_TRIANGLES, index, 6*cnt);
	if(tetrahedron== true)
		glDrawArrays(GL_TRIANGLES, 6*6 + index, 3 * cnt);
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
	case '1':
		tetrahedron = false;
		std::cout << "1 �Էµ�" << endl;
		index = 0;
		cnt = 1;
		break;
	case '2':
		tetrahedron = false;
		std::cout << "2 �Էµ�" << endl;
		index = 6*1;
		cnt = 1;
		break;
	case '3':
		tetrahedron = false;
		std::cout << "3 �Էµ�" << endl;
		index = 6 * 2;
		cnt = 1;
		break;
	case '4':
		tetrahedron = false;
		std::cout << "4 �Էµ�" << endl;
		index = 6 * 3;
		cnt = 1;
		break;
	case '5':
		tetrahedron = false;
		std::cout << "5 �Էµ�" << endl;
		index = 6 * 4;
		cnt = 1;
		break;
	case '6':
		tetrahedron = false;
		std::cout << "6 �Էµ�" << endl;
		index = 6 * 5;
		cnt = 1;
		break;
	case 'c':
		tetrahedron = false;
		index = 6 * pos(rd);
		cnt = 2;
		break;
	case '7':
		std::cout << "7 �Էµ�" << endl;
		tetrahedron = true;
		index = 3 * 0;
		cnt = 1;
		break;
	case '8':
		std::cout << "8 �Էµ�" << endl;
		tetrahedron = true;
		index = 3 * 1;
		cnt = 1;
		break;
	case '9':
		std::cout << "9 �Էµ�" << endl;
		tetrahedron = true;
		index = 3 * 2;
		cnt = 1;
		break;
	case '0':
		std::cout << "0 �Էµ�" << endl;
		tetrahedron = true;
		index = 3 * 3;
		cnt = 1;
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

