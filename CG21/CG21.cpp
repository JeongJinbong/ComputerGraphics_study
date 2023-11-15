#include "std.h"
#include <random>
using namespace std;

//-----------메인함수

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
GLuint shaderProgramID; //--- 세이더 프로그램 이름
GLuint vertexShader;	//--- 버텍스 세이더 객체
GLuint fragmentShader;	//--- 프래그먼트 세이더 객체
GLchar* vertexSource, * fragmentSource; //--- 소스코드 저장 변수

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
-2.0f,0.0f,0.0f,        // x축

 0.0f,0.0f,0.0f,
0.0f,2.0f,0.0f,

0.0f,0.0f,0.0f,
0.0f,-2.0f,0.0f,        // y축

 0.0f,0.0f,0.0f,
0.0f,0.0f,2.0f,

0.0f,0.0f,0.0f,
0.0f,0.0f,-2.0f,       //z축

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
-0.500000, 0.500000, -0.500000,     // 앞면

-0.500000, -0.500000, 0.500000,
-0.500000, 0.500000, 0.500000,
-0.500000, 0.500000, -0.500000,

 -0.500000, -0.500000, 0.500000,
-0.500000, 0.500000, -0.500000,
-0.500000, -0.500000, -0.500000,		//좌측 옆면

0.500000, -0.500000, 0.500000,
0.500000, 0.500000, 0.500000,
-0.500000, -0.500000, 0.500000,

 0.500000, 0.500000, 0.500000,
 -0.500000, 0.500000, 0.500000,
-0.500000, -0.500000, 0.500000,		//윗면

 0.500000, -0.500000, -0.500000,
0.500000, 0.500000, -0.500000,
0.500000, -0.500000, 0.500000,

0.500000, 0.500000, -0.500000,
0.500000, 0.500000, 0.500000,
0.500000, -0.500000, 0.500000,		// 우측 옆면

0.500000, 0.500000, -0.500000,
 -0.500000, 0.500000, -0.500000,
0.500000, 0.500000, 0.500000,

-0.500000, 0.500000, -0.500000,
 -0.500000, 0.500000, 0.500000,
0.500000, 0.500000, 0.500000,		// 밑면

0.500000, -0.500000, -0.500000,
0.500000, -0.500000, 0.500000,
-0.500000, -0.500000, 0.500000,

0.500000, -0.500000, -0.500000,
-0.500000, -0.500000, 0.500000,
-0.500000, -0.500000, -0.500000,			//뒷면
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
	glGenVertexArrays(1, &m_vao); //--- VAO 객체 생성, params(갯수, GLuint*)// n개를 받아도 생성되게 하려
	glBindVertexArray(m_vao); //--- VAO를 바인드하기

	// VBO 객체 생성
	glGenBuffers(1, &m_VBOvertex);
	//이런 vbo를 쓸거에요를 알려주고
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOvertex);
	//이런 점 데이터를 올릴거에요
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 18 * 6, vertex, GL_STATIC_DRAW);
	int positionAttrib = glGetAttribLocation(shaderProgramID, "vPos");
	glEnableVertexAttribArray(positionAttrib);
	glVertexAttribPointer(positionAttrib, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

	// VBO 객체 생성
	glGenBuffers(1, &m_VBOcolor);
	//이런 vbo를 쓸거에요를 알려주고
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOcolor);
	//이런 점 데이터를 올릴거에요
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 18 * 6, color, GL_STATIC_DRAW);
	//vertex shader에서 올릴 데이터 위치
	int vColorLocation = glGetAttribLocation(shaderProgramID, "vColor");
	//어떤 속성을 킬 것인지
	glEnableVertexAttribArray(vColorLocation);
	//어떤 속성에 어떤 데이터를 넘길 것인지
	glVertexAttribPointer(vColorLocation, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

	glGenVertexArrays(1, &m_vao2); //--- VAO 객체 생성, params(갯수, GLuint*)// n개를 받아도 생성되게 하려
	glBindVertexArray(m_vao2); //--- VAO를 바인드하기

	// VBO 객체 생성
	glGenBuffers(1, &m_Axixvertex);
	//이런 vbo를 쓸거에요를 알려주고
	glBindBuffer(GL_ARRAY_BUFFER, m_Axixvertex);
	//이런 점 데이터를 올릴거에요
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * 18, axix, GL_STATIC_DRAW);
	int axixLocation = glGetAttribLocation(shaderProgramID, "vPos");
	glEnableVertexAttribArray(axixLocation);
	glVertexAttribPointer(axixLocation, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

	// VBO 객체 생성
	glGenBuffers(1, &m_Axixcolor);
	//이런 vbo를 쓸거에요를 알려주고
	glBindBuffer(GL_ARRAY_BUFFER, m_Axixcolor);
	//이런 점 데이터를 올릴거에요
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * 18, axix_color, GL_STATIC_DRAW);
	//vertex shader에서 올릴 데이터 위치
	int axixColor = glGetAttribLocation(shaderProgramID, "vColor");
	//어떤 속성을 킬 것인지
	glEnableVertexAttribArray(axixColor);
	//어떤 속성에 어떤 데이터를 넘길 것인지
	glVertexAttribPointer(axixColor, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

	glEnable(GL_DEPTH_TEST);
}


void main(int argc, char** argv)	//--- 윈도우 출력하고 콜백함수 설정
{
	width = 1024;
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
	glutDisplayFunc(MyDisplay);
	glutReshapeFunc(Reshape);
	glutMouseFunc(Mouse);
	glutKeyboardFunc(Keyboard);
	glutMotionFunc(Motion);

	InitBuffer();

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
	glUseProgram(shaderProgramID);

	//--- 변경된 배경색 설정
	//--- 렌더링 파이프라인에 세이더 불러오기

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

	unsigned int modelLocation = glGetUniformLocation(shaderProgramID, "Transform"); // 버텍스 세이더에서 모델링 변환 위치 가져오기
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
	projection = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 50.0f); //--- 투영 공간 설정: fovy, aspect, near, far
	projection = glm::translate(projection, glm::vec3(0.0, 0.0, -5.0)); //--- 공간을 z축 이동
	unsigned int projectionLocation = glGetUniformLocation(shaderProgramID, "projection");
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);

	glm::vec3 cameraPos = glm::vec3(cameraPosX, cameraPosY, cameraPosZ); //--- 카메라 위치
	glm::vec3 cameraDirection = glm::vec3(0.0f, 0.0f, 0.0f); //--- 카메라 바라보는 방향
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f); //--- 카메라 위쪽 방향
	glm::mat4 view = glm::mat4(1.0f);

	view = glm::lookAt(cameraPos, cameraDirection, cameraUp);
	view = glm::rotate(view, glm::radians(cameraRotation), glm::vec3(0.0, 1.0, 0.0));
	unsigned int viewLocation = glGetUniformLocation(shaderProgramID, "viewTransform"); //--- 뷰잉 변환 설정
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);

	drawScene();

	glViewport(width/2 ,0 , width/2, height/2);

	glm::vec3 cameraPos2 = glm::vec3(0.0f, 0.0f, 1.0f); //--- 카메라 위치
	view = glm::lookAt(cameraPos2, cameraDirection, cameraUp);
	view = glm::rotate(view, glm::radians(cameraRotation), glm::vec3(0.0, 1.0, 0.0));
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);
	projection = glm::ortho(-2.0f, 2.0f, -2.0f, 2.0f, -2.0f, 200.0f); //--- 투영 공간 설정 
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);
	drawScene();


	glViewport(width / 2, height/2, width / 2, height / 2);

	glm::vec3 cameraPos3 = glm::vec3(0.0f, 3.0f, 0.0f); //--- 카메라 위치
	cameraUp = glm::vec3(0.0f, 0.0f, 1.0f);
	view = glm::lookAt(cameraPos3, cameraDirection, cameraUp);
	view = glm::rotate(view, glm::radians(cameraRotation), glm::vec3(0.0, 1.0, 0.0));
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);
	projection = glm::ortho(-2.0f, 2.0f, -2.0f, 2.0f, -2.0f, 200.0f); //--- 투영 공간 설정 
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);
	drawScene();

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



