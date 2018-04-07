/*****************************************************************************
FILE : submit.c (csc3260 2012-2013 Assignment 1)
NOTE : you have to implement functions in this file
*****************************************************************************/
/*****************************************************************************
Student Information
Student ID:			061600223
Student Name:		李嘉玮
*****************************************************************************/

#include <cstdlib>
#include <cmath>
#include <cstdio>
#include <GL/glut.h>
#include <Windows.h>

const int chaeckImageWidth = 64;
const int checkImageHeight = 64;
static GLubyte ImageFromTXT[checkImageHeight][chaeckImageWidth][4];	/* RGBA */
static GLuint tex_flower, tex_window, tex_penguin;					/* 三个纹理 */
extern GLubyte flower[checkImageHeight][chaeckImageWidth][4];
extern GLubyte window[checkImageHeight][chaeckImageWidth][4];
extern GLubyte penguin[checkImageHeight][chaeckImageWidth][4];

struct Point		// 定义三维的点类结构体
{
	double x, y, z;

	Point operator-(const Point& a)
	{
		Point r = *this;
		r.x -= a.x;
		r.y -= a.y;
		r.z -= a.z;
		return r;
	}
	Point operator+(const Point& a)
	{
		Point r = *this;
		r.x += a.x;
		r.y += a.y;
		r.z += a.z;
		return r;
	}
	Point& operator+=(const Point& a)
	{
		x += a.x;
		y += a.y;
		z += a.z;
		return *this;
	}
	Point& operator-=(const Point& a)
	{
		x -= a.x;
		y -= a.y;
		z -= a.z;
		return *this;
	}
	Point operator*(const double& m)
	{
		Point r = *this;
		r.x *= m;
		r.y *= m;
		r.z *= m;
		return r;
	}
};
typedef Point Vec;

/* camera */
static Point cameraPos = { 0.0, -5.0,  15.0 };
static Vec cameraFront = { 0.0, 0.0, -1.0 };			// 看向的方向
static Vec cameraUp = { 0.0, 1.0,  0.0 };
static float cameraSpeed = 0.5f;						// speed of movement
Vec myCross(const Point& a, const Point& b);
inline double radians(float angle)
{
	return angle / 360.0 * 2 * 3.14159;
}
float fovy = 60.0;
float yaw = -90.0f;
float pitch = 0.0f;

/* 材料 */
GLfloat mat_amb[] = { 0.2,0.2,0.2,1.0 };
GLfloat mat_dif[] = { 0.8,0.8,0.8,1.0 };
GLfloat mat_amb_dif[] = { 0.5,0.5,0.5,1.0 };
GLfloat mat_specular[] = { 0.0,0.0,0.0,1.0 };			// 镜面光
GLfloat mat_shininess[] = { 50.0 };						// 反射亮度

/* 光源 */
GLfloat light1_pos[] = { 0, 0, 0, 1.0 };				/* 聚光灯 跟随视图移动 */
GLfloat light1_amb[] = { 0.2, 0.2, 0.2, 1.0 };				// 环境光
GLfloat light1_dif[] = { 1.0, 1.0, 1.0, 1.0 };				// 散射光
GLfloat light1_spc[] = { 0.0, 0.0, 0.0, 1.0 };				// 镜面光
GLfloat light1_dir[] = { 0, 0, -1 };						// 聚光灯朝向
GLfloat light1_cutoff = 20;									// 聚光灯切角
GLfloat light1_exp = 500.0;									// 聚集度 or 强度?

GLfloat light2_pos[] = { 0, 0, 0, 0.0 };				/* 吊灯-颜色2 */
GLfloat light2_amb[] = { 0.5, 0.5, 0.5, 1.0 };
GLfloat light2_dif[] = { 0.5, 0.5, 0.5, 1.0 };
GLfloat light2_spc[] = { 0.0, 0.0, 0.0, 1.0 };
GLfloat light2_dir[] = { 0, -1, 0 };
GLfloat light2_cutoff = 180;
GLfloat light2_exp = 30.0;

GLfloat light3_pos[] = { 0, 0, 0, 0 };					/* 吊灯-颜色3 */
GLfloat light3_amb[] = { 0.7, 0.7, 0.6, 1.0 };
GLfloat light3_dif[] = { 0.2, 0.2, 0.2, 1.0 };
GLfloat light3_spc[] = { 0.0, 0.0, 0.0, 1.0 };
GLfloat light3_dir[] = { 0, -1, 0 };
GLfloat light3_cutoff = 180;
GLfloat light3_exp = 100.0;

GLfloat lightS_pos[] = { 0, 0, 0, 0 };					/* sun-背景光 */
GLfloat lightS_amb[] = { 0.2, 0.2, 0.2, 1.0 };
GLfloat lightS_dif[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat lightS_spc[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat lightS_dir[] = { 0, 0, 0 };
GLfloat lightS_cutoff = 180;
GLfloat lightS_exp = 10.0;

/* 颜色 */
GLfloat mat_backWall[] = { 0xEB / 255.0, 0xE1 / 255.0, 0xB0 / 255.0 };
GLfloat mat_lrWall[] = { 0xD0 / 255.0,0xE4 / 255.0,0x69 / 255.0 };
GLfloat mat_roof[] = { 0xEC / 255.0, 0xED / 255.0, 0xDF / 255.0 };
GLfloat mat_floor[] = { 0xDD / 255.0, 0xC7 / 255.0, 0xA3 / 255.0 };
GLfloat mat_sun[] = { 1.0,0.5,0.5 };
GLfloat mat_earth[] = { 0.0,0.2,1.0 };
GLfloat mat_moon[] = { 1.0,1.0,1.0,1.0 };
GLfloat mat_red[] = { 1.0,0.0,0.0 };
GLfloat mat_blue[] = { 0.0,0.0,1.0 };
GLfloat mat_yellow[] = { 1.0,1.0,0.0 };
GLfloat mat_robot[] = { 1.0,0.5,0.5 };

/* room size */
float roomWidth = 20, roomHeight = 15, roomDepth = 30;

/* 模型移动 */
bool planetRun = true;
int year = 0, day = 0, hour = 0;
float sunSize = 1.0;
float penguinX = 3, penguinDir = -0.05;
int robotX = 7, robotY = -roomHeight + 3, robotZ = -2;

/* 函数声明 - 模型视图 */
void lookAt(const Point& cameraPos, const Vec& cameraOrient, const Vec& cameraUp);		// 设置相机位置
void drawRoom(int width, int height, int depth);										// 画一个屋子(相当于画一个立方体)
void drawWirePlanets(int x, int y, int z);												// 画三个线框球，可以控制旋转 (作为行星模型)
void drawRobot(int x, int y, int z, int ang);											// 画一个 Panda 
void drawLight(int x, int y, int z);
void drawPainting(float x, float y, float z, GLuint texName, int size);					// 纹理名为 texName

/* 纹理 */
//void getImage(const char* fileName);							// 从文件中生成纹理
void getImage(GLubyte picture[checkImageHeight][chaeckImageWidth][4]);

/* 窗口信息 */
int glutSizeWidth = 800, glutSizeHeight = 600;
int glutPosWidth = 100, glutPosHeight = 100;
int glutMiddleX = glutSizeWidth / 2 + glutPosWidth, glutMiddleY = glutSizeHeight / 2 + glutPosHeight;
bool isFullScreen = false;
float sensitivity = 0.15;
int lastX = glutSizeWidth / 2, lastY = glutSizeHeight / 2;

/* 函数声明 - 回调函数 */
void init();
void display();
void reshape(int w, int h);
void keyboard(unsigned char key, int x, int y);
void idel();
void specialKeyboard(int key, int x, int y);
void mouse(int button, int state, int x, int y);
void motion(int x, int y);


/* 主函数 */
int main(int argc, char** argv)
{
	/*Initialization of GLUT Library */
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	/*Create a window with title specified */
	glutInitWindowSize(glutSizeWidth, glutSizeHeight);
	glutInitWindowPosition(glutPosWidth, glutPosHeight);
	glutCreateWindow("061600223"); 
	//glutFullScreen();
	glutSetCursor(GLUT_CURSOR_NONE);	/* 隐藏鼠标 */
	SetCursorPos(glutSizeWidth / 2 + glutPosWidth, glutSizeHeight / 2 + glutPosHeight);			/* 鼠标位置 */

	/*not GLUT call, initialize several parameters */
	init();
	
	/*Register different CALLBACK function for GLUT to response
	with different events, e.g. window sizing, mouse click or
	keyboard stroke */
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutIdleFunc(idel);
	glutSpecialFunc(specialKeyboard);
	glutMouseFunc(mouse);
	glutPassiveMotionFunc(motion);
	
	/*Enter the GLUT event processing loop which never returns.
	it will call different registered CALLBACK according
	to different events. */
	glutMainLoop();

	return 0;
}


/* 回调函数 */
void init(void) // All Setup For OpenGL Goes Here
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);		// 选择深蓝绿色作为清空屏幕的颜色
	glShadeModel(GL_FLAT);
	glEnable(GL_DEPTH_TEST);

	/* 光照设置 */
	/* light1 - 聚光灯 */
	glLightfv(GL_LIGHT1, GL_AMBIENT, light1_amb);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light1_spc);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_dif);
	glLightfv(GL_LIGHT1, GL_POSITION, light1_pos);
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, light1_cutoff);
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, light1_exp);
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, light1_dir);

	///* light2 - 吊灯 */
	glLightfv(GL_LIGHT2, GL_AMBIENT, light2_amb);
	glLightfv(GL_LIGHT2, GL_SPECULAR, light2_spc);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, light2_dif);
	glLightfv(GL_LIGHT2, GL_POSITION, light2_pos);
	glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, light2_cutoff);
	glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, light2_exp);
	glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, light2_dir);

	/* light3 - 吊灯 */
	glLightfv(GL_LIGHT3, GL_AMBIENT, light3_amb);
	glLightfv(GL_LIGHT3, GL_SPECULAR, light3_spc);
	glLightfv(GL_LIGHT3, GL_DIFFUSE, light3_dif);
	glLightfv(GL_LIGHT3, GL_POSITION, light3_pos);
	glLightf(GL_LIGHT3, GL_SPOT_CUTOFF, light3_cutoff);
	glLightf(GL_LIGHT3, GL_SPOT_EXPONENT, light3_exp);
	glLightfv(GL_LIGHT3, GL_SPOT_DIRECTION, light3_dir);

	/* sun - 背景光 */
	glLightfv(GL_LIGHT4, GL_AMBIENT, lightS_amb);
	glLightfv(GL_LIGHT4, GL_SPECULAR, lightS_spc);
	glLightfv(GL_LIGHT4, GL_DIFFUSE, lightS_dif);
	glLightfv(GL_LIGHT4, GL_POSITION, lightS_pos);
	glLightf(GL_LIGHT4, GL_SPOT_CUTOFF, lightS_cutoff);
	glLightf(GL_LIGHT4, GL_SPOT_EXPONENT, lightS_exp);
	glLightfv(GL_LIGHT4, GL_SPOT_DIRECTION, lightS_dir);

	/* 材料 */
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, mat_amb_dif);

	glEnable(GL_LIGHTING);
	//glEnable(GL_LIGHT1);
	//glEnable(GL_LIGHT2);
	glEnable(GL_LIGHT3);
	//glEnable(GL_LIGHT4);

	/* 纹理 */
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	/* flower */
	//getImage("flower.txt");
	getImage(flower);
	glGenTextures(1, &tex_flower);
	glBindTexture(GL_TEXTURE_2D, tex_flower);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
		chaeckImageWidth, checkImageHeight, 0, GL_RGBA,
		GL_UNSIGNED_BYTE, ImageFromTXT);
	/*  window */
	//getImage("window.txt");
	getImage(window);
	glGenTextures(1, &tex_window);
	glBindTexture(GL_TEXTURE_2D, tex_window);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
		chaeckImageWidth, checkImageHeight, 0, GL_RGBA,
		GL_UNSIGNED_BYTE, ImageFromTXT);
	/* penguin */
	//getImage("penguin.txt");
	getImage(penguin);
	glGenTextures(1, &tex_penguin);
	glBindTexture(GL_TEXTURE_2D, tex_penguin);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
		chaeckImageWidth, checkImageHeight, 0, GL_RGBA,
		GL_UNSIGNED_BYTE, ImageFromTXT);

	/* 启用混合，可以显示透明 */
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void display(void) // Here's Where We Do All The Drawing
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);					/* 相机位置 (要先设置 Camera 位置，再绘制房间等，否则刚开启窗口时 Camera 位置不正确) */
	glLoadIdentity();
	lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

	// TODO:
	// Draw walls and objects here
	drawRoom(roomWidth, roomHeight, roomDepth);
	drawWirePlanets(0, -6, 0);
	drawLight(0, roomHeight, -3);

	glColor3f(1.0, 0.5, 0.5);					/* robot 模型*/
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_robot);
	drawRobot(robotX, robotY, robotZ, -45);
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_red);
	drawRobot(-4, -roomHeight + 3, -6, 45);
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_blue);
	drawRobot(-6, -roomHeight + 3, -9, 45);
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_yellow);
	drawRobot(-8, -roomHeight + 3, -6, 45);

	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_yellow);
	drawPainting(-roomWidth, -roomHeight, -roomDepth + 0.1, tex_window, 7);
	drawPainting(penguinX, -roomHeight, -roomDepth + 10, tex_penguin, 7);
	drawPainting(7, -roomHeight, -roomDepth + 15, tex_flower, 5);

	glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y) // Handle the keyboard events here
{
	switch (key)
	{
	case'\033'://press 'esc' to quit
		exit(0);
		break;
	case '\r':
		if (isFullScreen)
		{
			glutReshapeWindow(800, 600);
			glutPositionWindow(100, 100);
		}
		else
			glutFullScreen();
		isFullScreen ^= 1;
		break;

		// TODO:
		// Add keyboard control here
		/* 星球模型控制 */
	case '8':
		day = (day + 10) % 360;
		glutPostRedisplay();
		break;
	case '5':
		day = (day - 10) % 360;
		glutPostRedisplay();
		break;
	case '4':
		year = (year + 5) % 360;
		glutPostRedisplay();
		break;
	case '6':
		year = (year - 5) % 360;
		glutPostRedisplay();
		break;
	case '7':
		hour = (hour + 5) % 360;
		glutPostRedisplay();
		break;
	case '9':
		hour = (hour - 5) % 360;
		glutPostRedisplay();
		break;
	case '+':
		sunSize += 0.05;
		glutPostRedisplay();
		break;
	case '-':
		sunSize -= 0.05;
		glutPostRedisplay();
		break;
	case 'p':
		if (planetRun)
			glutIdleFunc(nullptr);
		else
			glutIdleFunc(idel);
		planetRun ^= 1;
		break;
		
		/* Camera 控制 */
	case 'w':
		cameraPos += cameraFront * cameraSpeed;
		glutPostRedisplay();
		break;
	case 's':
		cameraPos -= cameraFront * cameraSpeed;
		glutPostRedisplay();
		break;
	case 'a':
		cameraPos -= myCross(cameraFront, cameraUp) * cameraSpeed;		// 若 Front & Up 是单位向量 叉乘后依然为单位向量，可以不归一化
		glutPostRedisplay();
		break;
	case 'd':
		//cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;	/* 参考代码 */
		cameraPos += myCross(cameraFront, cameraUp) * cameraSpeed;
		glutPostRedisplay();
		break;
		
		/* 光源控制 */
	case '.':
		if (glIsEnabled(GL_LIGHTING))
			glDisable(GL_LIGHTING);
		else
			glEnable(GL_LIGHTING);
		glutPostRedisplay();
		break;
	case '0':
		if (glIsEnabled(GL_LIGHT1))
			glDisable(GL_LIGHT1);
		else
			glEnable(GL_LIGHT1);
		glutPostRedisplay();
		break;
	case '1':
		glDisable(GL_LIGHT3);
		glDisable(GL_LIGHT4);
		if (glIsEnabled(GL_LIGHT2))
			glDisable(GL_LIGHT2);
		else
			glEnable(GL_LIGHT2);
		glutPostRedisplay();
		break;
	case '2':
		glDisable(GL_LIGHT2);
		glDisable(GL_LIGHT4);
		if (glIsEnabled(GL_LIGHT3))
			glDisable(GL_LIGHT3);
		else
			glEnable(GL_LIGHT3);
		glutPostRedisplay();
		break;
	case '3':
		glDisable(GL_LIGHT2);
		glDisable(GL_LIGHT3);
		if (glIsEnabled(GL_LIGHT4))
			glDisable(GL_LIGHT4);
		else
			glEnable(GL_LIGHT4);
		glutPostRedisplay();
		break;
	default:
		break;
	}
}

void specialKeyboard(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_UP:
		robotZ -= 1;
		break;
	case GLUT_KEY_DOWN:
		robotZ += 1;
		break;
	case GLUT_KEY_LEFT:
		robotX -= 1;
		break;
	case GLUT_KEY_RIGHT:
		robotX += 1;
		break;
	default:
		break;
	}
	glutPostRedisplay();
}

double fovyOffset = 5.0;
void mouse(int button, int state, int x, int y)
{
	if (state == GLUT_UP)
	{
		return;
	}

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	switch (button)
	{
	case GLUT_LEFT_BUTTON:
		fovy -= fovyOffset;
		break;
	case GLUT_RIGHT_BUTTON:
		fovy += fovyOffset;
		break;
	case GLUT_MIDDLE_BUTTON:
		fovy = 60.0;
		break;
	default:
		break;
	}
	if (fovy >= 80.0)
		fovy = 80.0;
	else if (fovy <= 20.0)
		fovy = 20.0;

	gluPerspective(fovy, (GLfloat)glutGet(GLUT_WINDOW_WIDTH) / (GLfloat)glutGet(GLUT_WINDOW_HEIGHT), 0.1, 500.0);
	glutPostRedisplay();
}

void motion(int x, int y)			/* 这里的 x 和 y 是相对 glut 窗口的位置 */
{
	float xoffset = x - lastX;
	float yoffset = lastY - y;
	
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;
	
	Vec front;
	front.x = cos(radians(yaw)) * cos(radians(pitch));
	front.y = sin(radians(pitch));
	front.z = sin(radians(yaw)) * cos(radians(pitch));
	cameraFront = front;

	lastX = glutSizeWidth / 2;
	lastY = glutSizeHeight / 2;

	SetCursorPos(glutMiddleX, glutMiddleY);
}

void reshape(int w, int h) // Resize the GL Window. w=width, h=height
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fovy, (GLfloat)w / (GLfloat)h, 0.1, 500.0);

	/* 获取窗口位置信息 */
	glutSizeWidth = w;
	glutSizeHeight = h;
	glutPosWidth = glutGet(GLUT_WINDOW_X);
	glutPosHeight = glutGet(GLUT_WINDOW_Y);
	glutMiddleX = glutSizeWidth / 2 + glutPosWidth;
	glutMiddleY = glutSizeHeight / 2 + glutPosHeight;

	lastX = glutSizeWidth / 2;
	lastY = glutSizeHeight / 2;

	SetCursorPos(glutMiddleX, glutMiddleY);
}

int delay = 0;
void idel()
{
	if (delay % 200000 == 0)
	{
		penguinX += penguinDir;
		if (penguinX > roomWidth - 6 || penguinX < -roomWidth)
			penguinDir = -penguinDir;

		day = (day + 1) % 360;
		year = (year + 1) % 360;
		hour = (hour + 1) % 360;

		glutPostRedisplay();
	}
	++delay;
}

/* 自定义函数实现 */
Vec myCross(const Point & a, const Point & b)
{
	Point t;
	t.x = a.y*b.z - b.y*a.z;
	t.y = b.x*a.z - a.x*b.z;
	t.z = a.x*b.y - b.x*a.y;

	return t;
}

void lookAt(const Point& cameraPos, const Vec& cameraOrient, const Vec& cameraUp)
{
	gluLookAt(cameraPos.x, cameraPos.y, cameraPos.z, cameraOrient.x, cameraOrient.y, cameraOrient.z, cameraUp.x, cameraUp.y, cameraUp.z);
}

void drawRoom(int width, int heigth, int depth)
{
	/* 设置墙面材料 */
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);

	/* 逆时针放点 */
	glColor3ub(0xEB, 0xE1, 0xB0);					// 前后颜色
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_backWall);
	glBegin(GL_QUADS);					// 前墙
	glVertex3f(width, heigth, -depth);
	glVertex3f(-width, heigth, -depth);
	glVertex3f(-width, -heigth, -depth);
	glVertex3f(width, -heigth, -depth);
	glEnd();
	//glBegin(GL_QUADS);				// 后墙
	//	glVertex3f(x, y, z);
	//	glVertex3f(-x, y, z);
	//	glVertex3f(-x, -y, z);
	//	glVertex3f(x, -y, z);
	//glEnd();

	glColor3ub(0xD0, 0xE4, 0x69);						// 左右颜色
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_lrWall);
	glBegin(GL_QUADS);					// 左墙
	glVertex3f(-width, heigth, -depth);
	glVertex3f(-width, heigth, depth);
	glVertex3f(-width, -heigth, depth);
	glVertex3f(-width, -heigth, -depth);
	glEnd();
	glBegin(GL_QUADS);					// 右墙
	glVertex3f(width, heigth, -depth);
	glVertex3f(width, heigth, depth);
	glVertex3f(width, -heigth, depth);
	glVertex3f(width, -heigth, -depth);
	glEnd();

	glColor3ub(0xEC, 0xED, 0xDF);					// 屋顶颜色
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_roof);
	glBegin(GL_QUADS);					// 屋顶
	glVertex3f(width, heigth, -depth);
	glVertex3f(-width, heigth, -depth);
	glVertex3f(-width, heigth, depth);
	glVertex3f(width, heigth, depth);
	glEnd();
	glColor3ub(0xDD, 0xC7, 0xA3);					// 地板颜色
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_floor);
	glBegin(GL_QUADS);					// 地板
	glVertex3f(width, -heigth, -depth);
	glVertex3f(-width, -heigth, -depth);
	glVertex3f(-width, -heigth, depth);
	glVertex3f(width, -heigth, depth);
	glEnd();

}

void drawWirePlanets(int x, int y, int z)
{
	//glLoadIdentity();				// 使用的话会覆盖 lookat 的设置，应该使用 PushMatrix 保存位置

	lightS_pos[0] = x;
	lightS_pos[1] = y - 3;
	lightS_pos[2] = z;

	glPushMatrix();
	glTranslatef(x, y, z);
	glLightfv(GL_LIGHT4, GL_POSITION, lightS_pos);
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_sun);
	//glMaterialfv(GL_FRONT, GL_EMISSION, mat_sun_emission);		// 不好用
	glColor3ub(0xE8, 0x5A, 0x08);	// 太阳颜色
	glutWireSphere(sunSize, 15, 16);
	glRotatef((GLfloat)year, 0.0, 1.0, 0.0);
	glTranslatef(2.0, 0.0, 0.0);

	glPushMatrix();
	glRotatef((GLfloat)day, 0.0, 1.0, 0.0);
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_earth);
	glColor3ub(0x00, 0x7A, 0xCC);	// 地球颜色
	glutWireSphere(0.5, 10, 8);
	glPopMatrix();

	glRotatef((GLfloat)hour, 0.0, 1.0, 0.0);
	glTranslatef(1.0, 0.0, 0.0);
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_moon);
	glColor3ub(0xE9, 0xF2, 0xFB);	// 月亮颜色
	glutWireSphere(0.2, 10, 4);
	glPopMatrix();

}

void drawRobot(int x, int y, int z, int ang)
{
	// 使用二次曲面绘制一个简单模型
	GLUquadricObj *myRobot;
	myRobot = gluNewQuadric();
	gluQuadricNormals(myRobot, GLU_SMOOTH);

	glPushMatrix();
	glTranslatef(x, y, z);							// 模型位置
	glRotated(ang, 0, 1, 0);

	glPushMatrix();
	glRotatef(90, 1, 0, 0);				//身体
	gluCylinder(myRobot, 1, 1, 1.5, 16, 16);
	glPopMatrix();
	glPushMatrix();
	glRotatef(90, 1, 0, 0);				//手
	glTranslatef(1.5, 0, -0.3);
	glRotatef(-45, 0, 1, 1);
	gluCylinder(myRobot, 0.2, 0.2, 1, 16, 16);
	glPopMatrix();
	glPushMatrix();
	glRotatef(90, 1, 0, 0);				//手
	glTranslatef(-1.5, 0, -0.3);
	glRotatef(45, 0, 1, 1);
	gluCylinder(myRobot, 0.2, 0.2, 1, 16, 16);
	glPopMatrix();
	glPushMatrix();
	glRotatef(90, 1, 0, 0);				//脚
	glTranslatef(0.5, 0, 1.5);
	gluCylinder(myRobot, 0.2, 0.2, 0.5, 16, 16);
	glPopMatrix();
	glPushMatrix();
	glRotatef(90, 1, 0, 0);				//脚
	glTranslatef(-0.5, 0, 1.5);
	gluCylinder(myRobot, 0.2, 0.2, 0.5, 16, 16);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0.7, 1.2, 0);			//天线触角
	glRotatef(90, 3, -1.5, 0);
	gluCylinder(myRobot, 0.1, 0.1, 0.5, 16, 16);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(-0.7, 1.2, 0);			//天线触角
	glRotatef(90, 3, 1.5, 0);
	gluCylinder(myRobot, 0.1, 0.1, 0.5, 16, 16);
	glPopMatrix();
	glPushMatrix();
	glutSolidSphere(1, 16, 16);			//头
	glPopMatrix();

	glPopMatrix();

}

void drawLight(int x, int y, int z)
{
	light2_pos[0] = light3_pos[0] = x;
	light2_pos[1] = light3_pos[1] = y;
	light2_pos[2] = light3_pos[2] = z;

	glColor3ub(0xFF, 0xFF, 0xFF);
	glPushMatrix();
	glTranslatef(x, y, z);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_lrWall);
	glutSolidCube(2.0);
	glLightfv(GL_LIGHT2, GL_POSITION, light2_pos);
	glLightfv(GL_LIGHT3, GL_POSITION, light3_pos);
	glPopMatrix();
}

void drawPainting(float x, float y, float z, GLuint texName, int size)
{
	glEnable(GL_TEXTURE_2D);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glBindTexture(GL_TEXTURE_2D, texName);

	glColor3ub(0xFF, 0x00, 0x00);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0);	glVertex3f(x, y, z);
	glTexCoord2f(0.0, 1.0);glVertex3f(x, y + size, z);
	glTexCoord2f(1.0, 1.0);glVertex3f(x + size, y + size, z);
	glTexCoord2f(1.0, 0.0);	glVertex3f(x + size, y, z);
	glEnd();

	glDisable(GL_TEXTURE_2D);
}

/* 读取贴图数据 - 文件读取 */
//void getImage(const char* fileName)
//{
//	/* 读取 */
//	int p1, p2, p3, p4;
//	FILE* img;
//	fopen_s(&img, fileName, "r");
//	if (img == NULL)
//	{
//		printf_s("未找到%s\n请确保图像信息 txt 文件在 exe 当前目录下\n", fileName);
//		return;
//	}
//	for (int i = checkImageHeight - 1; i >= 0; --i)
//	{
//		for (int j = 0;j < chaeckImageWidth; ++j)
//		{
//			fscanf_s(img, "%d,%d,%d,%d", &p1, &p2, &p3, &p4);
//			ImageFromTXT[i][j][0] = (GLubyte)p1;
//			ImageFromTXT[i][j][1] = (GLubyte)p2;
//			ImageFromTXT[i][j][2] = (GLubyte)p3;
//			ImageFromTXT[i][j][3] = (GLubyte)p4;
//		}
//	}
//	fclose(img);
//}

/* 读取贴图数据 - 数组读取 */
// checkImageHeight	chaeckImageWidth
void getImage(GLubyte picture[checkImageHeight][chaeckImageWidth][4])
{
	for (int i = 0; i < checkImageHeight; ++i)
	{
		for (int j = 0;j < chaeckImageWidth; ++j)
		{
			ImageFromTXT[i][j][0] = picture[63 - i][j][0];
			ImageFromTXT[i][j][1] = picture[63 - i][j][1];
			ImageFromTXT[i][j][2] = picture[63 - i][j][2];
			ImageFromTXT[i][j][3] = picture[63 - i][j][3];
		}
	}	
}


/* 很长很长的数组 */
GLubyte flower[checkImageHeight][chaeckImageWidth][4] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,57,172,252,80,57,172,251,176,56,171,251,64,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,255,112,0,16,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,53,175,255,48,56,173,251,128,48,175,255,16,57,172,251,144,48,175,255,16,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,255,108,0,80,252,107,0,176,253,107,0,112,255,104,0,32,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,57,172,252,80,56,173,251,128,56,173,252,96,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,252,106,0,96,255,108,0,64,255,106,0,48,255,108,0,80,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,48,175,255,16,56,173,252,96,56,175,255,32,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,255,106,0,48,253,107,0,112,253,108,0,144,255,104,0,32,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,255,106,0,48,255,104,0,32,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,252,106,0,96,253,108,0,127,253,108,0,127,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,255,112,0,16,252,108,0,191,253,108,0,255,253,108,0,208,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,255,211,4,64,255,210,4,63,255,223,0,16,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,255,112,0,16,253,107,0,160,253,108,0,255,253,108,0,255,255,104,0,32,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,255,223,0,16,255,211,4,207,255,211,4,255,255,223,0,16,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,253,108,0,144,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,255,212,2,112,255,211,4,144,255,215,0,32,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,252,106,0,96,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,253,107,0,112,252,108,0,192,255,106,0,48,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,32,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,16,66,66,66,214,80,75,61,214,0,0,0,16,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,255,108,0,64,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,40,40,40,190,114,105,64,248,86,78,45,202,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,68,68,68,235,239,239,239,255,223,223,223,255,88,84,62,228,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,44,44,44,116,105,99,77,224,75,68,41,187,0,0,0,16,95,94,87,231,234,207,96,255,82,73,34,251,0,0,0,0,0,0,0,32,0,0,0,48,0,0,0,0,0,0,0,0,16,16,16,124,51,50,43,253,77,74,61,255,93,89,73,255,38,35,24,255,0,0,0,46,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,71,71,71,200,241,222,139,255,186,164,73,255,37,33,18,209,111,108,101,228,234,207,96,255,82,72,37,247,40,40,40,152,119,113,92,245,129,113,54,254,53,49,31,124,0,0,0,0,51,51,51,160,246,233,178,255,237,211,102,255,207,184,89,255,165,144,58,255,0,0,0,64,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,32,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,35,35,35,130,201,193,160,255,233,207,95,255,81,73,40,245,20,20,20,229,60,55,32,249,49,44,27,178,134,131,120,241,237,211,102,255,169,150,65,255,83,78,48,148,0,0,0,0,55,55,55,102,138,134,118,249,237,211,102,255,235,208,99,255,83,73,33,254,0,0,0,16,0,0,0,64,0,0,0,16,0,0,0,144,63,56,24,220,62,53,21,87,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,48,0,0,0,95,40,40,40,190,66,61,42,247,34,32,24,250,162,127,101,255,143,90,50,255,56,37,25,252,86,84,76,252,159,141,63,255,47,41,23,240,0,0,0,0,0,0,0,0,0,0,0,0,24,24,24,106,22,22,19,216,70,64,39,204,62,53,21,87,0,0,0,64,105,99,76,252,77,71,39,241,0,0,0,136,216,193,88,255,43,37,15,186,74,71,60,204,86,78,45,202,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,28,28,28,144,133,129,109,243,177,157,80,255,98,88,43,252,31,30,27,229,244,213,190,255,102,78,59,255,103,80,64,255,194,120,68,255,36,26,19,251,25,25,25,174,30,28,20,218,89,81,49,177,0,0,0,16,0,0,0,0,0,0,0,0,0,0,0,128,0,0,0,0,0,0,0,0,0,0,0,32,99,96,80,241,160,141,63,255,11,11,11,212,24,20,10,245,23,23,23,228,237,211,109,255,81,72,36,247,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,42,42,42,172,215,203,155,255,237,211,102,255,213,188,82,255,25,24,19,250,177,138,110,255,175,175,175,255,191,191,191,255,156,103,64,255,53,35,23,254,171,165,142,255,237,211,102,255,201,177,79,255,66,61,35,224,0,0,0,0,0,0,0,0,0,0,0,128,0,0,0,0,54,54,54,61,80,77,62,213,62,57,35,233,32,31,24,236,133,101,78,255,145,96,58,255,26,20,16,253,58,53,33,235,51,47,28,126,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,16,32,32,32,165,100,93,65,221,51,46,28,225,19,19,19,238,228,199,178,255,88,69,55,255,90,77,67,255,195,122,69,255,36,25,18,251,100,98,88,238,208,186,97,255,157,138,61,255,66,61,35,224,0,0,0,0,0,0,0,0,0,0,0,160,0,0,0,0,33,33,33,184,211,196,130,255,172,152,68,255,52,50,44,245,75,64,56,255,93,85,79,255,124,76,43,255,34,30,15,193,160,142,66,255,53,47,21,210,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,50,50,50,139,119,113,92,245,72,64,31,254,63,59,52,248,203,149,110,255,196,123,70,255,70,48,33,254,53,49,39,245,60,55,32,215,40,40,40,152,53,49,31,124,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,192,0,0,0,0,0,0,0,16,41,39,32,144,43,42,33,223,27,26,20,243,135,98,71,255,115,82,58,255,55,38,26,252,9,8,4,199,85,75,34,227,69,61,26,160,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,48,139,137,129,248,237,211,102,255,169,150,65,255,44,40,30,202,30,29,28,245,34,30,18,247,34,34,34,228,241,222,139,255,186,164,73,255,75,68,41,187,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,192,0,0,0,0,0,0,0,0,42,42,42,172,195,180,112,255,80,71,35,252,30,29,26,235,40,32,22,245,65,62,53,250,85,77,38,253,62,53,21,87,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,80,139,137,124,248,145,128,57,255,73,65,36,232,65,65,65,193,244,227,159,255,141,125,56,255,5,5,5,225,201,193,160,255,236,209,100,255,93,83,44,242,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,128,0,0,0,0,0,0,0,0,61,61,61,168,99,90,56,243,25,24,14,226,152,145,114,250,99,88,45,252,63,62,55,214,175,156,76,255,86,78,45,202,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,44,44,44,116,70,70,70,88,0,0,0,0,85,85,85,192,241,222,139,255,143,126,57,255,0,0,0,184,51,51,51,160,95,90,63,238,86,78,45,202,0,0,0,0,0,0,0,0,0,0,0,0,35,35,35,130,0,0,0,159,0,0,0,63,0,0,0,63,0,0,0,16,0,0,0,32,0,0,0,165,119,114,97,245,51,46,26,246,0,0,0,16,55,51,37,145,0,0,0,16,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,237,212,102,112,237,211,101,144,239,213,101,48,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,42,42,42,172,166,154,111,251,80,72,36,245,0,0,0,96,0,0,0,48,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,28,28,28,144,101,107,99,250,76,107,68,255,72,103,64,255,37,49,33,250,0,0,0,144,0,0,0,32,0,0,0,64,0,0,0,32,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,237,212,102,112,237,211,101,144,239,215,104,32,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,64,0,0,0,76,0,0,0,74,0,0,0,128,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,26,26,26,196,93,101,90,246,85,115,78,255,48,70,41,255,0,0,0,184,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,24,24,24,106,69,69,69,219,136,147,133,255,57,64,54,250,5,6,5,212,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,128,0,0,0,32,35,35,35,130,0,0,0,136,0,0,0,106,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,32,36,36,36,129,131,132,131,228,101,125,94,255,53,74,47,255,51,71,45,255,15,21,14,250,32,32,32,87,37,38,37,208,33,39,31,163,0,0,0,16,0,0,0,0,0,0,0,0,0,0,0,192,0,0,0,0,0,0,0,0,0,0,0,144,0,0,0,48,0,0,0,32,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,48,37,37,37,209,50,63,47,250,50,71,45,255,62,89,55,255,44,64,39,255,37,47,36,191,44,44,44,237,114,153,104,255,92,128,83,255,27,35,24,250,0,0,0,80,0,0,0,0,0,0,0,192,0,0,0,0,0,0,0,16,10,10,10,225,59,63,59,244,48,63,44,251,49,60,47,178,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,32,0,0,0,96,48,59,45,148,0,0,0,48,0,0,0,0,17,18,17,240,67,91,61,254,38,52,35,251,42,52,40,187,0,0,0,0,0,0,0,0,0,0,0,160,0,0,0,0,0,0,0,112,76,76,76,225,66,83,62,255,66,91,59,255,45,65,40,255,0,0,0,48,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,166,0,0,0,64,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,128,0,0,0,0,0,0,0,160,28,28,28,144,121,131,118,249,59,82,53,255,60,86,52,255,40,48,38,159,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,96,0,0,0,48,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,128,0,0,0,64,0,0,0,96,0,0,0,0,24,24,24,106,45,50,44,231,60,77,55,249,17,21,16,239,0,0,0,32,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,84,84,84,119,28,28,28,215,32,32,31,220,37,36,33,225,37,36,33,225,31,31,29,228,24,24,22,234,37,36,33,225,37,36,33,225,37,36,33,225,37,36,33,225,26,26,24,232,26,26,24,232,37,36,33,225,37,36,33,225,39,38,34,228,42,41,33,232,107,104,91,145,0,0,0,32,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,68,68,68,218,255,255,255,255,245,237,205,255,242,232,189,255,242,232,189,255,242,232,189,255,242,232,189,255,242,232,189,255,242,232,189,255,242,232,189,255,242,232,189,255,242,232,189,255,242,232,189,255,242,232,189,255,242,232,189,255,235,224,176,255,214,200,139,255,79,76,60,247,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,84,84,84,143,26,26,26,241,53,52,50,244,58,57,50,251,58,57,50,251,58,57,50,251,58,57,50,251,58,57,50,251,58,57,50,251,58,57,50,251,58,57,50,251,58,57,50,251,58,57,50,251,58,57,50,251,58,57,50,251,56,54,44,254,41,39,30,254,86,84,74,180,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,71,71,71,200,255,254,251,255,242,232,189,255,242,232,189,255,242,232,189,255,242,232,189,255,242,232,189,255,242,232,189,255,242,232,189,255,242,232,189,255,242,232,189,255,242,232,189,255,242,232,189,255,238,228,182,255,214,200,139,255,81,76,58,250,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,42,42,42,172,239,239,239,255,243,234,197,255,242,232,189,255,242,232,189,255,242,232,189,255,242,232,189,255,242,232,189,255,242,232,189,255,242,232,189,255,242,232,189,255,242,232,189,255,242,232,189,255,233,222,172,255,214,200,139,255,71,68,54,232,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,80,191,191,191,255,246,239,209,255,242,232,189,255,242,232,189,255,242,232,189,255,242,232,189,255,242,232,189,255,242,232,189,255,242,232,189,255,242,232,189,255,242,232,189,255,242,232,189,255,226,214,160,255,187,175,121,255,87,85,69,178,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,32,114,114,114,232,249,245,226,255,242,232,189,255,242,232,189,255,242,232,189,255,242,232,189,255,242,232,189,255,242,232,189,255,242,232,189,255,242,232,189,255,242,232,189,255,242,232,189,255,219,206,148,255,135,126,88,255,0,0,0,64,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,64,64,64,214,252,249,238,255,242,232,189,255,242,232,189,255,242,232,189,255,242,232,189,255,242,232,189,255,242,232,189,255,242,232,189,255,242,232,189,255,242,232,189,255,240,230,186,255,214,200,139,255,82,77,57,253,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,70,70,70,176,255,255,255,255,242,232,189,255,242,232,189,255,242,232,189,255,242,232,189,255,242,232,189,255,242,232,189,255,242,232,189,255,242,232,189,255,242,232,189,255,235,224,176,255,214,200,139,255,89,85,67,235,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,133,133,133,100,223,223,223,255,245,237,205,255,242,232,189,255,242,232,189,255,242,232,189,255,242,232,189,255,242,232,189,255,242,232,189,255,242,232,189,255,242,232,189,255,228,216,164,255,200,187,130,255,85,81,67,202,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,40,40,40,76,191,191,191,255,245,237,205,255,242,232,189,255,242,232,189,255,242,232,189,255,242,232,189,255,242,232,189,255,242,232,189,255,242,232,189,255,242,232,189,255,228,216,164,255,163,153,105,255,43,41,33,234,0,0,0,16,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,223,223,223,255,247,240,213,255,242,232,189,255,242,232,189,255,242,232,189,255,242,232,189,255,242,232,189,255,242,232,189,255,242,232,189,255,242,232,189,255,228,216,164,255,214,200,139,255,123,116,85,253,0,0,0,48,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,119,119,119,240,248,243,222,255,242,232,189,255,242,232,189,255,242,232,189,255,242,232,189,255,242,232,189,255,242,232,189,255,242,232,189,255,242,232,189,255,228,216,164,255,105,98,68,255,29,28,23,203,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,109,109,109,224,249,245,226,255,242,232,189,255,242,232,189,255,242,232,189,255,242,232,189,255,242,232,189,255,242,232,189,255,242,232,189,255,226,217,177,255,69,65,48,255,160,150,104,255,123,116,85,253,0,0,0,48,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,104,104,104,216,252,249,238,255,242,232,189,255,242,232,189,255,242,232,189,255,242,232,189,255,242,232,189,255,242,232,189,255,226,217,177,255,90,87,70,255,207,195,141,255,200,187,130,255,50,48,38,248,0,0,0,16,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,74,74,74,226,252,249,238,255,242,232,189,255,242,232,189,255,242,232,189,255,242,232,189,255,242,232,189,255,242,232,189,255,105,101,82,255,226,217,177,255,166,157,116,255,70,65,45,255,55,52,41,237,0,0,0,16,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,65,65,65,236,250,246,230,255,242,232,189,255,242,232,189,255,242,232,189,255,242,232,189,255,242,232,189,255,242,232,189,255,242,232,189,255,121,116,94,255,128,121,92,255,214,200,139,255,109,103,77,252,0,0,0,32,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,119,119,119,240,248,243,222,255,242,232,189,255,242,232,189,255,242,232,189,255,242,232,189,255,242,232,189,255,242,232,189,255,166,159,129,255,151,145,118,255,228,216,164,255,101,94,66,255,53,51,43,124,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,64,159,159,159,255,248,242,218,255,242,232,189,255,242,232,189,255,242,232,189,255,242,232,189,255,242,232,189,255,242,232,189,255,166,159,129,255,242,232,189,255,142,135,102,255,75,70,49,255,0,0,0,64,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,31,31,31,91,191,191,191,255,245,237,205,255,242,232,189,255,242,232,189,255,242,232,189,255,242,232,189,255,242,232,189,255,242,232,189,255,242,232,189,255,196,188,153,255,99,94,71,255,160,150,104,255,0,0,0,80,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,51,51,51,160,223,223,223,255,245,237,205,255,242,232,189,255,242,232,189,255,242,232,189,255,242,232,189,255,242,232,189,255,242,232,189,255,242,232,189,255,90,87,70,255,228,216,164,255,200,187,130,255,85,81,67,202,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,47,47,47,196,255,255,255,255,242,232,189,255,242,232,189,255,242,232,189,255,242,232,189,255,242,232,189,255,242,232,189,255,242,232,189,255,181,174,141,255,151,145,118,255,235,224,176,255,214,200,139,255,75,72,57,238,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,64,64,64,214,253,251,243,255,242,232,189,255,242,232,189,255,242,232,189,255,242,232,189,255,242,232,189,255,242,232,189,255,242,232,189,255,121,116,94,255,226,217,177,255,240,230,186,255,214,200,139,255,95,89,68,249,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,16,114,114,114,232,249,245,226,255,242,232,189,255,242,232,189,255,242,232,189,255,242,232,189,255,242,232,189,255,242,232,189,255,242,232,189,255,90,87,70,255,242,232,189,255,242,232,189,255,218,204,145,255,122,114,81,255,0,0,0,48,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,80,191,191,191,255,247,240,213,255,242,232,189,255,242,232,189,255,242,232,189,255,242,232,189,255,242,232,189,255,242,232,189,255,242,232,189,255,242,232,189,255,242,232,189,255,242,232,189,255,224,212,157,255,173,162,112,255,69,66,55,159,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,51,51,51,160,164,164,164,255,61,59,51,255,60,58,47,255,60,58,47,255,60,58,47,255,60,58,47,255,60,58,47,255,60,58,47,255,60,58,47,255,60,58,47,255,60,58,47,255,60,58,47,255,57,54,41,255,173,162,112,255,85,81,67,202,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,24,24,24,106,188,188,188,244,247,240,213,255,242,232,189,255,242,232,189,255,242,232,189,255,242,232,189,255,242,232,189,255,242,232,189,255,242,232,189,255,242,232,189,255,242,232,189,255,242,232,189,255,224,212,157,255,173,162,112,255,69,66,55,159,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,64,0,0,0,127,0,0,0,127,0,0,0,127,0,0,0,127,0,0,0,127,0,0,0,127,0,0,0,127,0,0,0,127,0,0,0,127,0,0,0,127,0,0,0,127,0,0,0,127,0,0,0,127,0,0,0,127,0,0,0,127,0,0,0,127,0,0,0,127,0,0,0,127,0,0,0,127,15,15,15,203,39,39,37,225,47,46,42,237,49,48,43,239,49,48,43,239,49,48,43,239,49,48,43,239,49,48,43,239,49,48,43,239,49,48,43,239,49,48,43,239,50,48,41,241,52,49,38,247,38,36,29,228,0,0,0,127,0,0,0,127,0,0,0,127,0,0,0,127,0,0,0,96,0,0,0,0,0,0,0,127,0,0,0,127,0,0,0,127,0,0,0,127,0,0,0,127,0,0,0,127,0,0,0,127,0,0,0,127,0,0,0,127,0,0,0,127,0,0,0,127,0,0,0,127,0,0,0,127,0,0,0,127,0,0,0,127,0,0,0,64,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, };
GLubyte window[checkImageHeight][chaeckImageWidth][4] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,16,0,0,0,63,0,0,0,127,0,0,0,191,7,7,7,215,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,4,4,4,228,0,0,0,191,0,0,0,127,0,0,0,79,0,0,0,16,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,64,0,0,0,159,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,51,51,51,255,51,51,51,255,51,51,51,255,51,51,51,255,12,12,12,255,0,0,0,255,0,0,0,255,0,0,0,255,1,1,1,240,0,0,0,159,0,0,0,64,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,64,0,0,0,223,0,0,0,255,0,0,0,255,25,25,25,255,102,102,102,255,156,156,156,255,201,201,201,255,212,212,212,255,215,215,215,255,216,216,216,255,216,216,216,255,215,215,215,255,212,212,212,255,211,211,211,255,153,153,153,255,89,89,89,255,25,25,25,255,0,0,0,255,0,0,0,255,0,0,0,191,0,0,0,64,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,32,0,0,0,175,0,0,0,255,0,0,0,255,76,76,76,255,154,154,154,255,211,211,211,255,219,219,219,255,227,227,227,255,186,186,186,255,172,172,172,255,144,144,144,255,115,115,115,255,115,115,115,255,144,144,144,255,172,172,172,255,186,186,186,255,226,226,226,255,218,218,218,255,210,210,210,255,153,153,153,255,63,63,63,255,0,0,0,255,0,0,0,255,0,0,0,175,0,0,0,16,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,12,12,12,170,0,0,0,255,51,51,51,255,168,168,168,255,214,214,214,255,225,225,225,255,158,158,158,255,101,101,101,255,29,29,29,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,29,29,29,255,101,101,101,255,172,172,172,255,225,225,225,255,213,213,213,255,154,154,154,255,38,38,38,255,0,0,0,255,1,1,1,224,0,0,0,48,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,205,205,205,86,177,177,177,246,132,132,132,255,216,216,216,255,226,226,226,255,171,171,171,255,29,29,29,255,0,0,0,255,0,0,0,255,6,13,13,255,36,75,76,255,49,100,102,255,61,125,127,255,73,150,153,255,73,150,153,255,73,150,153,255,49,100,102,255,36,75,76,255,12,25,26,255,0,0,0,255,0,0,0,255,58,58,58,255,171,171,171,255,225,225,225,255,215,215,215,255,107,107,107,255,0,0,0,255,2,2,2,242,0,0,0,96,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,96,61,61,61,181,204,204,204,255,204,204,204,255,219,219,219,255,200,200,200,255,58,58,58,255,0,0,0,255,0,0,0,255,37,76,77,255,79,163,165,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,73,150,153,255,84,88,88,255,0,0,0,255,0,0,0,255,58,58,58,255,214,214,214,255,217,217,217,255,153,153,153,255,12,12,12,255,0,0,0,255,0,0,0,96,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,64,0,0,0,255,25,25,25,255,205,205,205,255,223,223,223,255,171,171,171,255,15,15,15,255,0,0,0,255,31,63,64,255,85,175,178,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,132,154,155,255,255,255,255,255,207,207,207,255,64,64,64,255,0,0,0,255,15,15,15,255,186,186,186,255,221,221,221,255,168,168,168,255,12,12,12,255,0,0,0,255,0,0,0,64,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,32,2,2,2,241,0,0,0,255,166,166,166,255,222,222,222,255,143,143,143,255,0,0,0,255,6,13,13,255,61,125,127,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,122,151,152,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,143,143,143,255,0,0,0,255,0,0,0,255,143,143,143,255,220,220,220,255,165,165,165,255,0,0,0,255,0,0,0,240,0,0,0,16,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,208,0,0,0,255,127,127,127,255,218,218,218,255,157,157,157,255,0,0,0,255,6,13,13,255,79,163,165,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,134,174,176,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,177,180,180,255,6,13,13,255,0,0,0,255,171,171,171,255,217,217,217,255,114,114,114,255,0,0,0,255,0,0,0,191,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,96,0,0,0,255,63,63,63,255,214,214,214,255,186,186,186,255,15,15,15,255,6,13,13,255,79,163,165,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,134,174,176,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,240,240,240,255,112,180,182,255,73,150,153,255,0,0,0,255,15,15,15,255,186,186,186,255,212,212,212,255,51,51,51,255,0,0,0,255,0,0,0,96,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,3,3,226,0,0,0,255,180,180,180,255,226,226,226,255,29,29,29,255,0,0,0,255,73,151,153,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,134,174,176,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,240,240,240,255,112,180,182,255,98,201,204,255,98,201,204,255,61,125,127,255,0,0,0,255,43,43,43,255,225,225,225,255,179,179,179,255,0,0,0,255,0,0,0,223,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,96,0,0,0,255,102,102,102,255,217,217,217,255,128,128,128,255,0,0,0,255,37,76,77,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,134,174,176,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,240,240,240,255,112,180,182,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,31,63,64,255,0,0,0,255,143,143,143,255,216,216,216,255,76,76,76,255,0,0,0,255,0,0,0,96,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,224,12,12,12,255,196,196,196,255,214,214,214,255,15,15,15,255,6,13,13,255,91,188,191,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,134,174,176,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,240,240,240,255,112,180,182,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,85,175,178,255,0,0,0,255,29,29,29,255,227,227,227,255,182,182,182,255,0,0,0,255,0,0,0,207,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,32,0,0,0,255,63,63,63,255,214,214,214,255,128,128,128,255,0,0,0,255,55,113,115,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,134,174,176,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,240,240,240,255,112,180,182,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,56,98,100,255,0,0,0,255,143,143,143,255,212,212,212,255,63,63,63,255,0,0,0,255,0,0,0,32,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,112,0,0,0,255,127,127,127,255,222,222,222,255,43,43,43,255,0,0,0,255,86,176,179,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,134,174,176,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,240,240,240,255,112,180,182,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,113,181,182,255,208,209,209,255,0,0,0,255,71,71,71,255,221,221,221,255,127,127,127,255,0,0,0,255,0,0,0,112,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,192,0,0,0,255,180,180,180,255,213,213,213,255,0,0,0,255,31,63,64,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,134,174,176,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,240,240,240,255,112,180,182,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,113,181,182,255,239,239,239,255,255,255,255,255,48,48,48,255,0,0,0,255,227,227,227,255,179,179,179,255,0,0,0,255,0,0,0,176,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,255,38,38,38,255,211,211,211,255,157,157,157,255,0,0,0,255,49,101,102,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,134,174,176,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,240,240,240,255,112,180,182,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,113,181,182,255,239,239,239,255,255,255,255,255,255,255,255,255,128,128,128,255,0,0,0,255,171,171,171,255,208,208,208,255,0,0,0,255,0,0,0,223,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,255,51,51,51,255,212,212,212,255,114,114,114,255,0,0,0,255,67,138,141,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,134,174,176,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,240,240,240,255,112,180,182,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,113,181,182,255,239,239,239,255,255,255,255,255,255,255,255,255,255,255,255,255,160,160,160,255,0,0,0,255,143,143,143,255,211,211,211,255,51,51,51,255,0,0,0,255,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,16,0,0,0,255,51,51,51,255,215,215,215,255,114,114,114,255,0,0,0,255,80,164,166,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,134,174,176,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,240,240,240,255,112,180,182,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,113,181,182,255,239,239,239,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,192,192,192,255,0,0,0,255,114,114,114,255,213,213,213,255,51,51,51,255,0,0,0,255,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,64,0,0,0,255,102,102,102,255,218,218,218,255,71,71,71,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,114,114,114,255,216,216,216,255,63,63,63,255,0,0,0,255,0,0,0,48,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,64,0,0,0,255,102,102,102,255,218,218,218,255,171,171,171,255,57,57,57,255,57,57,57,255,57,57,57,255,57,57,57,255,57,57,57,255,57,57,57,255,57,57,57,255,57,57,57,255,57,57,57,255,57,57,57,255,57,57,57,255,57,57,57,255,57,57,57,255,57,57,57,255,57,57,57,255,57,57,57,255,57,57,57,255,57,57,57,255,57,57,57,255,57,57,57,255,57,57,57,255,57,57,57,255,57,57,57,255,57,57,57,255,57,57,57,255,57,57,57,255,57,57,57,255,57,57,57,255,57,57,57,255,57,57,57,255,57,57,57,255,57,57,57,255,57,57,57,255,71,71,71,255,171,171,171,255,217,217,217,255,102,102,102,255,0,0,0,255,0,0,0,64,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,64,0,0,0,255,102,102,102,255,218,218,218,255,229,229,229,255,229,229,229,255,229,229,229,255,229,229,229,255,229,229,229,255,229,229,229,255,229,229,229,255,229,229,229,255,229,229,229,255,229,229,229,255,229,229,229,255,229,229,229,255,229,229,229,255,229,229,229,255,229,229,229,255,229,229,229,255,229,229,229,255,229,229,229,255,229,229,229,255,229,229,229,255,229,229,229,255,229,229,229,255,229,229,229,255,229,229,229,255,229,229,229,255,229,229,229,255,229,229,229,255,229,229,229,255,229,229,229,255,229,229,229,255,229,229,229,255,229,229,229,255,229,229,229,255,229,229,229,255,229,229,229,255,229,229,229,255,217,217,217,255,102,102,102,255,0,0,0,255,0,0,0,64,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,64,0,0,0,255,102,102,102,255,218,218,218,255,214,214,214,255,172,172,172,255,172,172,172,255,172,172,172,255,172,172,172,255,172,172,172,255,172,172,172,255,172,172,172,255,172,172,172,255,172,172,172,255,172,172,172,255,172,172,172,255,172,172,172,255,172,172,172,255,172,172,172,255,172,172,172,255,214,214,214,255,229,229,229,255,229,229,229,255,214,214,214,255,172,172,172,255,172,172,172,255,172,172,172,255,172,172,172,255,172,172,172,255,172,172,172,255,172,172,172,255,172,172,172,255,172,172,172,255,172,172,172,255,172,172,172,255,172,172,172,255,172,172,172,255,172,172,172,255,172,172,172,255,214,214,214,255,217,217,217,255,102,102,102,255,0,0,0,255,0,0,0,64,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,64,0,0,0,255,102,102,102,255,218,218,218,255,85,85,85,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,128,128,128,255,229,229,229,255,229,229,229,255,114,114,114,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,85,85,85,255,217,217,217,255,102,102,102,255,0,0,0,255,0,0,0,64,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,64,0,0,0,255,102,102,102,255,218,218,218,255,57,57,57,255,0,0,0,255,49,101,102,255,49,100,102,255,49,100,102,255,49,100,102,255,49,100,102,255,49,100,102,255,49,100,102,255,49,100,102,255,49,100,102,255,49,100,102,255,49,100,102,255,49,100,102,255,37,76,77,255,0,0,0,255,114,114,114,255,229,229,229,255,229,229,229,255,114,114,114,255,0,0,0,255,37,76,77,255,49,100,102,255,49,100,102,255,49,100,102,255,49,100,102,255,49,100,102,255,49,100,102,255,49,100,102,255,49,100,102,255,49,100,102,255,49,100,102,255,49,100,102,255,37,76,77,255,0,0,0,255,57,57,57,255,217,217,217,255,102,102,102,255,0,0,0,255,0,0,0,64,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,64,0,0,0,255,102,102,102,255,218,218,218,255,57,57,57,255,0,0,0,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,73,151,153,255,0,0,0,255,114,114,114,255,229,229,229,255,229,229,229,255,114,114,114,255,0,0,0,255,73,151,153,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,73,151,153,255,0,0,0,255,57,57,57,255,217,217,217,255,102,102,102,255,0,0,0,255,0,0,0,64,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,64,0,0,0,255,102,102,102,255,218,218,218,255,57,57,57,255,0,0,0,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,73,151,153,255,0,0,0,255,114,114,114,255,229,229,229,255,229,229,229,255,114,114,114,255,0,0,0,255,73,151,153,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,73,151,153,255,0,0,0,255,57,57,57,255,217,217,217,255,102,102,102,255,0,0,0,255,0,0,0,64,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,64,0,0,0,255,102,102,102,255,218,218,218,255,57,57,57,255,0,0,0,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,119,143,144,255,0,0,0,255,114,114,114,255,229,229,229,255,229,229,229,255,114,114,114,255,0,0,0,255,73,151,153,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,119,143,144,255,0,0,0,255,57,57,57,255,217,217,217,255,102,102,102,255,0,0,0,255,0,0,0,64,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,64,0,0,0,255,102,102,102,255,218,218,218,255,57,57,57,255,0,0,0,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,173,188,188,255,192,192,192,255,0,0,0,255,114,114,114,255,229,229,229,255,229,229,229,255,114,114,114,255,0,0,0,255,73,151,153,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,173,188,188,255,192,192,192,255,0,0,0,255,57,57,57,255,217,217,217,255,102,102,102,255,0,0,0,255,0,0,0,64,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,64,0,0,0,255,102,102,102,255,218,218,218,255,57,57,57,255,0,0,0,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,173,188,188,255,255,255,255,255,192,192,192,255,0,0,0,255,114,114,114,255,229,229,229,255,229,229,229,255,114,114,114,255,0,0,0,255,73,151,153,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,173,188,188,255,255,255,255,255,192,192,192,255,0,0,0,255,57,57,57,255,217,217,217,255,102,102,102,255,0,0,0,255,0,0,0,64,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,64,0,0,0,255,102,102,102,255,218,218,218,255,57,57,57,255,0,0,0,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,173,188,188,255,255,255,255,255,255,255,255,255,192,192,192,255,0,0,0,255,114,114,114,255,229,229,229,255,229,229,229,255,114,114,114,255,0,0,0,255,73,151,153,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,173,188,188,255,255,255,255,255,255,255,255,255,192,192,192,255,0,0,0,255,57,57,57,255,217,217,217,255,102,102,102,255,0,0,0,255,0,0,0,64,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,64,0,0,0,255,102,102,102,255,218,218,218,255,57,57,57,255,0,0,0,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,173,188,188,255,255,255,255,255,255,255,255,255,255,255,255,255,129,147,148,255,0,0,0,255,114,114,114,255,229,229,229,255,229,229,229,255,114,114,114,255,0,0,0,255,73,151,153,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,173,188,188,255,255,255,255,255,255,255,255,255,255,255,255,255,151,160,161,255,0,0,0,255,57,57,57,255,217,217,217,255,102,102,102,255,0,0,0,255,0,0,0,64,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,64,0,0,0,255,102,102,102,255,218,218,218,255,57,57,57,255,0,0,0,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,173,188,188,255,255,255,255,255,255,255,255,255,255,255,255,255,134,174,176,255,73,151,153,255,0,0,0,255,114,114,114,255,229,229,229,255,229,229,229,255,114,114,114,255,0,0,0,255,73,151,153,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,173,188,188,255,255,255,255,255,255,255,255,255,255,255,255,255,173,188,188,255,73,151,153,255,0,0,0,255,57,57,57,255,217,217,217,255,102,102,102,255,0,0,0,255,0,0,0,64,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,64,0,0,0,255,102,102,102,255,218,218,218,255,57,57,57,255,0,0,0,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,173,188,188,255,255,255,255,255,255,255,255,255,255,255,255,255,134,174,176,255,98,201,204,255,73,151,153,255,0,0,0,255,114,114,114,255,229,229,229,255,229,229,229,255,114,114,114,255,0,0,0,255,73,151,153,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,173,188,188,255,255,255,255,255,255,255,255,255,255,255,255,255,173,188,188,255,98,201,204,255,73,151,153,255,0,0,0,255,57,57,57,255,217,217,217,255,102,102,102,255,0,0,0,255,0,0,0,64,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,64,0,0,0,255,102,102,102,255,218,218,218,255,57,57,57,255,0,0,0,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,173,188,188,255,255,255,255,255,255,255,255,255,255,255,255,255,134,174,176,255,98,201,204,255,98,201,204,255,73,151,153,255,0,0,0,255,114,114,114,255,229,229,229,255,229,229,229,255,114,114,114,255,0,0,0,255,73,151,153,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,173,188,188,255,255,255,255,255,255,255,255,255,255,255,255,255,173,188,188,255,98,201,204,255,98,201,204,255,73,151,153,255,0,0,0,255,57,57,57,255,217,217,217,255,102,102,102,255,0,0,0,255,0,0,0,64,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,64,0,0,0,255,102,102,102,255,218,218,218,255,57,57,57,255,0,0,0,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,173,188,188,255,255,255,255,255,255,255,255,255,255,255,255,255,134,174,176,255,98,201,204,255,98,201,204,255,98,201,204,255,73,151,153,255,0,0,0,255,114,114,114,255,229,229,229,255,229,229,229,255,114,114,114,255,0,0,0,255,73,151,153,255,98,201,204,255,98,201,204,255,98,201,204,255,173,188,188,255,255,255,255,255,255,255,255,255,255,255,255,255,173,188,188,255,98,201,204,255,98,201,204,255,98,201,204,255,73,151,153,255,0,0,0,255,57,57,57,255,217,217,217,255,102,102,102,255,0,0,0,255,0,0,0,64,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,64,0,0,0,255,102,102,102,255,218,218,218,255,57,57,57,255,0,0,0,255,98,201,204,255,98,201,204,255,98,201,204,255,173,188,188,255,255,255,255,255,255,255,255,255,255,255,255,255,134,174,176,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,73,151,153,255,0,0,0,255,114,114,114,255,229,229,229,255,229,229,229,255,114,114,114,255,0,0,0,255,73,151,153,255,98,201,204,255,98,201,204,255,173,188,188,255,255,255,255,255,255,255,255,255,255,255,255,255,173,188,188,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,73,151,153,255,0,0,0,255,57,57,57,255,217,217,217,255,102,102,102,255,0,0,0,255,0,0,0,64,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,64,0,0,0,255,102,102,102,255,218,218,218,255,57,57,57,255,0,0,0,255,98,201,204,255,98,201,204,255,173,188,188,255,255,255,255,255,255,255,255,255,255,255,255,255,134,174,176,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,73,151,153,255,0,0,0,255,114,114,114,255,229,229,229,255,229,229,229,255,114,114,114,255,0,0,0,255,73,151,153,255,98,201,204,255,173,188,188,255,255,255,255,255,255,255,255,255,255,255,255,255,173,188,188,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,73,151,153,255,0,0,0,255,57,57,57,255,217,217,217,255,102,102,102,255,0,0,0,255,0,0,0,64,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,64,0,0,0,255,102,102,102,255,218,218,218,255,57,57,57,255,0,0,0,255,98,201,204,255,173,188,188,255,255,255,255,255,255,255,255,255,255,255,255,255,134,174,176,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,73,151,153,255,0,0,0,255,114,114,114,255,229,229,229,255,229,229,229,255,114,114,114,255,0,0,0,255,73,151,153,255,173,188,188,255,255,255,255,255,255,255,255,255,255,255,255,255,173,188,188,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,73,151,153,255,0,0,0,255,57,57,57,255,217,217,217,255,102,102,102,255,0,0,0,255,0,0,0,64,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,64,0,0,0,255,102,102,102,255,218,218,218,255,57,57,57,255,0,0,0,255,173,188,188,255,255,255,255,255,255,255,255,255,255,255,255,255,134,174,176,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,73,151,153,255,0,0,0,255,114,114,114,255,229,229,229,255,229,229,229,255,114,114,114,255,0,0,0,255,151,160,161,255,255,255,255,255,255,255,255,255,255,255,255,255,173,188,188,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,73,151,153,255,0,0,0,255,57,57,57,255,217,217,217,255,102,102,102,255,0,0,0,255,0,0,0,64,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,64,0,0,0,255,102,102,102,255,218,218,218,255,57,57,57,255,0,0,0,255,255,255,255,255,255,255,255,255,255,255,255,255,134,174,176,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,73,151,153,255,0,0,0,255,114,114,114,255,229,229,229,255,229,229,229,255,114,114,114,255,0,0,0,255,192,192,192,255,255,255,255,255,255,255,255,255,173,188,188,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,73,151,153,255,0,0,0,255,57,57,57,255,217,217,217,255,102,102,102,255,0,0,0,255,0,0,0,64,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,64,0,0,0,255,102,102,102,255,218,218,218,255,57,57,57,255,0,0,0,255,255,255,255,255,255,255,255,255,134,174,176,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,73,151,153,255,0,0,0,255,114,114,114,255,229,229,229,255,229,229,229,255,114,114,114,255,0,0,0,255,192,192,192,255,255,255,255,255,173,188,188,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,73,151,153,255,0,0,0,255,57,57,57,255,217,217,217,255,102,102,102,255,0,0,0,255,0,0,0,64,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,64,0,0,0,255,102,102,102,255,218,218,218,255,57,57,57,255,0,0,0,255,255,255,255,255,134,174,176,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,73,151,153,255,0,0,0,255,114,114,114,255,229,229,229,255,229,229,229,255,114,114,114,255,0,0,0,255,192,192,192,255,142,175,176,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,73,151,153,255,0,0,0,255,57,57,57,255,217,217,217,255,102,102,102,255,0,0,0,255,0,0,0,64,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,64,0,0,0,255,102,102,102,255,218,218,218,255,57,57,57,255,0,0,0,255,134,174,176,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,73,151,153,255,0,0,0,255,114,114,114,255,229,229,229,255,229,229,229,255,114,114,114,255,0,0,0,255,92,139,141,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,73,151,153,255,0,0,0,255,57,57,57,255,217,217,217,255,102,102,102,255,0,0,0,255,0,0,0,64,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,64,0,0,0,255,102,102,102,255,218,218,218,255,57,57,57,255,0,0,0,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,73,151,153,255,0,0,0,255,114,114,114,255,229,229,229,255,229,229,229,255,114,114,114,255,0,0,0,255,73,151,153,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,98,201,204,255,73,151,153,255,0,0,0,255,57,57,57,255,217,217,217,255,102,102,102,255,0,0,0,255,0,0,0,64,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,64,0,0,0,255,102,102,102,255,218,218,218,255,57,57,57,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,114,114,114,255,229,229,229,255,229,229,229,255,114,114,114,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,71,71,71,255,217,217,217,255,102,102,102,255,0,0,0,255,0,0,0,64,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,64,0,0,0,255,102,102,102,255,218,218,218,255,171,171,171,255,57,57,57,255,57,57,57,255,57,57,57,255,57,57,57,255,57,57,57,255,57,57,57,255,57,57,57,255,57,57,57,255,57,57,57,255,57,57,57,255,57,57,57,255,57,57,57,255,57,57,57,255,57,57,57,255,71,71,71,255,200,200,200,255,229,229,229,255,229,229,229,255,186,186,186,255,57,57,57,255,57,57,57,255,57,57,57,255,57,57,57,255,57,57,57,255,57,57,57,255,57,57,57,255,57,57,57,255,57,57,57,255,57,57,57,255,57,57,57,255,57,57,57,255,57,57,57,255,57,57,57,255,57,57,57,255,171,171,171,255,217,217,217,255,102,102,102,255,0,0,0,255,0,0,0,64,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,64,0,0,0,255,102,102,102,255,212,212,212,255,216,216,216,255,216,216,216,255,216,216,216,255,216,216,216,255,216,216,216,255,216,216,216,255,216,216,216,255,216,216,216,255,216,216,216,255,216,216,216,255,216,216,216,255,216,216,216,255,216,216,216,255,216,216,216,255,216,216,216,255,216,216,216,255,216,216,216,255,216,216,216,255,216,216,216,255,216,216,216,255,216,216,216,255,216,216,216,255,216,216,216,255,216,216,216,255,216,216,216,255,216,216,216,255,216,216,216,255,216,216,216,255,216,216,216,255,216,216,216,255,216,216,216,255,216,216,216,255,216,216,216,255,216,216,216,255,216,216,216,255,216,216,216,255,211,211,211,255,102,102,102,255,0,0,0,255,0,0,0,64,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,80,0,0,0,127,0,0,0,127,0,0,0,159,0,0,0,255,51,51,51,255,102,102,102,255,102,102,102,255,102,102,102,255,102,102,102,255,102,102,102,255,102,102,102,255,102,102,102,255,102,102,102,255,102,102,102,255,102,102,102,255,102,102,102,255,102,102,102,255,102,102,102,255,102,102,102,255,102,102,102,255,102,102,102,255,102,102,102,255,102,102,102,255,102,102,102,255,102,102,102,255,102,102,102,255,102,102,102,255,102,102,102,255,102,102,102,255,102,102,102,255,102,102,102,255,102,102,102,255,102,102,102,255,102,102,102,255,102,102,102,255,102,102,102,255,102,102,102,255,102,102,102,255,102,102,102,255,102,102,102,255,102,102,102,255,102,102,102,255,102,102,102,255,51,51,51,255,0,0,0,255,0,0,0,159,0,0,0,127,0,0,0,127,0,0,0,80,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,239,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,255,44,29,11,255,179,117,45,255,179,117,45,255,179,117,45,255,179,117,45,255,179,117,45,255,179,117,45,255,179,117,45,255,179,117,45,255,179,117,45,255,179,117,45,255,179,117,45,255,179,117,45,255,179,117,45,255,179,117,45,255,179,117,45,255,179,117,45,255,179,117,45,255,179,117,45,255,179,117,45,255,179,117,45,255,179,117,45,255,179,117,45,255,179,117,45,255,179,117,45,255,179,117,45,255,179,117,45,255,179,117,45,255,179,117,45,255,179,117,45,255,179,117,45,255,179,117,45,255,179,117,45,255,179,117,45,255,179,117,45,255,179,117,45,255,179,117,45,255,179,117,45,255,179,117,45,255,179,117,45,255,179,117,45,255,179,117,45,255,179,117,45,255,179,117,45,255,179,117,45,255,179,117,45,255,179,117,45,255,44,29,11,255,0,0,0,255,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,255,59,39,15,255,239,157,60,255,239,157,60,255,239,157,60,255,239,157,60,255,239,157,60,255,239,157,60,255,239,157,60,255,239,157,60,255,239,157,60,255,239,157,60,255,239,157,60,255,239,157,60,255,239,157,60,255,239,157,60,255,239,157,60,255,239,157,60,255,239,157,60,255,239,157,60,255,239,157,60,255,239,157,60,255,239,157,60,255,239,157,60,255,239,157,60,255,239,157,60,255,239,157,60,255,239,157,60,255,239,157,60,255,239,157,60,255,239,157,60,255,239,157,60,255,239,157,60,255,239,157,60,255,239,157,60,255,239,157,60,255,239,157,60,255,239,157,60,255,239,157,60,255,239,157,60,255,239,157,60,255,239,157,60,255,239,157,60,255,239,157,60,255,239,157,60,255,239,157,60,255,239,157,60,255,239,157,60,255,59,39,15,255,0,0,0,255,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,255,58,34,11,255,232,137,45,255,232,137,45,255,232,137,45,255,232,137,45,255,232,137,45,255,232,137,45,255,232,137,45,255,232,137,45,255,232,137,45,255,232,137,45,255,232,137,45,255,232,137,45,255,232,137,45,255,232,137,45,255,232,137,45,255,232,137,45,255,232,137,45,255,232,137,45,255,232,137,45,255,232,137,45,255,232,137,45,255,232,137,45,255,232,137,45,255,232,137,45,255,232,137,45,255,232,137,45,255,232,137,45,255,232,137,45,255,232,137,45,255,232,137,45,255,232,137,45,255,232,137,45,255,232,137,45,255,232,137,45,255,232,137,45,255,232,137,45,255,232,137,45,255,232,137,45,255,232,137,45,255,232,137,45,255,232,137,45,255,232,137,45,255,232,137,45,255,232,137,45,255,232,137,45,255,232,137,45,255,58,34,11,255,0,0,0,255,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,255,29,15,3,255,113,59,15,255,113,59,15,255,113,59,15,255,113,59,15,255,113,59,15,255,113,59,15,255,113,59,15,255,113,59,15,255,113,59,15,255,113,59,15,255,113,59,15,255,113,59,15,255,113,59,15,255,113,59,15,255,113,59,15,255,113,59,15,255,113,59,15,255,113,59,15,255,113,59,15,255,113,59,15,255,113,59,15,255,113,59,15,255,113,59,15,255,113,59,15,255,113,59,15,255,113,59,15,255,113,59,15,255,113,59,15,255,113,59,15,255,113,59,15,255,113,59,15,255,113,59,15,255,113,59,15,255,113,59,15,255,113,59,15,255,113,59,15,255,183,96,24,255,226,119,30,255,226,119,30,255,211,111,28,255,113,59,15,255,113,59,15,255,113,59,15,255,113,59,15,255,113,59,15,255,113,59,15,255,29,15,3,255,0,0,0,255,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,226,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,83,48,15,175,232,134,41,112,232,134,41,112,97,55,17,166,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,255,0,0,0,224,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,16,0,0,0,64,0,0,0,64,0,0,0,64,0,0,0,64,0,0,0,64,0,0,0,64,0,0,0,64,0,0,0,64,0,0,0,64,0,0,0,64,0,0,0,64,0,0,0,64,0,0,0,64,0,0,0,64,0,0,0,64,0,0,0,64,0,0,0,64,0,0,0,64,0,0,0,64,0,0,0,64,0,0,0,64,0,0,0,64,0,0,0,64,0,0,0,64,0,0,0,64,0,0,0,64,0,0,0,64,0,0,0,64,0,0,0,64,0,0,0,64,0,0,0,64,0,0,0,64,0,0,0,64,0,0,0,64,0,0,0,64,0,0,0,64,0,0,0,48,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,48,0,0,0,64,0,0,0,64,0,0,0,64,0,0,0,64,0,0,0,64,0,0,0,64,0,0,0,16,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, };
GLubyte penguin[checkImageHeight][chaeckImageWidth][4] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,96,48,48,16,102,50,50,127,102,51,51,175,101,51,51,191,101,50,50,159,102,50,50,127,101,49,49,63,104,48,48,32,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,96,48,48,16,101,49,49,63,102,50,50,127,101,50,50,239,102,51,51,255,102,51,51,255,102,51,51,255,102,51,51,255,102,51,51,255,102,51,51,255,102,51,51,255,101,51,51,191,102,51,51,95,96,48,48,16,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,100,48,48,64,102,51,51,175,102,51,51,255,102,51,51,255,102,51,51,255,102,51,51,255,107,59,59,255,126,88,88,255,140,111,112,255,121,80,81,255,111,67,67,255,102,51,51,255,102,51,51,255,102,51,51,255,102,51,51,255,101,50,50,239,102,50,50,127,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,102,50,50,143,102,51,51,255,102,51,51,255,102,51,51,255,102,51,51,255,102,51,51,255,117,73,73,255,168,155,156,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,172,163,164,255,149,125,126,255,126,88,88,255,102,51,51,255,102,51,51,255,102,51,51,255,102,50,50,223,100,48,48,64,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,101,50,50,159,102,51,51,255,102,51,51,255,121,70,57,255,140,89,63,255,102,51,51,255,102,51,51,255,168,155,156,255,178,171,172,255,145,118,118,255,102,51,51,255,154,133,134,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,168,155,156,255,130,96,96,255,102,51,51,255,102,51,51,255,102,51,51,255,102,50,50,127,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,102,50,50,128,102,51,51,255,102,51,51,255,159,108,69,255,255,204,102,255,150,99,67,255,102,51,51,255,122,81,81,255,178,171,172,255,178,171,172,255,130,96,96,255,102,51,51,255,145,118,118,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,159,141,142,255,117,73,73,255,102,51,51,255,102,51,51,255,101,50,50,159,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,104,48,48,32,102,51,51,255,102,51,51,255,159,108,69,255,255,204,102,255,255,204,102,255,140,89,63,255,102,51,51,255,122,81,81,255,178,171,172,255,178,171,172,255,172,163,164,255,158,140,141,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,172,163,164,255,126,89,89,255,102,51,51,255,102,51,51,255,102,51,51,175,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,101,50,50,144,102,51,51,255,102,51,51,255,102,51,51,255,102,51,51,255,130,80,60,255,121,70,57,255,102,51,51,255,111,67,67,255,173,163,164,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,131,96,97,255,102,51,51,255,102,51,51,255,102,50,50,128,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,101,50,50,192,102,51,51,255,102,51,51,255,102,51,51,255,102,51,51,255,102,51,51,255,102,51,51,255,102,51,51,255,102,51,51,255,131,96,97,255,173,163,164,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,126,89,89,255,102,51,51,255,102,51,51,255,100,48,48,64,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,101,48,48,48,102,50,50,128,102,50,50,128,101,50,50,96,102,50,50,128,101,50,50,144,101,50,50,224,102,51,51,255,102,51,51,255,102,51,51,255,117,73,73,255,173,163,164,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,172,163,164,255,117,73,73,255,102,51,51,255,101,50,50,239,96,48,48,16,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,101,48,48,48,102,51,51,160,102,51,51,255,102,51,51,255,117,73,73,255,173,163,164,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,163,148,149,255,102,51,51,255,102,51,51,255,102,50,50,128,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,101,50,50,96,102,51,51,255,102,51,51,255,140,111,111,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,131,96,97,255,102,51,51,255,101,50,50,239,96,48,48,16,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,102,51,51,160,102,51,51,255,107,59,59,255,173,163,164,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,168,155,156,255,102,51,51,255,102,51,51,255,102,50,50,112,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,96,48,48,16,102,51,51,255,102,51,51,255,149,126,127,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,131,96,97,255,102,51,51,255,101,51,51,207,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,102,50,50,208,102,51,51,255,140,111,111,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,149,126,127,255,102,51,51,255,102,51,51,255,96,48,48,16,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,101,50,50,192,102,51,51,255,122,81,81,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,172,163,164,255,102,51,51,255,102,51,51,255,101,50,50,96,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,101,50,50,192,102,51,51,255,131,96,97,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,122,81,81,255,102,51,51,255,102,50,50,128,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,102,50,50,223,102,51,51,255,140,111,111,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,122,81,81,255,102,51,51,255,101,51,51,176,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,100,48,48,64,102,51,51,255,102,51,51,255,163,148,149,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,131,96,97,255,135,104,104,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,135,104,104,255,102,51,51,255,101,50,50,192,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,96,48,48,16,101,51,51,207,102,51,51,255,111,67,67,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,159,141,142,255,102,51,51,255,102,51,51,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,140,111,111,255,102,51,51,255,101,50,50,192,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,101,50,50,159,102,51,51,255,102,51,51,255,154,133,134,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,159,141,142,255,102,51,51,255,102,51,51,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,140,111,111,255,102,51,51,255,101,50,50,192,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,102,50,50,112,102,51,51,255,102,51,51,255,135,104,104,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,159,141,142,255,102,51,51,255,102,51,51,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,140,111,111,255,102,51,51,255,101,50,50,192,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,102,51,51,80,102,51,51,255,102,51,51,255,126,89,89,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,159,141,142,255,102,51,51,255,102,51,51,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,140,111,111,255,102,51,51,255,101,50,50,192,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,104,48,48,32,101,50,50,239,102,51,51,255,117,73,73,255,172,163,164,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,159,141,142,255,102,51,51,255,102,51,51,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,140,111,111,255,102,51,51,255,101,50,50,192,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,102,51,51,160,102,51,51,255,107,59,59,255,168,155,156,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,159,141,142,255,102,51,51,255,102,51,51,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,140,111,111,255,102,51,51,255,101,50,50,192,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,101,48,48,48,102,51,51,255,102,51,51,255,140,111,111,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,159,141,142,255,102,51,51,255,102,51,51,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,140,111,111,255,102,51,51,255,101,50,50,192,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,101,51,51,191,102,51,51,255,111,67,67,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,159,141,142,255,102,51,51,255,102,51,51,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,140,111,111,255,102,51,51,255,101,50,50,192,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,104,48,48,32,102,51,51,255,102,51,51,255,149,126,127,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,159,141,142,255,102,51,51,255,102,51,51,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,140,111,111,255,102,51,51,255,101,50,50,192,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,102,50,50,112,102,51,51,255,102,51,51,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,159,141,142,255,102,51,51,255,102,51,51,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,140,111,111,255,102,51,51,255,101,50,50,192,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,101,51,51,176,102,51,51,255,126,89,89,255,178,171,172,255,178,171,172,255,178,171,172,255,145,118,118,255,154,133,134,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,159,141,142,255,102,51,51,255,102,51,51,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,140,111,111,255,102,51,51,255,101,50,50,192,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,96,48,48,16,102,51,51,255,102,51,51,255,149,126,127,255,178,171,172,255,178,171,172,255,149,125,126,255,102,51,51,255,106,58,58,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,159,141,142,255,102,51,51,255,102,51,51,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,140,111,111,255,102,51,51,255,101,50,50,192,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,100,48,48,64,102,51,51,255,102,51,51,255,168,155,156,255,178,171,172,255,178,171,172,255,130,96,96,255,102,51,51,255,120,80,81,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,163,148,149,255,102,51,51,255,102,51,51,255,159,141,142,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,140,111,111,255,102,51,51,255,101,50,50,192,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,100,48,48,64,102,51,51,255,102,51,51,255,178,171,172,255,178,171,172,255,178,171,172,255,116,73,73,255,102,51,51,255,140,110,111,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,102,51,51,255,102,51,51,255,159,141,142,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,140,111,111,255,102,51,51,255,101,50,50,192,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,102,50,50,112,102,51,51,255,102,51,51,255,178,171,172,255,178,171,172,255,178,171,172,255,102,51,51,255,102,51,51,255,154,133,134,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,107,59,59,255,102,51,51,255,144,118,119,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,140,111,111,255,102,51,51,255,101,50,50,192,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,102,50,50,128,102,51,51,255,122,81,81,255,178,171,172,255,178,171,172,255,178,171,172,255,102,51,51,255,102,51,51,255,158,140,141,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,131,96,97,255,102,51,51,255,117,73,73,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,140,111,111,255,102,51,51,255,101,50,50,192,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,102,50,50,128,102,51,51,255,117,73,73,255,178,171,172,255,178,171,172,255,178,171,172,255,102,51,51,255,102,51,51,255,158,140,141,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,154,134,134,255,102,51,51,255,102,51,51,255,159,141,142,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,140,111,111,255,102,51,51,255,101,50,50,192,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,101,50,50,96,102,51,51,255,102,51,51,255,178,171,172,255,178,171,172,255,178,171,172,255,107,58,58,255,102,51,51,255,145,118,118,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,117,73,73,255,102,51,51,255,117,73,73,255,173,163,164,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,140,111,111,255,102,51,51,255,101,50,50,192,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,100,48,48,64,102,51,51,255,102,51,51,255,178,171,172,255,178,171,172,255,178,171,172,255,120,80,81,255,102,51,51,255,140,110,111,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,163,148,149,255,102,51,51,255,102,51,51,255,131,96,97,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,140,111,111,255,102,51,51,255,101,50,50,192,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,100,48,48,64,102,51,51,255,102,51,51,255,164,148,149,255,178,171,172,255,178,171,172,255,130,96,96,255,102,51,51,255,116,73,73,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,135,104,104,255,102,51,51,255,102,51,51,255,126,89,89,255,173,163,164,255,178,171,172,255,178,171,172,255,178,171,172,255,140,111,111,255,102,51,51,255,101,50,50,192,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,102,51,51,240,102,51,51,255,144,118,119,255,178,171,172,255,178,171,172,255,154,133,134,255,102,51,51,255,102,51,51,255,168,155,156,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,131,96,97,255,102,51,51,255,102,51,51,255,107,59,59,255,149,126,127,255,173,163,164,255,178,171,172,255,140,111,111,255,102,51,51,255,101,50,50,192,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,101,51,51,176,102,51,51,255,122,81,81,255,178,171,172,255,178,171,172,255,178,171,172,255,107,58,58,255,102,51,51,255,134,102,103,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,117,73,73,255,102,51,51,255,102,51,51,255,102,51,51,255,107,59,59,255,122,81,81,255,122,81,81,255,102,51,51,255,101,50,50,192,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,101,50,50,96,102,51,51,255,102,51,51,255,173,163,164,255,178,171,172,255,178,171,172,255,140,110,111,255,102,51,51,255,102,51,51,255,172,163,164,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,140,111,111,255,102,51,51,255,102,51,51,255,102,51,51,255,102,51,51,255,102,51,51,255,102,51,51,255,102,51,51,255,101,50,50,192,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,96,48,48,16,102,51,51,255,102,51,51,255,140,111,111,255,178,171,172,255,178,171,172,255,172,163,164,255,107,58,58,255,107,58,58,255,168,155,156,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,140,111,111,255,102,51,51,255,111,63,63,255,236,230,230,255,188,165,165,255,149,114,114,255,121,76,76,255,102,51,51,255,101,50,50,192,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,102,51,51,160,102,51,51,255,107,59,59,255,173,163,164,255,178,171,172,255,178,171,172,255,178,171,172,255,172,163,164,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,163,148,149,255,102,51,51,255,102,51,51,255,227,217,217,255,255,255,255,255,255,255,255,255,179,153,153,255,102,51,51,255,101,50,50,192,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,104,48,48,32,102,51,51,240,102,51,51,255,140,111,111,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,111,67,67,255,102,51,51,255,159,127,127,255,255,255,255,255,255,255,255,255,179,153,153,255,102,51,51,255,101,50,50,192,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,102,50,50,128,102,51,51,255,102,51,51,255,164,148,149,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,140,111,111,255,102,51,51,255,102,51,51,255,236,230,230,255,255,255,255,255,179,153,153,255,102,51,51,255,101,50,50,192,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,96,48,48,16,101,50,50,224,102,51,51,255,107,59,59,255,168,156,157,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,172,163,164,255,107,59,59,255,102,51,51,255,150,115,115,255,255,255,255,255,179,153,153,255,102,51,51,255,101,50,50,239,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,101,48,48,48,102,51,51,240,102,51,51,255,117,73,73,255,173,163,164,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,140,111,111,255,102,51,51,255,102,51,51,255,188,166,166,255,217,204,204,255,102,51,51,255,102,51,51,255,96,48,48,16,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,102,51,51,80,102,51,51,255,102,51,51,255,117,73,73,255,168,156,157,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,178,171,172,255,122,81,81,255,102,51,51,255,102,51,51,255,188,166,166,255,102,51,51,255,102,51,51,255,101,50,50,96,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,102,50,50,128,102,51,51,255,102,51,51,255,102,51,51,255,126,89,89,255,144,118,119,255,159,141,142,255,159,141,142,255,159,141,142,255,159,141,142,255,159,141,142,255,159,141,142,255,159,141,142,255,159,141,142,255,159,141,142,255,159,141,142,255,154,134,134,255,107,59,59,255,102,51,51,255,102,51,51,255,111,63,63,255,102,51,51,255,101,50,50,239,96,48,48,16,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,101,50,50,159,102,51,51,255,102,51,51,255,102,51,51,255,102,51,51,255,102,51,51,255,102,51,51,255,102,51,51,255,102,51,51,255,102,51,51,255,102,51,51,255,102,51,51,255,102,51,51,255,102,51,51,255,102,51,51,255,102,51,51,255,102,51,51,255,102,51,51,255,102,51,51,255,102,51,51,255,102,51,51,255,102,51,51,255,102,51,51,255,102,51,51,255,101,51,51,207,104,48,48,32,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,100,48,48,64,102,51,51,255,102,51,51,255,102,51,51,255,102,51,51,255,102,51,51,255,102,51,51,255,102,51,51,255,102,51,51,255,102,51,51,255,102,51,51,255,102,51,51,255,102,51,51,255,102,51,51,255,102,51,51,255,102,51,51,255,102,51,51,255,102,51,51,255,102,51,51,255,102,51,51,255,102,51,51,255,102,51,51,255,102,51,51,255,102,51,51,255,102,51,51,255,102,51,51,255,101,50,50,239,102,50,50,143,101,48,48,48,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,102,50,50,128,102,51,51,255,121,70,57,255,245,194,98,255,255,204,102,255,255,204,102,255,255,204,102,255,255,204,102,255,255,204,102,255,255,204,102,255,255,204,102,255,255,204,102,255,255,204,102,255,255,204,102,255,255,204,102,255,255,204,102,255,255,204,102,255,150,99,67,255,102,51,51,255,101,50,50,224,0,0,0,0,0,0,0,0,100,48,48,64,101,50,50,224,102,51,51,255,102,51,51,255,102,51,51,255,102,51,51,255,102,51,51,255,102,51,51,255,101,51,51,191,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,102,50,50,112,102,51,51,255,111,60,53,255,207,156,85,255,217,165,88,255,217,165,88,255,217,165,88,255,217,165,88,255,217,165,88,255,217,165,88,255,217,165,88,255,217,165,88,255,217,165,88,255,217,165,88,255,217,165,88,255,188,137,79,255,140,89,63,255,102,51,51,255,102,51,51,255,102,50,50,128,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,100,48,48,64,102,51,51,160,101,50,50,224,102,51,51,255,102,51,51,255,102,51,51,255,102,51,51,240,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,104,48,48,32,102,51,51,240,102,51,51,255,102,51,51,255,102,51,51,255,102,51,51,255,102,51,51,255,102,51,51,255,102,51,51,255,102,51,51,255,102,51,51,255,102,51,51,255,102,51,51,255,102,51,51,255,102,51,51,255,102,51,51,255,102,51,51,255,102,51,51,255,102,51,51,160,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,96,48,48,16,100,48,48,64,100,48,48,64,104,48,48,32,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,100,48,48,64,101,50,50,192,102,51,51,255,102,51,51,255,102,51,51,255,102,51,51,255,102,51,51,255,102,51,51,255,102,51,51,255,102,51,51,255,102,51,51,255,102,51,51,255,102,51,51,255,102,51,51,255,102,51,51,240,101,50,50,192,100,48,48,64,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, };


