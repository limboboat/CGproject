#include <cstdlib>
#include "Foothold.h"
#include"filetobuf.h"
#define pi 3.141592


GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);
//void Mouse(int button, int state, int x, int y);
//void MouseMove(GLint x, GLint y);
void Timerfunction(int value);
void make_vertexShader();
void make_fragmentShader();

using namespace std;

int g_window_w;
int g_window_h;

GLuint vao, vbo[2];
GLuint s_program;

GLchar* vertexsource, * fragmentsource; // �ҽ��ڵ� ���� ����
GLuint vertexshader, fragmentshader; // ���̴� ��ü
float theta = 0;
float cx = 1, cy = 1, cz = 1;

using namespace std;



GLfloat	box[][3] = {
	{ -0.5, 0, 0.5 },
	{ 0.5, 0, 0.5 },
	{ 0.5,1, 0.5 },	

	{ 0.5, 1, 0.5 },
	{ -0.5, 1, 0.5 },
	{ -0.5, 0, 0.5 }, // �ո�

	{ -0.5, 0, -0.5 },
	{ 0.5, 0, -0.5 },
	{ -0.5, 0, 0.5 },

	{ 0.5, 0, -0.5 },
	{ 0.5, 0, 0.5 },
	{ -0.5, 0, 0.5 }, // �ظ�

	{ -0.5,0, 0.5 }, 
	{ -0.5, 1, 0.5 },
	{ -0.5, 0, -0.5 },

	{ -0.5, 1, 0.5 },
	{ -0.5, 1, -0.5 },
	{ -0.5, 0, -0.5 }, //�޿��� (�ٶ󺸴±���)

	{ -0.5, 0, -0.5 }, 
	{ 0.5, 0, -0.5 },
	{ -0.5, 1, -0.5 },

	{ 0.5, 0, -0.5 },
	{ 0.5, 1, -0.5 },
	{ -0.5, 1, -0.5 }, //�޸�

	{ -0.5, 1, 0.5 },
	{ 0.5, 1, 0.5 },
	{ -0.5, 1, -0.5 },

	{ -0.5, 1, -0.5 },
	{ 0.5, 1, 0.5 },
	{ 0.5,1, -0.5 }, //����

	{ 0.5, 1, -0.5 },
	{ 0.5, 1, 0.5 },
	{ 0.5,0, -0.5 },

	{ 0.5, 1, 0.5 },
	{ 0.5, 0, 0.5 },
	{ 0.5, 0, -0.5 }, //��������
};
GLfloat	boxN[][3] = {
	 0.0f,  0.0f,  1.0f, 
	 0.0f,  0.0f,  1.0f,
	 0.0f,  0.0f,  1.0f,

	 0.0f,  0.0f,  1.0f,
	 0.0f,  0.0f,  1.0f,
	 0.0f,  0.0f,  1.0f, // �ո�

	  0.0f, -1.0f,  0.0f,
	  0.0f, -1.0f,  0.0f,
	  0.0f, -1.0f,  0.0f,

	  0.0f, -1.0f,  0.0f,
	  0.0f, -1.0f,  0.0f,
	  0.0f, -1.0f,  0.0f, // �ظ�

	  -1.0f,  0.0f,  0.0f,
	  -1.0f,  0.0f,  0.0f,
	  -1.0f,  0.0f,  0.0f,

	  -1.0f,  0.0f,  0.0f,
	  -1.0f,  0.0f,  0.0f,
	  -1.0f,  0.0f,  0.0f, //����

	  0.0f,  0.0f, -1.0f,
	  0.0f,  0.0f, -1.0f,
	  0.0f,  0.0f, -1.0f,

	  0.0f,  0.0f, -1.0f,
	  0.0f,  0.0f, -1.0f,
	  0.0f,  0.0f, -1.0f, //�޸�

	  0.0f,  1.0f,  0.0f,
	  0.0f,  1.0f,  0.0f,
	  0.0f,  1.0f,  0.0f,

	  0.0f,  1.0f,  0.0f,
	  0.0f,  1.0f,  0.0f,
	  0.0f,  1.0f,  0.0f, //����

	  1.0f,  0.0f,  0.0f,
	  1.0f,  0.0f,  0.0f,
	  1.0f,  0.0f,  0.0f,

	  1.0f,  0.0f,  0.0f,
	  1.0f,  0.0f,  0.0f,
	  1.0f,  0.0f,  0.0f,
};

std::vector<Foothold> Bottom;

void init()
{
	glEnable(GL_DEPTH_TEST);
}

void make_vertexShader()
{
	vertexsource = filetobuf("vertex.glsl");
	//--- ���ؽ� ���̴� ��ü �����
	vertexshader = glCreateShader(GL_VERTEX_SHADER);
	//--- ���̴� �ڵ带 ���̴� ��ü�� �ֱ�
	glShaderSource(vertexshader, 1, (const GLchar**)&vertexsource, 0);
	//--- ���ؽ� ���̴� �������ϱ�
	glCompileShader(vertexshader);
	//--- �������� ����� ���� ���� ���: ���� üũ
	checkCompileErrors(vertexshader, "vertex shader");
}

void make_fragmentShader()
{
	fragmentsource = filetobuf("fragment.glsl");
	//--- �����׸�Ʈ ���̴� ��ü �����
	fragmentshader = glCreateShader(GL_FRAGMENT_SHADER);
	//--- ���̴� �ڵ带 ���̴� ��ü�� �ֱ�
	glShaderSource(fragmentshader, 1, (const GLchar**)&fragmentsource, 0);
	//--- �����׸�Ʈ ���̴� ������
	glCompileShader(fragmentshader);
	//--- �������� ����� ���� ���� ���: ������ ���� üũ
	checkCompileErrors(fragmentshader, "fragment shader");
}

void InitBuffer()
{
	glGenVertexArrays(1, &vao); //--- VAO �� �����ϰ� �Ҵ��ϱ�
	glGenBuffers(2, vbo); //--- 2���� VBO�� �����ϰ� �Ҵ��ϱ�

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER,sizeof(box), box, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(boxN), boxN, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);
}

void InitShader()
{
	make_vertexShader();
	make_fragmentShader();
	//-- shader Program
	s_program = glCreateProgram();
	glAttachShader(s_program, vertexshader);
	glAttachShader(s_program, fragmentshader);
	glLinkProgram(s_program);
	checkCompileErrors(s_program, "PROGRAM");
	//--- ���̴� �����ϱ�
	glDeleteShader(vertexshader);
	glDeleteShader(fragmentshader);
	//--- Shader Program ����ϱ�
	glUseProgram(s_program);
}


void main(int argc, char** argv) //--- ������ ����ϰ� �ݹ��Լ� ����
{
	srand(time(0));

	//--- ������ �����ϱ�
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA|GLUT_DEPTH);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(600, 600);
	glutCreateWindow("�⸻");
	//--- GLEW �ʱ�ȭ�ϱ�
	glewExperimental = GL_TRUE;
	glewInit();
	InitShader();
	InitBuffer();

	init();
	MakeFoothold(Bottom);

	glutTimerFunc(100, Timerfunction, 1);

	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	glutMainLoop();
}

void Timerfunction(int value)
{
	switch (value) {
	case 1:
		/*		for (int i = 0; i < Bottom.size(); ++i) {
			//if() �浹üũ ������
				Bottom[i].Delete();
			if (Bottom[i].Del)
				Bottom.erase(Bottom.begin()+i);
		} */

		// delete test
		Bottom[0].Delete();
	if (Bottom[0].Del)
		Bottom.erase(Bottom.begin());

		glutPostRedisplay();
		glutTimerFunc(100, Timerfunction, 1);
		break;
	}
}

float radius = 5, camX=0,camY=0,camZ=0;
float lx = 3.0, ly = 2.0, lz = -1.5, ltheta=0; 
float Lx = 0, Ly = 0, Lz = 0;
float aml = 0.3f;

GLvoid drawScene() //--- �ݹ� �Լ�: �׸��� �ݹ� �Լ�
{
	//--- ����� ���� ����
	glClearColor(0.2, 0.2, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// ������ ���������ο� ���̴� �ҷ�����

	glUseProgram(s_program);

	GLUquadricObj * armL, * armR;
	armL = gluNewQuadric();
	armR = gluNewQuadric();

	glm::mat4 ptrans = glm::mat4(1.0f);
	glm::mat4 vtrans = glm::mat4(1.0f);

	Lx = (float)cos(ltheta / 180 * 3.141592) * lx;			// ���� ȸ��
	Ly = ly;
	Lz = (float)sin(ltheta / 180 * 3.141592) * (-lz);

	unsigned int lightPosLocation = glGetUniformLocation(s_program, "lightPos"); //--- lightPos �� ����: (0.0, 0.0, 5.0);
	glUniform3f(lightPosLocation, Lx,Ly,Lz);
	unsigned int lightColorLocation = glGetUniformLocation(s_program, "lightColor"); //--- lightColor �� ����: (1.0, 1.0, 1.0) ���
	glUniform3f(lightColorLocation, cx, cy, cz);
	unsigned int amlight = glGetUniformLocation(s_program, "ambientLight");
	glUniform1f(amlight, aml);
	

	unsigned int color_location = glGetUniformLocation(s_program, "objectColor");
	unsigned int model = glGetUniformLocation(s_program, "model");

	camX = (float)sin(theta / 180 * 3.141592) * radius;
	camY = +0.0;
	camZ = -1 * (float)cos(theta / 180 * 3.141592) * radius;
	//vtrans = glm::lookAt(glm::vec3(camX,camY,camZ), glm::vec3(X,Y,Z-5.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	vtrans = glm::lookAt(glm::vec3(0, 0, 0 + 5), glm::vec3(camX, camY, camZ), glm::vec3(0.0f, 1.0f, 0.0f));

	unsigned int view = glGetUniformLocation(s_program, "view");
	glUniformMatrix4fv(view, 1, GL_FALSE, &vtrans[0][0]);

	unsigned int projection = glGetUniformLocation(s_program, "projection");
	ptrans = glm::perspective(glm::radians(45.0f), (float)g_window_w / (float)g_window_h, 0.1f, 100.0f);
	ptrans = glm::translate(ptrans, glm::vec3(0, 0, -5.0));

	glUniformMatrix4fv(projection, 1, GL_FALSE, &ptrans[0][0]);

	glBindVertexArray(vao);
	for (int i = 0; i < Bottom.size(); ++i)
	{
		Bottom[i].Draw_Start();
		glUniform3f(color_location, Bottom[i].r, Bottom[i].g, Bottom[i].b);
		glUniformMatrix4fv(model, 1, GL_FALSE, glm::value_ptr(glm::mat4(Bottom[i].Drawing)));
		glDrawArrays(GL_TRIANGLES, 0, Bottom[i].size);
	}


	glutSwapBuffers();
}

GLvoid Reshape(int w, int h) //--- �ݹ� �Լ�: �ٽ� �׸��� �ݹ� �Լ�
{
	g_window_w = w;
	g_window_h = h;
	glViewport(0, 0, w, h);
}

GLvoid Keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'p':
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		break;
	case 'P':
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;
	case 'Q':
		exit(0);
		break;
	}
	InitBuffer();
	glutPostRedisplay();
}