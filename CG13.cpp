#include "std.h"
#include <random>
using namespace std;

//-----------�����Լ�

void make_vertexShaders();
void make_fragmentShaders();
void make_shaderProgram();

GLvoid drawScene();
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);
GLvoid Mouse(int button, int state, int x, int y);
GLvoid Motion(int x, int y);

bool click = false;
bool PointClick = false;
bool PointClick2 = false;
bool PointClick3 = false;
bool PointClick4 = false;

GLint width, height;
GLuint shaderProgramID; //--- ���̴� ���α׷� �̸�
GLuint vertexShader;	//--- ���ؽ� ���̴� ��ü
GLuint fragmentShader;	//--- �����׸�Ʈ ���̴� ��ü
GLchar* vertexSource, * fragmentSource; //--- �ҽ��ڵ� ���� ����

std::random_device rd;
std::default_random_engine dre(rd());
std::uniform_real_distribution<float> post(-0.05, 0.05);


class Rect {
	float color[18];
	GLuint m_vao;
	GLuint m_vbo;
	GLuint m_VBOvertex;
	GLuint m_VBOcolor;

public:
	float vertex[18];
	float faraway[12];
	float temp[12];

	Rect() {}
	void setVAO()
	{
		glGenVertexArrays(1, &m_vao); //--- VAO ��ü ����, params(����, GLuint*)// n���� �޾Ƶ� �����ǰ� �Ϸ� 
		glBindVertexArray(m_vao); //--- VAO�� ���ε��ϱ�

		// VBO ��ü ����
		glGenBuffers(1, &m_VBOvertex);
		//�̷� vbo�� ���ſ��並 �˷��ְ�
		glBindBuffer(GL_ARRAY_BUFFER, m_VBOvertex);
		//�̷� �� �����͸� �ø��ſ���
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 18, vertex, GL_STATIC_DRAW);
		int positionAttrib = glGetAttribLocation(shaderProgramID, "vPos");
		glEnableVertexAttribArray(positionAttrib);
		glVertexAttribPointer(positionAttrib, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

		// VBO ��ü ����
		glGenBuffers(1, &m_VBOcolor);
		//�̷� vbo�� ���ſ��並 �˷��ְ�
		glBindBuffer(GL_ARRAY_BUFFER, m_VBOcolor);
		//�̷� �� �����͸� �ø��ſ���
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 18, color, GL_STATIC_DRAW);
		//vertex shader���� �ø� ������ ��ġ
		int vColorLocation = glGetAttribLocation(shaderProgramID, "vColor");
		//� �Ӽ��� ų ������
		glEnableVertexAttribArray(vColorLocation);
		//� �Ӽ��� � �����͸� �ѱ� ������
		glVertexAttribPointer(vColorLocation, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

	}
	Rect(int x, int y)		//������
	{
		float X = (float)x / width * 2.0f - 1.0f;
		float Y = -(float)y / height * 2.0f + 1.0f;

		float r, g, b;

		for (int i = 0; i < 12; i++)
			faraway[i] = 0.25;

		vertex[0] = X - faraway[0];
		vertex[1] = Y + faraway[1];
		vertex[2] = 0.0f;
		vertex[3] = X + faraway[2];
		vertex[4] = Y + faraway[3];
		vertex[5] = 0.0f;
		vertex[6] = X + faraway[4];
		vertex[7] = Y - faraway[5];
		vertex[8] = 0.0f;
		vertex[9] = X - faraway[6];
		vertex[10] = Y + faraway[7];
		vertex[11] = 0.0f;
		vertex[12] = X - faraway[8];
		vertex[13] = Y - faraway[9];
		vertex[14] = 0.0f;
		vertex[15] = X + faraway[10];
		vertex[16] = Y - faraway[11];
		vertex[17] = 0.0f;

		r = (float)(rand() % 256) / 256;
		g = (float)(rand() % 256) / 256;
		b = (float)(rand() % 256) / 256;

		color[0] = color[3] = color[6] = color[9] = color[12] = color[15] = r;
		color[1] = color[4] = color[7] = color[10] = color[13] = color[16] = g;
		color[2] = color[5] = color[8] = color[11] = color[14] = color[17] = b;
		setVAO();
	}

	void Draw() {
		glBindVertexArray(m_vao);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}
};

Rect rect[1];

void main(int argc, char** argv)	//--- ������ ����ϰ� �ݹ��Լ� ����
{
	width = 800;
	height = 600;
	//--- ������ �����ϱ�
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
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
	rect[0] = Rect(400, 300);
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

GLvoid drawScene()
//--- �ݹ� �Լ�: �׸��� �ݹ� �Լ�
{
	//--- ����� ���� ����
	glClearColor(1.0, 1.0, 1.0, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//--- ������ ���������ο� ���̴� �ҷ�����
	glUseProgram(shaderProgramID);

	rect[0].Draw();
	
	glutSwapBuffers(); //--- ȭ�鿡 �����
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
	}
	glutPostRedisplay();
}

GLvoid Mouse(int button, int state, int x, int y)
{
	float X = (float)x / width * 2.0f - 1.0f;
	float Y = -(float)y / height * 2.0f + 1.0f;

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		if (rect->vertex[0] <= X && X <= rect->vertex[3] && rect->vertex[7] <= Y && rect->vertex[1] >= Y)
		{
			std::cout << "Ŭ���� �Ǿ���" << endl;

			if (rect->vertex[0]-0.1f <X && rect->vertex[0]+0.1f >X && rect->vertex[1] - 0.1f <Y && rect->vertex[1]+ 0.1f >Y)
			{
				std::cout << "���� ��� ������ Ŭ���� �Ǿ���" << endl;

				PointClick = true;
			}
			else if (rect->vertex[3] - 0.1f < X && rect->vertex[3] + 0.1f > X && rect->vertex[4] - 0.1f < Y && rect->vertex[4] + 0.1f > Y)
			{
				std::cout << "������ ��� ������ Ŭ���� �Ǿ���" << endl;

				PointClick2 = true;
			}
			else if (rect->vertex[6] - 0.1f < X && rect->vertex[6] + 0.1f > X && rect->vertex[7] - 0.1f < Y && rect->vertex[7] + 0.1f > Y)
			{
				std::cout << "������ �ϴ� ������ Ŭ���� �Ǿ���" << endl;

				PointClick3 = true;
			}
			else if (rect->vertex[12] - 0.1f < X && rect->vertex[12] + 0.1f > X && rect->vertex[13] - 0.1f < Y && rect->vertex[13] + 0.1f > Y)
			{
				std::cout << "���� �ϴ� ������ Ŭ���� �Ǿ���" << endl;

				PointClick4 = true;
			}
			else
				click = true;
		}
	}

	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
		std::cout << "Ŭ�� ����" << endl;
		click = false;
		PointClick = false;
		PointClick2 = false;
		PointClick3 = false;
		PointClick4 = false;

	}
	glutPostRedisplay();
}

GLvoid Motion(int x, int y)
{
	float X = (float)x / width * 2.0f - 1.0f;
	float Y = -(float)y / height * 2.0f + 1.0f;

	if (click == true)
	{
		rect->vertex[0] = X - rect->faraway[0];
		rect->vertex[1] = Y + rect->faraway[1];
		rect->vertex[3] = X + rect->faraway[2];
		rect->vertex[4] = Y + rect->faraway[3];
		rect->vertex[6] = X + rect->faraway[4];
		rect->vertex[7] = Y - rect->faraway[5];
		rect->vertex[9] = X - rect->faraway[6];
		rect->vertex[10] = Y + rect->faraway[7];
		rect->vertex[12] = X - rect->faraway[8];
		rect->vertex[13] = Y - rect->faraway[9];
		rect->vertex[15] = X + rect->faraway[10];
		rect->vertex[16] = Y - rect->faraway[11];
		rect->setVAO();
	}
	if (PointClick == true)
	{
		rect->vertex[0] = rect->temp[0];
		rect->vertex[1] = rect->temp[1];
		rect->vertex[9] = rect->temp[2];
		rect->vertex[10] = rect->temp[3];
		rect->vertex[0] = X;
		rect->vertex[1] = Y;
		rect->vertex[9] = X;
		rect->vertex[10] = Y;
		rect->faraway[0] = abs(rect->temp[0] - rect->vertex[0]);
		rect->faraway[1] = abs(rect->temp[1] - rect->vertex[1]);
		rect->faraway[6] = abs(rect->temp[2] - rect->vertex[9]);
		rect->faraway[7] = abs(rect->temp[3] - rect->vertex[10]);

		rect->setVAO();
	}
	if (PointClick2 == true)
	{
		rect->vertex[3] = rect->temp[4];
		rect->vertex[4] = rect->temp[5];
		rect->vertex[3] = X;
		rect->vertex[4] = Y;
		rect->faraway[2] = abs(rect->temp[4] - rect->vertex[3]);
		rect->faraway[3] = abs(rect->temp[5] - rect->vertex[4]);
		rect->setVAO();
	}

	if (PointClick3 == true)
	{
		rect->vertex[6] = rect->temp[6];
		rect->vertex[7] = rect->temp[7];
		rect->vertex[15] = rect->temp[8];
		rect->vertex[16] = rect->temp[9];
		rect->vertex[6] = X;
		rect->vertex[7] = Y;
		rect->vertex[15] = X;
		rect->vertex[16] = Y;
		rect->faraway[4] = abs(rect->temp[6] - rect->vertex[6]);
		rect->faraway[5] = abs(rect->temp[7] - rect->vertex[7]);
		rect->faraway[10] = abs(rect->temp[8] - rect->vertex[15]);
		rect->faraway[11] = abs(rect->temp[9] - rect->vertex[16]);
		rect->setVAO();
	}

	if (PointClick4 == true)
	{
		rect->vertex[12] = rect->temp[10];
		rect->vertex[13] = rect->temp[11];
		rect->vertex[12] = X;
		rect->vertex[13] = Y;
		rect->faraway[8]= abs(rect->temp[10] - rect->vertex[12]);
        rect->faraway[9]= abs(rect->temp[11] - rect->vertex[13]);
		rect->setVAO();
	}
	glutPostRedisplay();
}