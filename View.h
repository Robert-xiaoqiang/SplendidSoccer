#ifndef VIEW_H_
#define VIEW_H_

#include "glew.h"
#include "GL/freeglut.h"
enum Mode {FIRST, THIRD};

void mouse(int button, int state, int x, int y);
void viewWork(int x, int y);
void mouseMotion(int x, int y);
void passiveMouseMotion(int x, int y);
void reshape(int w, int h);
void changeView();
void going();
void processKeyboard(int key, int x, int y);

class View {
public:
	static void getModelViewMatrix(GLfloat *matrix); // 4 * 4
	static void getViewMatrix(GLfloat *matrix);
	static void getProjectionMatrix(GLfloat *matrix);
	static void getEye(GLfloat *eyePosition);
	static void getAt(GLfloat *atPostion);
	static GLfloat getFovy();
	static int getWidth();
	static int getHeight();
	static void setEye(GLfloat *eyePosition);
	static void setAt(GLfloat *atPosition);
};

#endif