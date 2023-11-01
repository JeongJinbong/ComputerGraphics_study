#include "std.h"
#include <random>
using namespace std;

//-----------메인함수

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
GLuint shaderProgramID; //--- 세이더 프로그램 이름
GLuint vertexShader;	//--- 버텍스 세이더 객체
GLuint fragmentShader;	//--- 프래그먼트 세이더 객체
GLchar* vertexSource, * fragmentSource; //--- 소스코드 저장 변수

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
		qobj = gluNewQuadric(); // 객체 생성하기

	}

	void Draw()
	{
		gluQuadricDrawStyle(qobj, Fill ? GLU_FILL : GLU_LINE); // 도형 스타일
		gluSphere(qobj, 0.25, 50, 50); // 객체 만들기
	}

	void Set_Matrix()
	{
		Tx = glm::translate(Tx, glm::vec3(xMove, yMove, zMove));
		Rx = glm::rotate(Rx, glm::radians(30.0f + xRotate), glm::vec3(1.0, 0.0, 0.0));
		Ry = glm::rotate(Ry, glm::radians(-30.0f + yRotate), glm::vec3(0.0, 1.0, 0.0));
		SRx = glm::rotate(SRx, glm::radians(self_xRotate), glm::vec3(1.0, 0.0, 0.0));
		SRy = glm::rotate(SRy, glm::radians(self_yRotate), glm::vec3(0.0, 1.0, 0.0));
		TR = (Rx * Ry) * (Tx * SRx * SRy);

		unsigned int modelLocation = glGetUniformLocation(shaderProgramID, "Transform"); // 버텍스 세이더에서 모델링 변환 위치 가져오기
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
		qobj = gluNewQuadric(); // 객체 생성하기

	}

	void Set_Matrix()
	{
		Tx = glm::translate(Tx, glm::vec3(xMove, yMove, zMove));
		Rx = glm::rotate(Rx, glm::radians(30.0f + xRotate), glm::vec3(1.0, 0.0, 0.0));
		Ry = glm::rotate(Ry, glm::radians(-30.0f + yRotate), glm::vec3(0.0, 1.0, 0.0));
		SRx = glm::rotate(SRx, glm::radians(self_xRotate), glm::vec3(1.0, 0.0, 0.0));
		SRy = glm::rotate(SRy, glm::radians(self_yRotate), glm::vec3(0.0, 1.0, 0.0));
		TR = (Rx * Ry) * (Tx * SRx * SRy);

		unsigned int modelLocation = glGetUniformLocation(shaderProgramID, "Transform"); // 버텍스 세이더에서 모델링 변환 위치 가져오기
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR));
		glBindVertexArray(m_vao);
	}
	void Draw()
	{
		gluQuadricDrawStyle(qobj, Fill ? GLU_FILL : GLU_LINE); // 도형 스타일
		gluSphere(qobj, 0.125, 50, 50); // 객체 만들기
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
		qobj = gluNewQuadric(); // 객체 생성하기
		gluQuadricDrawStyle(qobj, Fill ? GLU_FILL : GLU_LINE); // 도형 스타일
		gluSphere(qobj, 0.25, 50, 50); // 객체 만들기
	}

	void Set_Matrix()
	{
		Tx = glm::translate(Tx, glm::vec3(xMove, yMove, zMove));
		Rx = glm::rotate(Rx, glm::radians(30.0f + xRotate), glm::vec3(1.0, 0.0, 0.0));
		Ry = glm::rotate(Ry, glm::radians(-30.0f + yRotate), glm::vec3(0.0, 1.0, 0.0));
		SRx = glm::rotate(SRx, glm::radians(30.0f + self_xRotate), glm::vec3(1.0, 0.0, 0.0));
		SRy = glm::rotate(SRy, glm::radians(-30.0f + self_yRotate), glm::vec3(0.0, 1.0, 0.0));
		TR = (Rx * Ry) * (Tx * SRx * SRy);

		unsigned int modelLocation = glGetUniformLocation(shaderProgramID, "Transform"); // 버텍스 세이더에서 모델링 변환 위치 가져오기
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
	glGenVertexArrays(1, &m_vao); //--- VAO 객체 생성, params(갯수, GLuint*)// n개를 받아도 생성되게 하려
	glBindVertexArray(m_vao); //--- VAO를 바인드하기

	// VBO 객체 생성
	glGenBuffers(1, &m_VBOvertex);
	//이런 vbo를 쓸거에요를 알려주고
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOvertex);
	//이런 점 데이터를 올릴거에요
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * 5, vertex, GL_STATIC_DRAW);
	int positionAttrib = glGetAttribLocation(shaderProgramID, "vPos");
	glEnableVertexAttribArray(positionAttrib);
	glVertexAttribPointer(positionAttrib, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

	// VBO 객체 생성
	glGenBuffers(1, &m_VBOcolor);
	//이런 vbo를 쓸거에요를 알려주고
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOcolor);
	//이런 점 데이터를 올릴거에요
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * 5, color, GL_STATIC_DRAW);
	//vertex shader에서 올릴 데이터 위치
	int vColorLocation = glGetAttribLocation(shaderProgramID, "vColor");
	//어떤 속성을 킬 것인지
	glEnableVertexAttribArray(vColorLocation);
	//어떤 속성에 어떤 데이터를 넘길 것인지
	glVertexAttribPointer(vColorLocation, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

}


void main(int argc, char** argv)	//--- 윈도우 출력하고 콜백함수 설정
{
	width = 800;
	height = 800;
	//--- 윈도우 생성하기
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
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
	glEnable(GL_DEPTH_TEST);

	InitBuffer();
	sun[0] = Sun();
	planet[0] = Planet();
	planet[1] = Planet();
	planet[2] = Planet();

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

	glEnable(GL_DEPTH_TEST);

	//--- 렌더링 파이프라인에 세이더 불러오기
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

	glutSwapBuffers(); //--- 화면에 출력하기
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

