#include "std.h"
#include <random>
using namespace std;

//-----------메인함수

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
GLuint shaderProgramID; //--- 세이더 프로그램 이름
GLuint vertexShader;	//--- 버텍스 세이더 객체
GLuint fragmentShader;	//--- 프래그먼트 세이더 객체
GLchar* vertexSource, * fragmentSource; //--- 소스코드 저장 변수
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
		glGenVertexArrays(1, &m_vao); //--- VAO 객체 생성, params(갯수, GLuint*)// n개를 받아도 생성되게 하려 

		glBindVertexArray(m_vao); //--- VAO를 바인드하기

		// VBO 객체 생성
		glGenBuffers(1, &m_VBOvertex);
		//이런 vbo를 쓸거에요를 알려주고
		glBindBuffer(GL_ARRAY_BUFFER, m_VBOvertex);
		//이런 점 데이터를 올릴거에요
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 12, vertex, GL_STATIC_DRAW);
		int positionAttrib = glGetAttribLocation(shaderProgramID, "vPos");
		glEnableVertexAttribArray(positionAttrib);
		glVertexAttribPointer(positionAttrib, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

		// VBO 객체 생성
		glGenBuffers(1, &m_VBOcolor);
		//이런 vbo를 쓸거에요를 알려주고
		glBindBuffer(GL_ARRAY_BUFFER, m_VBOcolor);
		//이런 점 데이터를 올릴거에요
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 12, color, GL_STATIC_DRAW);
		//vertex shader에서 올릴 데이터 위치
		int vColorLocation = glGetAttribLocation(shaderProgramID, "vColor");
		//어떤 속성을 킬 것인지
		glEnableVertexAttribArray(vColorLocation);
		//어떤 속성에 어떤 데이터를 넘길 것인지
		glVertexAttribPointer(vColorLocation, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

		int vPosLocation = glGetUniformLocation(shaderProgramID, "vPosmove");
		glUseProgram(shaderProgramID);
	    glUniform3f(vPosLocation, going_vector[0], going_vector[1], 0.0f);

	}
	Triangle(int x, int y)		//생성자
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

void main(int argc, char** argv)	//--- 윈도우 출력하고 콜백함수 설정
{
	width = 800;
	height = 800;
	//--- 윈도우 생성하기
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(width, height);
	glutCreateWindow("Example1");
	//--- GLEW 초기화하기
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





//--- 버텍스 세이더 객체 만들기
void make_vertexShaders()
{
	vertexSource = filetobuf("vertex.glsl");
	//--- 버텍스 세이더 객체 만들기
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	//--- 세이더 코드를 세이더 객체에 넣기
	glShaderSource(vertexShader, 1, (const GLchar**)&vertexSource, 0);
	//--- 버텍스 세이더 컴파일하기
	glCompileShader(vertexShader);
	//--- 컴파일이 제대로 되지 않은 경우: 에러 체크
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, errorLog);
		cerr << "ERROR: vertex shader 컴파일 실패\n" << errorLog << endl;
		return;
	}
}

void make_fragmentShaders()
{
	fragmentSource = filetobuf("fragment.glsl");
	//--- 프래그먼트 세이더 객체 만들기
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	//--- 세이더 코드를 세이더 객체에 넣기
	glShaderSource(fragmentShader, 1, (const GLchar**)&fragmentSource, 0);
	//--- 프래그먼트 세이더 컴파일
	glCompileShader(fragmentShader);
	//--- 컴파일이 제대로 되지 않은 경우: 컴파일 에러 체크
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, errorLog);
		cerr << "ERROR: fragment shader 컴파일 실패\n" << errorLog << endl;
		return;
	}
}


//--- 세이더 프로그램 만들기
void make_shaderProgram()
{
	make_vertexShaders(); //--- 버텍스 세이더 만들기
	make_fragmentShaders(); //--- 프래그먼트 세이더 만들기
	//-- shader Program
	shaderProgramID = glCreateProgram();
	glAttachShader(shaderProgramID, vertexShader);
	glAttachShader(shaderProgramID, fragmentShader);
	glLinkProgram(shaderProgramID);
	//--- 세이더 삭제하기
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	//--- Shader Program 사용하기
	glUseProgram(shaderProgramID);
}



//--- 출력 콜백 함수
GLvoid drawScene()
//--- 콜백 함수: 그리기 콜백 함수
{
	//--- 변경된 배경색 설정
	glClearColor(1.0, 1.0, 1.0, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//--- 렌더링 파이프라인에 세이더 불러오기
	glUseProgram(shaderProgramID);
	for (int i = 0; i < 4; i++)
	{
		tri[i].Draw();
	}
	glutSwapBuffers(); //--- 화면에 출력하
}

//--- 다시그리기 콜백 함수
GLvoid Reshape(int w, int h)
//--- 콜백 함수: 다시 그리기 콜백 함수
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