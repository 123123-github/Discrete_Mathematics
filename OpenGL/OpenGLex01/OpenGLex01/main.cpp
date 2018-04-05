#include <gl\glut.h>
#include <cstdio>
#include <cstdlib>
#include <windows.h>

#define chaeckImageWidth 64
#define checkImageHeight 64
static GLubyte ImageFromTXT[checkImageHeight][chaeckImageWidth][4];	/* RGBA */
static GLuint tex_flower, tex_window, tex_penguin;					/* 三个纹理 */

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
typedef Point Vec;			// 点坐标看作向量

/* 摄像机参数 */
static Point cameraPos = { 0.0, -5.0,  15.0 };
static Vec cameraFront = { 0.0, 0.0, -1.0 };
static Vec cameraUp = { 0.0, 1.0,  0.0 };
static float cameraSpeed = 0.5f;						// 调整移动速度
Vec myCross(const Point& a, const Point& b);

/* 材料 */
GLfloat mat_amb[] = { 0.2,0.2,0.2,1.0 };
GLfloat mat_dif[] = { 0.8,0.8,0.8,1.0 };
GLfloat mat_amb_dif[] = { 0.5,0.5,0.5,1.0 };
GLfloat mat_specular[] = { 0.0,0.0,0.0,1.0 };			// 镜面光
GLfloat mat_shininess[] = { 50.0 };						// 反射亮度

/* 开启光照用到的参数 */
GLfloat light1_pos[] = { 0, 0, 0, 1.0 };				/* 聚光灯 跟随视图移动 */
GLfloat light1_amb[] = { 0.2, 0.2, 0.2, 1.0 };				// 环境光
GLfloat light1_dif[] = { 1.0, 1.0, 1.0, 1.0 };				// 散射光
GLfloat light1_spc[] = { 0.0, 0.0, 0.0, 1.0 };				// 镜面光
GLfloat light1_dir[] = { 0, 0, -1 };						// 聚光灯朝向
GLfloat light1_cutoff = 20;									// 聚光灯切角
GLfloat light1_exp = 500.0;									// 聚集度 or 强度?

GLfloat light2_pos[] = { 0, -1, 0, 0 };					/* 吊灯-颜色2 */
GLfloat light2_amb[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat light2_dif[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat light2_spc[] = { 0.0, 0.0, 0.0, 1.0 };
GLfloat light2_dir[] = { 0, -1, 0 };
GLfloat light2_cutoff = 90;
GLfloat light2_exp = 30.0;

GLfloat light3_pos[] = { 0, -1, 0, 0 };					/* 吊灯-颜色3 */
GLfloat light3_amb[] = { 0.5, 0.5, 0.0, 1.0 };
GLfloat light3_dif[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat light3_spc[] = { 0.0, 0.0, 0.0, 1.0 };
GLfloat light3_dir[] = { 0, -1, 0 };
GLfloat light3_cutoff = 90;
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

/* 房间大小 */
float roomWidth = 20, roomHeight = 15, roomDepth = 30;

/* 移动模型参数 */
bool planetRun = true;
static int year = 0, day = 0, hour = 0;
static float sunSize = 1.0;
float penguinX = 3, penguinDir = -0.05;

/* 三维物体 - 右边的 */
static int robotX = 7, robotY = -roomHeight+3, robotZ = -2;

/* 函数声明 - 模型视图 */
void lookAt(const Point& cameraPos, const Vec& cameraOrient, const Vec& cameraUp);		// 设置相机位置
void drawRoom(int width, int height, int depth);										// 画一个屋子(相当于画一个立方体)
void drawWirePlanets(int x,int y,int z);												// 画三个线框球，可以控制旋转 (作为行星模型)
void drawRobot(int x, int y, int z, int ang);											// 画一个 Panda 
void drawLight(int x, int y, int z);
void drawPainting(float x, float y, float z, GLuint texName,int size);									// 纹理名为 texName

/* 函数声明 - 回调函数 */
void init();
void display();
void reshape(int w, int h);
void keyboard(unsigned char key, int x, int y);
void idel();
void specialKeyboard(int key, int x, int y);

/* 纹理生成 */
void getImage(const char* fileName);							// 从文件中生成纹理

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("ex01");

	init();

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutIdleFunc(idel);
	glutSpecialFunc(specialKeyboard);

	glutMainLoop();

	return 0;
}

/* 回调函数实现 */
void init()
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);		// 选择深蓝绿色作为清空屏幕的颜色
	glShadeModel(GL_FLAT);
	glEnable(GL_DEPTH_TEST);

	/* 光照设置 */
	/* 使用默认参数的环境光 */

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
	//glEnable(GL_LIGHT4);

	/* 纹理 */
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	/* flower */
	getImage("flower.txt");
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
	getImage("window.txt");
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
	getImage("penguin.txt");
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

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// 模型视图
	glMatrixMode(GL_MODELVIEW);					/* 相机位置 (要先设置 Camera 位置，再绘制房间等，否则刚开启窗口时 Camera 位置不正确 ?) */
	glLoadIdentity();
	lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	
	drawRoom(roomWidth, roomHeight, roomDepth);
	drawWirePlanets(0, -6, 0);
	drawLight(0, roomHeight, -3);

	glColor3f(1.0, 0.5, 0.5);							// 模型颜色
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_robot);
	drawRobot(robotX, robotY, robotZ, -45);
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_red);
	drawRobot(-4, -roomHeight+3, -6, 45);
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_blue);
	drawRobot(-6, -roomHeight+3, -9, 45);
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_yellow);
	drawRobot(-8, -roomHeight+3, -6, 45);
	
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_yellow);
	drawPainting(-roomWidth, -roomHeight, -roomDepth+0.1, tex_window,7);
	drawPainting(penguinX, -roomHeight, -roomDepth + 10, tex_penguin,7);
	drawPainting(7, -roomHeight, -roomDepth + 15, tex_flower, 5);

	glutSwapBuffers();
}

void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	
	// 调整视线范围
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (GLfloat)w / (GLfloat)h, 0.1, 500.0);

}

void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
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
		break;
	case 'd':
		//cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;	/* 参考代码 */
		cameraPos += myCross(cameraFront, cameraUp) * cameraSpeed;
		break;
	/* 光源控制 */
	case '.':
		if (glIsEnabled(GL_LIGHT4))
			glDisable(GL_LIGHT4);
		else
			glEnable(GL_LIGHT4);
		break;
	case '0':
		if (glIsEnabled(GL_LIGHTING))
			glDisable(GL_LIGHTING);
		else
			glEnable(GL_LIGHTING);
		break;
	case '1':
		if (glIsEnabled(GL_LIGHT1))
			glDisable(GL_LIGHT1);
		else
			glEnable(GL_LIGHT1);
		break;
	case '2':
		if (glIsEnabled(GL_LIGHT2))
			glDisable(GL_LIGHT2);
		else
			glEnable(GL_LIGHT2);
		break;
	case '3':
		if (glIsEnabled(GL_LIGHT3))
			glDisable(GL_LIGHT3);
		else
			glEnable(GL_LIGHT3);
		break;
	default:
		break;
	}
}

/* 读取贴图数据 */
void getImage(const char* fileName)
{
	/* 读取 */
	int p1, p2, p3, p4;
	FILE* img;
	fopen_s(&img, fileName, "r");
	for (int i = checkImageHeight - 1; i >= 0; --i)
		for (int j = 0;j < chaeckImageWidth; ++j)
		{
			fscanf_s(img, "%d,%d,%d,%d", &p1, &p2, &p3, &p4);
			ImageFromTXT[i][j][0] = (GLubyte)p1;
			ImageFromTXT[i][j][1] = (GLubyte)p2;
			ImageFromTXT[i][j][2] = (GLubyte)p3;
			ImageFromTXT[i][j][3] = (GLubyte)p4;
		}
	fclose(img);
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
	
	glColor3ub(0xD0,0xE4,0x69);						// 左右颜色
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
	lightS_pos[1] = y-3;
	lightS_pos[2] = z;

	glPushMatrix();
		glTranslatef(x, y, z);
		glLightfv(GL_LIGHT4, GL_POSITION, lightS_pos);
		glMaterialfv(GL_FRONT, GL_AMBIENT, mat_sun);
		//glMaterialfv(GL_FRONT, GL_EMISSION, mat_sun_emission);		// 不好用
		glColor3ub(0xE8, 0x5A, 0x08);	// 太阳颜色
		glutWireSphere(sunSize,15, 16);
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

void drawPainting(float x, float y, float z, GLuint texName,int size)
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
