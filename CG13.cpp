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
GLvoid Motion(int x, int y);

bool click = false;
bool PointClick = false;
bool PointClick2 = false;
bool PointClick3 = false;
bool PointClick4 = false;

GLint width, height;
GLuint shaderProgramID; //--- 세이더 프로그램 이름
GLuint vertexShader;	//--- 버텍스 세이더 객체
GLuint fragmentShader;	//--- 프래그먼트 세이더 객체
GLchar* vertexSource, * fragmentSource; //--- 소스코드 저장 변수

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
		glGenVertexArrays(1, &m_vao); //--- VAO 객체 생성, params(갯수, GLuint*)// n개를 받아도 생성되게 하려 
		glBindVertexArray(m_vao); //--- VAO를 바인드하기

		// VBO 객체 생성
		glGenBuffers(1, &m_VBOvertex);
		//이런 vbo를 쓸거에요를 알려주고
		glBindBuffer(GL_ARRAY_BUFFER, m_VBOvertex);
		//이런 점 데이터를 올릴거에요
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 18, vertex, GL_STATIC_DRAW);
		int positionAttrib = glGetAttribLocation(shaderProgramID, "vPos");
		glEnableVertexAttribArray(positionAttrib);
		glVertexAttribPointer(positionAttrib, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

		// VBO 객체 생성
		glGenBuffers(1, &m_VBOcolor);
		//이런 vbo를 쓸거에요를 알려주고
		glBindBuffer(GL_ARRAY_BUFFER, m_VBOcolor);
		//이런 점 데이터를 올릴거에요
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 18, color, GL_STATIC_DRAW);
		//vertex shader에서 올릴 데이터 위치
		int vColorLocation = glGetAttribLocation(shaderProgramID, "vColor");
		//어떤 속성을 킬 것인지
		glEnableVertexAttribArray(vColorLocation);
		//어떤 속성에 어떤 데이터를 넘길 것인지
		glVertexAttribPointer(vColorLocation, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

	}
	Rect(int x, int y)		//생성자
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

void main(int argc, char** argv)	//--- 윈도우 출력하고 콜백함수 설정
{
	width = 800;
	height = 600;
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
	glutMotionFunc(Motion);
	rect[0] = Rect(400, 300);
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

GLvoid drawScene()
//--- 콜백 함수: 그리기 콜백 함수
{
	//--- 변경된 배경색 설정
	glClearColor(1.0, 1.0, 1.0, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//--- 렌더링 파이프라인에 세이더 불러오기
	glUseProgram(shaderProgramID);

	rect[0].Draw();
	
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
			std::cout << "클릭이 되었음" << endl;

			if (rect->vertex[0]-0.1f <X && rect->vertex[0]+0.1f >X && rect->vertex[1] - 0.1f <Y && rect->vertex[1]+ 0.1f >Y)
			{
				std::cout << "왼쪽 상단 꼭짓점 클릭이 되었음" << endl;

				PointClick = true;
			}
			else if (rect->vertex[3] - 0.1f < X && rect->vertex[3] + 0.1f > X && rect->vertex[4] - 0.1f < Y && rect->vertex[4] + 0.1f > Y)
			{
				std::cout << "오른쪽 상단 꼭짓점 클릭이 되었음" << endl;

				PointClick2 = true;
			}
			else if (rect->vertex[6] - 0.1f < X && rect->vertex[6] + 0.1f > X && rect->vertex[7] - 0.1f < Y && rect->vertex[7] + 0.1f > Y)
			{
				std::cout << "오른쪽 하단 꼭짓점 클릭이 되었음" << endl;

				PointClick3 = true;
			}
			else if (rect->vertex[12] - 0.1f < X && rect->vertex[12] + 0.1f > X && rect->vertex[13] - 0.1f < Y && rect->vertex[13] + 0.1f > Y)
			{
				std::cout << "왼쪽 하단 꼭짓점 클릭이 되었음" << endl;

				PointClick4 = true;
			}
			else
				click = true;
		}
	}

	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
		std::cout << "클릭 해제" << endl;
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