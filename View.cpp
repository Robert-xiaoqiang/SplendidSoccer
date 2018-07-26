#define FREEGLUT_STATIC
#include "View.h"
#include "General.h"
#include <cmath>
#include <iostream>

#define GLUT_WHEEL_UP   3
#define GLUT_WHEEL_DOWN 4

const double PI = 3.142;

extern State current; // from main.cpp

// @Note: can't be static
// must be extern -> for SkyBox reference
// static double eye[3] = { 10, 3.0, -1.2 };   // stadium
// static double eye[3] = { 10.0, 1.0, -1.2 }; // reflect
static double eye[3] = { 50.0, 4.0, 0.0 };     // beginMenu
static double at[3] = { 0.0, 4.0, 0.0 };
static int width, height;
static double fovy = 60.0;

// third person default
Mode viewMode = THIRD;
static int leftPulse = 0;
static int cx0 = 0, cy0 = 0;
int _time;

void View::getModelViewMatrix(GLfloat *matrix)
{
	glUseProgram(0);
	glMatrixMode(GL_MODELVIEW);
	glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
}

void View::getViewMatrix(GLfloat *matrix)
{
	glUseProgram(0);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	gluLookAt(eye[0], eye[1], eye[2], at[0], at[1], at[2], 0.0, 1.0, 0.0);
	glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
	glPopMatrix();
}

void View::getProjectionMatrix(GLfloat *matrix)
{
	glUseProgram(0);
	glGetFloatv(GL_PROJECTION_MATRIX, matrix);
}

void View::getEye(GLfloat *eyePosition)
{
	eyePosition[0] = (GLfloat)eye[0];
	eyePosition[1] = (GLfloat)eye[1];
	eyePosition[2] = (GLfloat)eye[2];
	changeView();
	return;
}

void View::getAt(GLfloat *atPosition)
{
	atPosition[0] = (GLfloat)at[0];
	atPosition[1] = (GLfloat)at[1];
	atPosition[2] = (GLfloat)at[2];
	changeView();
	return;
}

void View::setEye(GLfloat *eyePosition)
{
	eye[0] = eyePosition[0];
	eye[1] = eyePosition[1];
	eye[2] = eyePosition[2];
	changeView();
}

void View::setAt(GLfloat *atPosition)
{
	at[0] = atPosition[0];
	at[1] = atPosition[1];
	at[2] = atPosition[2];
	changeView();
}

GLfloat View::getFovy()
{
	return (GLfloat)fovy;
}

int View::getWidth()
{
	return width;
}

int View::getHeight()
{
	return height;
}

// !!! view & projection & viewport !!! specification/transformation
void reshape(int w, int h)
{
	width = w; height = h;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (double)w / h, 1.0, 200.0); // perspective projection
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();                                // view specification
	gluLookAt(eye[0], eye[1], eye[2], at[0], at[1], at[2], 0.0, 1.0, 0.0);
}

void changeView()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fovy, (double)width / height, 0.1, 500.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();                            // view specification
	gluLookAt(eye[0], eye[1], eye[2], at[0], at[1], at[2], 0.0, 1.0, 0.0);
}


void going()
{
	//start0 eye[0] = 48.319942, eye[1] = 28.846288, eye[2] = -21.849183
	//point2 eye[0] = 22.151530, eye[1] = 4.429236, eye[2] = -6.552708
	//point1 eye[0] = 7.293059, eye[1] = 13.906723, eye[2] = -13.064267
	//ending  eye[0] = 10.874907, eye[1] = 2.600000, eye[2] = -1.940925

	if(_time < 100)
	{
		eye[0] -= (48.319942 - 7.293059) / 100;
		eye[1] -= (28.846288 - 13.906723) / 120;
		eye[2] += (21.849183 - 13.064267) / 110;
		_time++;
		//printf("_time = %d\n", _time);
		changeView();
	} else if(_time == 100)
	{
		eye[0] = 22.151530;
		eye[1] = 4.429236;
		eye[2] = -6.552708;
		_time++;
		changeView();
	} else if(_time > 100 && _time < 199)
	{
		eye[0] -= 0.2256 / 2;
		eye[1] -= 0.0364 / 2;
		eye[2] += 0.0922 / 2;
		_time++;
		//printf("_time = %d\n", _time);
		changeView();
	} else if(_time > 199 && _time < 220)
	{
		eye[0] = 10.874907;
		eye[1] = 4.600000;
		eye[2] = -3.940925;
		_time++;
		changeView();
	}
	//printf("eye[0] = %lf, eye[1] = %lf, eye[2] = %lf\n", eye[0], eye[1], eye[2]);	

}

// keyboard 'E' -> eye rotate upward
void upRotate()
{
	double length = sqrt((at[0] - eye[0]) * (at[0] - eye[0]) + (at[2] - eye[2]) * (at[2] - eye[2]));
	double xi = (eye[0] - at[0]) / length;
	double zi = (eye[2] - at[2]) / length;
	double v[2];
	v[0] = length;
	v[1] = eye[1] - at[1];
	rotate(0.0, 0.0, v, 0.1);
	eye[0] = at[0] + v[0] * xi;
	eye[2] = at[2] + v[0] * zi;
	eye[1] = at[1] + v[1];
}

// keyboard 'Q' -> eye rotate downward
void downRotate()
{
	double length = sqrt((at[0] - eye[0]) * (at[0] - eye[0]) + (at[2] - eye[2]) * (at[2] - eye[2]));
	double xi = (eye[0] - at[0]) / length;
	double zi = (eye[2] - at[2]) / length;
	double v[2];
	v[0] = length;
	v[1] = eye[1] - at[1];
	rotate(0.0, 0.0, v, -0.1);
	eye[0] = at[0] + v[0] * xi;
	eye[2] = at[2] + v[0] * zi;
	eye[1] = at[1] + v[1];
}

void processKeyboard(int key, int x, int y)
{
	double length = sqrt((at[0] - eye[0]) * (at[0] - eye[0]) +
		(at[2] - eye[2]) * (at[2] - eye[2]) +
		(at[1] - eye[1]) * (at[1] - eye[1]));
	double xi = (at[0] - eye[0]) / length;
	double yi = (at[1] - eye[1]) / length;
	double zi = (at[2] - eye[2]) / length;
	double v1[3], v2[3] = { 0.0, 1.0, 0.0 };
	//(xi, zi) is identity vector in x-z plan 
	// direction of vector(eye->at) is x-z plan 
	static double upDownTotal = 0.0;
	v1[0] = xi, v1[1] = 0, v1[2] = zi;
	vec3 v3;
	cross(vec3((float)v1[0], (float)v1[1], (float)v1[2]), vec3((float)v2[0], (float)v2[1], (float)v2[2]), v3);
	switch(key) {
		// wark w-a-s-d
	case 'w':
		eye[0] += xi * 0.5;
		eye[1] += yi * 0.5;
		eye[2] += zi * 0.5;
		break;
	case 's':
		eye[0] += -xi * 0.5;
		eye[1] += -yi * 0.5;
		eye[2] += -zi * 0.5;
		break;
	case 'a':
		eye[0] += -v3.x * 0.5;
		eye[2] += -v3.z * 0.5;

		at[0] = eye[0] + xi * length;
		at[2] = eye[2] + zi * length;
		break;
	case 'd':
		eye[0] += v3.x * 0.5;
		eye[2] += v3.z * 0.5;

		at[0] = eye[0] + xi * length;
		at[2] = eye[2] + zi * length;
		break;
	case 'q':
		if(viewMode == THIRD) {
			if(upDownTotal <= 1.5) {
				upRotate();
				upDownTotal += 0.1;
			} else if(upDownTotal <= -1.5) {
				upDownTotal += 0.1;
			}
		}
		break;
	case 'e':
		if(viewMode == THIRD) {
			if(upDownTotal >= -1.5) {
				downRotate();
				upDownTotal -= 0.1;
			} else if(upDownTotal >= 1.5) {
				upDownTotal -= 0.1;
			}
		}
		break;
	case 9:        // change MODE for different viewing transformations
		if(viewMode == FIRST) {
			viewMode = THIRD;
			glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
		} else {
			viewMode = FIRST;
			glutSetCursor(GLUT_CURSOR_NONE);
		}
		break;
	case 27:
		exit(0);
		break;
	case 'z':
		if(current == GAMING) {

		}
	default:
		break;
	}
	changeView();
	glutPostRedisplay();
}

void mouse(int button, int state, int x, int y)
{
	switch(button) {
	case GLUT_LEFT_BUTTON:
		if(state == GLUT_DOWN) {
			leftPulse = 1;           // stick to left-button
		} else if(state == GLUT_UP) {
			leftPulse = 0;
		}
		break;
	case GLUT_WHEEL_UP:
		fovy -= 0.5;
		break;
	case GLUT_WHEEL_DOWN:
		fovy += 0.5;
		break;
	case GLUT_MIDDLE_BUTTON:
		break;
	case GLUT_RIGHT_BUTTON:
		if(state == GLUT_DOWN) {
			current = GAMING;
			GLfloat newEye[3] = { 70.0, -50.0, 0.0 };     // gaming position
			GLfloat newAt[3] = { 20.0, -70.0, 0.0 };
			View::setAt(newAt);
			View::setEye(newEye);
		}
		break;
	}
	
	cx0 = x; cy0 = y;
	changeView();
	glutPostRedisplay();
}

void viewWork(int x, int y)
{
	double v[2];
	int deltaX = x - cx0;
	int deltaY = y - cy0;
	// rotate in left-right
	// the target of viewing left/right rotation
	if(deltaX < 0) {
		v[0] = at[0], v[1] = at[2]; //x, z;
		rotate(eye[0], eye[2], v, deltaX * 4 * PI / width);
		at[0] = v[0], at[2] = v[1];
	} else if(deltaX > 0) {
		v[0] = at[0], v[1] = at[2]; //x, z;
		rotate(eye[0], eye[2], v, deltaX * 4 * PI / width);
		at[0] = v[0], at[2] = v[1];
	}

	// rotate in up-down
	// the target of viewing up/down rotation
	if(deltaY < 0) {
		double length = sqrt((at[0] - eye[0]) * (at[0] - eye[0]) + (at[2] - eye[2]) * (at[2] - eye[2]));
		double xi = (at[0] - eye[0]) / length;
		double zi = (at[2] - eye[2]) / length;
		v[0] = length;
		v[1] = at[1] - eye[1];
		rotate(0.0, 0.0, v, -deltaY * 4 * PI / height);
		at[0] = eye[0] + v[0] * xi;
		at[2] = eye[2] + v[0] * zi;
		at[1] = eye[1] + v[1];
	} else if(deltaY > 0) {
		double length = sqrt((at[0] - eye[0]) * (at[0] - eye[0]) + (at[2] - eye[2]) * (at[2] - eye[2]));
		double xi = (at[0] - eye[0]) / length;
		double zi = (at[2] - eye[2]) / length;
		v[0] = length;
		v[1] = at[1] - eye[1];
		rotate(0.0, 0.0, v, -deltaY * 4 * PI / height);
		at[0] = eye[0] + v[0] * xi;
		at[2] = eye[2] + v[0] * zi;
		at[1] = eye[1] + v[1];
	}
}

void passiveMouseMotion(int x, int y)
{
	if((cx0 != x || cy0 != y) && viewMode == FIRST) {
		viewWork(x, y);
	}

	cx0 = x; cy0 = y;
	changeView();
	glutPostRedisplay();
}

void mouseMotion(int x, int y)
{
	if((cx0 != x || cy0 != y) && leftPulse == 1 && viewMode == THIRD) {
		viewWork(x, y);
	}

	cx0 = x; cy0 = y;
	changeView();
	glutPostRedisplay();
}