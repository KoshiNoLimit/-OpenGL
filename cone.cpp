#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/GLU.h>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <vector>
#include <cmath>
#include <time.h>
#include <windows.h>
#include <soil.h>
#include <fstream>
#include <string>

using namespace std;

#define PI 3.14159265

float matr[] = {
	1, 0, 0, 0,
	0, 1, 0, 0,
	cos(PI / 4), sin(PI / 4), 1, 0,
	0, 0, 0, 1,
};

bool polygonVision = false, Anime = false, TextureVision = false;;
clock_t global_time;
GLuint textureID;
GLfloat material_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat light0_diffuse[] = { 1, 1, 1 };
GLfloat light0_position[] = { 2.0, 2.0, 2.0, 1.0 };
GLfloat light0_ambient[] = { 3, 3, 3 };
GLfloat light0_spot_direction[] = { -1.0, -1.0, -1.0 };

struct point {
	GLdouble  x, y, z;
};

class Cone {
public:
	vector<vector<point>> points;
	Cone();
	void construct();
	void draw();

	float V = 1;
	float xalpha = 0, yalpha = 0, zalpha = 0;
	float xpos = 0, ypos = 0;
	double h = 1;
	double speed = 0;
	double flour = 0;
	int size = 40;
	double alpha;
	double R = 0.5;
};

Cone::Cone(){
	construct();
}

void Cone::construct() {
	points.resize(size);
	points[0].resize(size);
	for (int i = 0; i < size; i++) 
		points[0][i] = point{ 0, 0, 0 };
	
	for (int j = 1; j < size; j++) {
		points[j].resize(size);
		double gamma = alpha*size/8;
		double sinVal = sin(gamma);
		double cosVal = cos(gamma);

		for (int i = 0; i < size; i++) {
			double fi = PI * 2 * i / size;
			double xt = R * (1 - (double)(j - 1) / size) * cos(fi);
			double zt = R * (1 - (double)(j - 1) / size) * sin(fi);
			double yt = (h*(j - 1)) / size;
			double xtOld = xt;
			xt = xt * cosVal - zt * sinVal;
			zt = xtOld * sinVal + zt * cosVal;
			points[j][i] = { xt, yt, zt };
		}
	}
	for (int i = 0; i < size; i++) 
		points[size - 1][i] = point{ 0, h, 0 };	
}

void Cone::draw() {
	glFrontFace(GL_CW);
	for (int i = 0; i < size - 1; i++) 
		for (int j = 0; j < size; j++) {
			glBindTexture(GL_TEXTURE_2D, textureID);
			glBegin(GL_POLYGON);

			double  x0 = points[i][j].x, x1 = points[(i + 1) % size][j].x, x2 = points[i][(j + 1) % size].x, x3 = points[i + 1][(j + 1) % size].x,
					y0 = points[i][j].y, y1 = points[(i + 1) % size][j].y, y2 = points[i][(j + 1) % size].y, y3 = points[i + 1][(j + 1) % size].y,
					z0 = points[i][j].z, z1 = points[(i + 1) % size][j].z, z2 = points[i][(j + 1) % size].z, z3 = points[i + 1][(j + 1) % size].z;


			glNormal3f((y1 - y0)*(z2 - z1) - (z1 - z0)*(y2 - y1),
					(x1 - x0)*(z2 - z1) - (z1 - z0)*(x2 - x1),
					(x1 - x0)*(y2 - y1) - (y1 - y0)*(x2 - x1));

			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(x0, y0, z0);
			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(x1, y1, z1);
			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(x3, y3, z3);
			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(x2, y2, z2);
			glEnd();
		}
}

auto cone = new Cone();

void save() {
	std::ofstream ttt;
	ttt.open("./position.txt");
	if (ttt.is_open()) {
		ttt << "flour = (" << cone->flour << ") V = (" << cone->V << ") xalpha = (" << cone->xalpha << ") yalpha = (" << cone->yalpha << ") zalpha = (" << cone->zalpha << ") size = (" << cone->size << ") xpos = (" << cone->xpos << ") ypos = (" << cone->ypos << ") speed = (" << cone->speed << ")";
	}
}

void load() {
	std::ifstream file("./position.txt");
	std::string str, word = "";
	getline(file, str);
	file.close();
	int i = 1;

	for (; str[i] != '('; i++);
	i++;
	for (; str[i] != ')'; word += str[i++]);
	double flour = std::stod(word);
	word = "";

	cone = new Cone();

	for (vector<vector<point>>::iterator iter = cone->points.begin(); iter != cone->points.end(); ++iter) 
		for (vector<point>::iterator iter1 = (*iter).begin(); iter1 != (*iter).end(); ++iter1) 
			(*iter1).y += flour;

	cone->flour = flour;

	for (; str[i] != '('; i++);
	i++;
	for (; str[i] != ')'; word += str[i++]);
	cone->V = std::stod(word);
	word = "";

	for (; str[i] != '('; i++);
	i++;
	for (; str[i] != ')'; word += str[i++]);
	cone->xalpha = std::stod(word);
	word = "";

	for (; str[i] != '('; i++);
	i++;
	for (; str[i] != ')'; word += str[i++]);
	cone->yalpha = std::stod(word);
	word = "";

	for (; str[i] != '('; i++);
	i++;
	for (; str[i] != ')'; word += str[i++]);
	cone->zalpha = std::stod(word);
	word = "";

	for (; str[i] != '('; i++);
	i++;
	for (; str[i] != ')'; word += str[i++]);
	cone->size = std::stod(word);
	word = "";

	for (; str[i] != '('; i++);
	i++;
	for (; str[i] != ')'; word += str[i++]);
	cone->xpos = std::stod(word);
	word = "";

	for (; str[i] != '('; i++);
	i++;
	for (; str[i] != ')'; word += str[i++]);
	cone->ypos = std::stod(word);
	word = "";

	for (; str[i] != '('; i++);
	i++;
	for (; str[i] != ')'; word += str[i++]);
	cone->speed = std::stod(word);
	word = "";
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (!action || action == GLFW_REPEAT) {
		switch (key) {
		case GLFW_KEY_L:
			load();
			break;
		case GLFW_KEY_K:
			save();
			break;
		case GLFW_KEY_ENTER:
			global_time = clock();
			Anime ^= true;
			break;
		case GLFW_KEY_UP:
			cone->ypos += 0.1;
			break;
		case GLFW_KEY_DOWN:
			cone->ypos -= 0.1;
			break;
		case GLFW_KEY_LEFT:
			cone->xpos -= 0.1;
			break;
		case GLFW_KEY_RIGHT:
			cone->xpos += 0.1;
			break;
		case GLFW_KEY_R:
			if (!polygonVision) 
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			else 
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			polygonVision = !polygonVision;
			break;
		case GLFW_KEY_T:
			TextureVision ^= true;
			break;
		case GLFW_KEY_S:
			cone->zalpha -= 4;
			break;
		case GLFW_KEY_W:
			cone->zalpha += 4;
			break;
		case GLFW_KEY_A:
			cone->alpha -= 0.2;
			cone->construct();
			break;
		case GLFW_KEY_D:
			cone->alpha += 0.2;
			cone->construct();
			break;
		case GLFW_KEY_Q:
			if (cone->size < 64) {
				cone->size++;
				cone->construct();
			}
			break;
		case GLFW_KEY_E:
			if (cone->size > 6) {
				cone->size--;
				cone->construct();
			}
			break;
		default:
			break;
		}
	}
}

void scrollCallback(GLFWwindow* window, double xOffs, double yOffs) {
	cone->V += yOffs * 0.1;
}

void animation() {
	if (cone->flour <= -1) 
		cone->speed = -cone->speed;
	clock_t local_time = clock();
	double time = ((double)local_time) / CLOCKS_PER_SEC - ((double)global_time) / CLOCKS_PER_SEC;
	global_time = local_time;
	cone->speed -= time;
	double ypos = time * cone->speed;
	for (vector<vector<point>>::iterator iter = cone->points.begin(); iter != cone->points.end(); ++iter) 
		for (vector<point>::iterator iter1 = (*iter).begin(); iter1 != (*iter).end(); ++iter1) 
			(*iter1).y += ypos;	
	cone->flour += ypos;
}

void display(GLFWwindow* window) {

	glClearColor(0.1, 0.1, 0.1, 0.1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	if (Anime) animation();

	if (TextureVision) glEnable(GL_TEXTURE_2D);
	else glDisable(GL_TEXTURE_2D);
	
	glTranslatef(cone->xpos, cone->ypos, 0);
	glRotated(cone->xalpha, 0, 1, 0);
	glRotated(cone->yalpha, 1, 0, 0);
	glRotated(cone->zalpha, 0, 0, 1);
	glScalef(cone->V, cone->V, cone->V);
	cone->draw();
	glfwSwapBuffers(window);
	glfwPollEvents();
}

int main() {
	if (!glfwInit()) exit(1);
	glfwWindowHint(GLFW_SAMPLES, 4);
	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	int width = 800, height = 800;

	GLFWwindow* window = glfwCreateWindow(width, height, "L6", NULL, NULL);

	if (!window) {
		glfwTerminate();
		exit(1);
	}
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, keyCallback);
	glfwSetScrollCallback(window, scrollCallback);

	glEnable(GL_NORMALIZE);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material_diffuse);

	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 50);
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, light0_spot_direction);
	glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 30.0);

	int w = 512, h = 512;
	unsigned char* image = SOIL_load_image("gr.bmp", &w, &h, 0, SOIL_LOAD_RGB);
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	glEnable(GL_DEPTH_TEST);
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(matr);
	gluPerspective(60, (float)width / height, 1, 100);
	glTranslated(-1.5, -2, -6);

	glRotated(45, 0, 1, 0);

	while (!glfwWindowShouldClose(window)) 
		display(window);
	
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
