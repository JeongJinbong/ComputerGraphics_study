#include "std.h"
#include <random>
using namespace std;

//-----------메인함수

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


const float vertex[] = 
	{0.0, 0.0, 0.0,
	 0.5, 0.5, 0.0,
	0.5,0.0, 0.0,

	0.0, 0.0 , 0.0,
	0.0, 0.5, 0.0,
	0.5,0.5, 0.0,		// 앞면

	0.0, 0.0 ,0.0,
	0.0,0.5,0.5,
	0.0,0.5,0.0,

	0.0,0.0,0.0,
	0.0,0.0,0.5,
	0.0,0.5,0.5,		//좌측 옆면

	0.0,0.5,0.0,
	0.5,0.5,0.5,
	0.5,0.5,0.0,

	0.0,0.5,0.0,
	0.0,0.5,0.5,
	0.5,0.5,0.5,		//윗면

	0.5,0.0,0.0,
	0.5,0.5,0.0,
	0.5,0.5,0.5,

	0.5,0.0,0.0,
	0.5,0.5,0.5,
	0.5,0.0,0.5,		// 우측 옆면

	0.0,0.0,0.0,
	0.5,0.0,0.0,
	0.5,0.0,0.5,

	0.0,0.0,0.0,
	0.5,0.0,0.5,
	0.0,0.0,0.5,		// 밑면

	0.0,0.0,0.5,
	0.5,0.0,0.5,
	0.5,0.5,0.5,

	0.0,0.0,0.5,
	0.5,0.5,0.5,
	0.0,0.5,0.5,			//뒷면

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
	glGenVertexArrays(1, &m_vao); //--- VAO 객체 생성, params(갯수, GLuint*)// n개를 받아도 생성되게 하려
	glBindVertexArray(m_vao); //--- VAO를 바인드하기

	// VBO 객체 생성
	glGenBuffers(1, &m_VBOvertex);
	//이런 vbo를 쓸거에요를 알려주고
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOvertex);
	//이런 점 데이터를 올릴거에요
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 18*8, vertex, GL_STATIC_DRAW);
	int positionAttrib = glGetAttribLocation(shaderProgramID, "vPos");
	glEnableVertexAttribArray(positionAttrib);
	glVertexAttribPointer(positionAttrib, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

	// VBO 객체 생성
	glGenBuffers(1, &m_VBOcolor);
	//이런 vbo를 쓸거에요를 알려주고
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOcolor);
	//이런 점 데이터를 올릴거에요
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 18*8, color, GL_STATIC_DRAW);
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
	glm::mat4 Rx = glm::mat4(1.0f);
	glm::mat4 Ry = glm::mat4(1.0f);
	glm::mat4 Tx = glm::mat4(1.0f);
	glm::mat4 TR = glm::mat4(1.0f);

	Tx = glm::translate(Tx, glm::vec3(0.0, 0.0, 0.0));
	Rx = glm::rotate(Rx, glm::radians(10.0f), glm::vec3(1.0, 0.0, 0.0));
	Ry = glm::rotate(Rx, glm::radians(10.0f), glm::vec3(0.0, 1.0, 0.0));

	TR = Rx * Ry * Tx;

	unsigned int modelLocation = glGetUniformLocation(shaderProgramID, "Transform"); // 버텍스 세이더에서 모델링 변환 위치 가져오기
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR));
	glBindVertexArray(m_vao);
	if (tetrahedron==false)
		glDrawArrays(GL_TRIANGLES, index, 6*cnt);
	if(tetrahedron== true)
		glDrawArrays(GL_TRIANGLES, 6*6 + index, 3 * cnt);
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
	case '1':
		tetrahedron = false;
		std::cout << "1 입력됨" << endl;
		index = 0;
		cnt = 1;
		break;
	case '2':
		tetrahedron = false;
		std::cout << "2 입력됨" << endl;
		index = 6*1;
		cnt = 1;
		break;
	case '3':
		tetrahedron = false;
		std::cout << "3 입력됨" << endl;
		index = 6 * 2;
		cnt = 1;
		break;
	case '4':
		tetrahedron = false;
		std::cout << "4 입력됨" << endl;
		index = 6 * 3;
		cnt = 1;
		break;
	case '5':
		tetrahedron = false;
		std::cout << "5 입력됨" << endl;
		index = 6 * 4;
		cnt = 1;
		break;
	case '6':
		tetrahedron = false;
		std::cout << "6 입력됨" << endl;
		index = 6 * 5;
		cnt = 1;
		break;
	case 'c':
		tetrahedron = false;
		index = 6 * pos(rd);
		cnt = 2;
		break;
	case '7':
		std::cout << "7 입력됨" << endl;
		tetrahedron = true;
		index = 3 * 0;
		cnt = 1;
		break;
	case '8':
		std::cout << "8 입력됨" << endl;
		tetrahedron = true;
		index = 3 * 1;
		cnt = 1;
		break;
	case '9':
		std::cout << "9 입력됨" << endl;
		tetrahedron = true;
		index = 3 * 2;
		cnt = 1;
		break;
	case '0':
		std::cout << "0 입력됨" << endl;
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

