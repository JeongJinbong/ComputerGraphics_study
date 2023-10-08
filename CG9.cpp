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
void TimerFunction(int value);
void TimerFunction2(int value);



GLint width, height;
GLuint shaderProgramID; //--- ���̴� ���α׷� �̸�
GLuint vertexShader;	//--- ���ؽ� ���̴� ��ü
GLuint fragmentShader;	//--- �����׸�Ʈ ���̴� ��ü
GLchar* vertexSource, * fragmentSource; //--- �ҽ��ڵ� ���� ����
int movement = 0;

std::random_device rd;
std::default_random_engine dre(rd());
std::uniform_real_distribution<float> post(-0.05, 0.05);


float* reflectionVector(float P[], float N[])
{
	float result[2] = { 0, 0 };

	result[0] = P[0] + 2 * N[0] * ((-P[0]) * N[0] + (-P[1]) * N[1]);
	result[1] = P[1] + 2 * N[1] * ((-P[0]) * N[0] + (-P[1]) * N[1]);

	return result;

}

class Triangle {
	float color[9];
	float vertex[9];
	float movex = 0.0;
	float movey = 0.0;
	float going_vector[2] = { 0, 0 };
	GLuint m_vao;
	GLuint m_vbo;
	GLuint m_VBOvertex;
	GLuint m_VBOcolor;
	int way;

public:
	Triangle() {}
	void setVAO()
	{
		glGenVertexArrays(1, &m_vao); //--- VAO ��ü ����, params(����, GLuint*)// n���� �޾Ƶ� �����ǰ� �Ϸ� 

		glBindVertexArray(m_vao); //--- VAO�� ���ε��ϱ�

		// VBO ��ü ����
		glGenBuffers(1, &m_VBOvertex);
		//�̷� vbo�� ���ſ��並 �˷��ְ�
		glBindBuffer(GL_ARRAY_BUFFER, m_VBOvertex);
		//�̷� �� �����͸� �ø��ſ���
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 12, vertex, GL_STATIC_DRAW);
		int positionAttrib = glGetAttribLocation(shaderProgramID, "vPos");
		glEnableVertexAttribArray(positionAttrib);
		glVertexAttribPointer(positionAttrib, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

		// VBO ��ü ����
		glGenBuffers(1, &m_VBOcolor);
		//�̷� vbo�� ���ſ��並 �˷��ְ�
		glBindBuffer(GL_ARRAY_BUFFER, m_VBOcolor);
		//�̷� �� �����͸� �ø��ſ���
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 12, color, GL_STATIC_DRAW);
		//vertex shader���� �ø� ������ ��ġ
		int vColorLocation = glGetAttribLocation(shaderProgramID, "vColor");
		//� �Ӽ��� ų ������
		glEnableVertexAttribArray(vColorLocation);
		//� �Ӽ��� � �����͸� �ѱ� ������
		glVertexAttribPointer(vColorLocation, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

		int vPosLocation = glGetUniformLocation(shaderProgramID, "vPosmove");
		glUseProgram(shaderProgramID);
	    glUniform3f(vPosLocation, going_vector[0], going_vector[1], 0.0f);

	}
	Triangle(int x, int y)		//������
	{ 
		float X = (float)x / width * 2.0f - 1.0f;
		float Y = -(float)y /height  * 2.0f + 1.0f;

		going_vector[0] = post(dre);
		going_vector[1] = post(dre);

		float r, g, b;

		vertex[0] = X;
		vertex[1] = Y + 0.1;
		vertex[2] = 0.0f;
		vertex[3] = X - 0.07;
		vertex[4] = Y - 0.05;
		vertex[5] = 0.0f;
		vertex[6] = X + 0.07;
		vertex[7] = Y - 0.05;
		vertex[8] = 0.0f;

		r = (float)(rand() % 256) / 256;
		g = (float)(rand() % 256) / 256;
		b = (float)(rand() % 256) / 256;

		color[0] = color[3] = color[6] = r;
		color[1] = color[4] = color[7] = g;
		color[2] = color[5] = color[8] = b;
		setVAO();
	}

	void Draw() {
		glBindVertexArray(m_vao);
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}
	
	void Bounce()
	{
		vertex[0] += going_vector[0];
		vertex[1] += going_vector[1];
		vertex[3] += going_vector[0];
		vertex[4] += going_vector[1];
		vertex[6] += going_vector[0];
		vertex[7] += going_vector[1];

		float top[2] = { 0 , -1 };
		float bottom[2] = { 0 , 1 };
		float left[2] = { 1 , 0 };
		float right[2] = { -1 , 0 };

		float* temp = going_vector;

		if (vertex[1] + going_vector[1] > 1.0f)
		{
			temp = reflectionVector(going_vector, top);

		}

		else if (vertex[6] + going_vector[0] > 1.0f)
		{
			 temp = reflectionVector(going_vector, right);
		
		}

		else if (vertex[3] + going_vector[0] < -1.0f)
		{
			temp = reflectionVector(going_vector, left);
			
		}

		else if (vertex[4] + going_vector[1] < -1.0f)
		{
			 temp = reflectionVector(going_vector, bottom);
		}

		going_vector[0] = temp[0];
		going_vector[1] = temp[1];

		setVAO();
	}

	void zigzag()
	{
		vertex[0] += going_vector[0];
		vertex[1] += going_vector[1];
		vertex[3] += going_vector[0];
		vertex[4] += going_vector[1];
		vertex[6] += going_vector[0];
		vertex[7] += going_vector[1];
		setVAO();

	}
};

Triangle tri[4];

void main(int argc, char** argv)	//--- ������ ����ϰ� �ݹ��Լ� ����
{
	width = 800;
	height = 800;
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

	tri[0] = Triangle(500, 200);
	tri[1] = Triangle(200, 200);
	tri[2] = Triangle(200, 500);
	tri[3] = Triangle(500, 500);

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
	for (int i = 0; i < 4; i++)
	{
		tri[i].Draw();
	}
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
	case '1':
		if (movement == 0)
		{
			glutTimerFunc(10, TimerFunction, 1);
			movement = 1;
		}
		else
		{
			movement = 0;
		}
		break;
	case '2':
		if (movement == 0)
		{
			glutTimerFunc(10, TimerFunction2, 1);
			movement = 2;
		}
		else
		{
			movement = 0;
		}
		break;

	case '3':
		if (movement == 0)
		{
			glutTimerFunc(10, TimerFunction2, 1);
			movement = 3;
		}
		else
		{
			movement = 0;
		}
		break;
	case '4':
		if (movement == 0)
		{
			glutTimerFunc(10, TimerFunction2, 1);
			movement = 4;
		}
		else
		{
			movement = 0;
		}
		break;
	case 'q':
		exit(0);
		break;
	}
	glutPostRedisplay();
}

GLvoid Mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{

		if (x < 400 && y < 400)
		{
			tri[1] = Triangle(x, y);
			std::cout << x << "," << y << std::endl;
		}
		else if (x > 400 && y < 400)
			tri[0] = Triangle(x, y);
		else if (x < 400 && y > 400)
			tri[2] = Triangle(x, y);
		else if (x > 400 && y > 400)
			tri[3] = Triangle(x, y);
	}
	glutPostRedisplay();
}

void TimerFunction(int value)
{
	if (movement == 1)
	{
		for (int i = 0; i < 4; i++)
		{
			tri[i].Bounce();
		}
		glutTimerFunc(10, TimerFunction, 1);
		glutPostRedisplay();
	}
}

void TimerFunction2(int value)
{
	if (movement == 2)
	{
		for (int i = 0; i < 4; i++)
		{
			tri[i].zigzag();
		}
		glutTimerFunc(10, TimerFunction, 1);
		glutPostRedisplay();
	}
}